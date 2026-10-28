# open [![CMake](https://img.shields.io/github/actions/workflow/status/kiddersmxj/open/cmake.yml?style=for-the-badge)](https://github.com/kiddersmxj/open/actions/workflows/cmake.yml)

Opens a project from `$devel` onto the next free dwm tag, spawning `st` clients
for the files, a ranger client, or a claude session.

## install

```sh
./install.sh
```

Builds to `/usr/local/bin/open` and symlinks the supporting scripts
(`OpenLaunchCmd.sh`, `RangerLaunchCmd.sh`, `ClaudeLaunchCmd.sh`) alongside it.

It runs without a password prompt, so an agent can run it unattended. The
install step is `sudo cmake`, and `cmake` is passwordless in sudoers on
mxj-arch-2 (the twin gets the same `/etc` through custos). The symlink steps use
plain `sudo` but only run on a first install, when a script is missing from
`/usr/local/bin`. A rebuild after a change is just `./install.sh`.

## usage

```
usage: open [ -p project ] [ -f file in project ]
            [ -r ] [ -c ] [ -h/-v ]
with no arguments (and not inside a project dir) opens ranger in $devel
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit
    -p / --project-name specifiy project name (pass '-p list' to list)
    -f / --file-name    specifiy file name (pass '-p name -f list' to list)
    -r / --ranger       also spawn a ranger client in passed project dir
    -c / --claude       open project with claude ('-cc' for clc, '-cr' for clr)
    -t / --tag-here     opens request on current tag
    -n / --new          new tag even if the project is already open somewhere
    -d / --destroy      destorys window (lowkey useless possibly except for morse)
    -R / --remote       place a claude window for dir without touching the view
                        (dir: absolute, ~/..., or a name under $devel)
                        prints one json line, see README
    --dry-run           with -R, print the decision json but spawn nothing
```

The project name is inferred from the working directory when `-p` is omitted, so
running `open` inside a project dir is enough. Outside one, bare `open` falls
back to a ranger client in `$devel` itself (`-c` gives claude there instead).

## already open

The lookup only applies to a project named with `-p`. When the name was
inferred from the working directory you are already standing in the project, so
a jump would be a no-op: bare `open` inside a project dir always spawns on a
fresh tag as it always did.

If the project is named and already running somewhere, `open` takes you to that
tag instead of claiming a new one. A bare `open -p foo` is then just a tag switch —
nothing is spawned, not even the ranger it would normally give you. Anything
asked for explicitly still gets opened, on that same tag:

```sh
open -p foo             # switch to foo's tag
open -p foo -c          # switch, then add a claude window there
open -p foo -f main.cpp # switch, then open main.cpp there
open -p foo -n          # ignore all that, fresh tag as before
```

A project counts as open when any window's working directory is the project dir
or below it, so a terminal you `cd`'d into `foo/src` by hand counts too. `-t`
skips the lookup entirely, since it already means "here", as does inferring the
project from the working directory.

The lookup asks dwm for a session snapshot (`fsignal:7`) and reads the cwd and
tag of every client out of `~/.cache/dwm/session`, which costs 250-400ms —
dwm writes `session.raw` immediately but `session-enrich.py` needs a moment to
resolve each client's cwd through `/proc`. If the json does not refresh in time
we fall back to matching the `st:<dir>` window titles in the raw dump. Clients
on the stash tag are ignored, so a stashed workspace does not count as open.
Where the project spans several tags the busiest one wins.

## claude

`-c` opens the tag with a plain terminal plus a claude window. The suffix picks
which alias runs:

| flag  | alias | runs               |
|-------|-------|--------------------|
| `-c`  | `cl`  | `claude`           |
| `-cc` | `clc` | `claude --continue`|
| `-cr` | `clr` | `claude --resume`  |

`ClaudeLaunchCmd.sh` sources `~/devel/home-k/scripts/aliases.sh` and expands the
alias by hand, so it always runs whatever those aliases currently mean — move
that scripts repo and the path in the script needs updating. A full `bash -i`
picks them up too but costs ~1s to source the whole bashrc chain. Once claude
exits the window `exec`s `$SHELL`, leaving a normal terminal with ble.sh.

The claude window is launched last and then zoomed (`MODKEY+Return`) so it lands
as master, since dwm attaches below the selected client rather than at the head.

`-c` takes its suffix attached, so it must come last in a cluster: `-rc` and
`-tc` are fine, `-cp foo` reads the `p` as a suffix. Likewise `-p` takes its
argument attached, so `-pc foo` means project `c` — use `-p foo -c`.

## remote

`open -R <dir>` is for when nobody is at the keyboard, e.g. a service starting
a session from the phone. It never switches the view, never sends keys and
never takes focus. It needs `DISPLAY` set and dwm-k's instance placement: `st
-n dwm-tag-<N>` puts a window on tag N (zero based), `st -n dwm-ws-<name>` puts
it into stashed workspace `<name>`.

`<dir>` is an absolute path, `~/...`, or a path under `$devel` (`foo`,
`eNBe/desk`). It must exist. It always launches claude (`cl`, or `-cc`/`-cr`).
`-S <id>` reopens that chat instead (`claude --resume <id>`); the id must be
hex and dashes, since the launch script evals it. custos uses this to bring
back a session that had no window on the other machine.
Placement, first match wins:

| `placed`    | when                                              | spawns            |
|-------------|---------------------------------------------------|-------------------|
| `tag`       | a live tag has a window in the dir or below it    | claude only       |
| `stash`     | a stashed workspace has a window in the dir       | claude only       |
| `new-tag`   | a free tag exists (past `FIRSTTAG`)               | terminal + claude |
| `new-stash` | no free tag                                       | terminal + claude |

Live tags use the same lookup as above, busiest tag wins. Stashed workspaces
are matched the same way from the `workspace` field enrich writes on each
client. A new stashed workspace is named after the dir's basename, with `-2`,
`-3`... added if that name is already in use.

On success it prints one json line to stdout and exits 0:

```json
{"dir":"/home/kidders/devel/foo","placed":"new-tag","tag":"A9","terminal":true,"workspace":null}
{"dir":"/home/kidders/devel/blinder","placed":"stash","tag":null,"terminal":false,"workspace":"blinder"}
```

| field       | meaning                                                   |
|-------------|-----------------------------------------------------------|
| `dir`       | the resolved, canonical dir                               |
| `placed`    | `tag`, `stash`, `new-tag` or `new-stash`                  |
| `tag`       | tag as the bar labels it (`1`-`9`, `A1`-`A9`), else null  |
| `workspace` | stashed workspace name, else null                         |
| `terminal`  | whether a plain terminal was spawned too                  |
| `dry_run`   | only present, as true, under `--dry-run`                  |

On failure it prints one line to stderr and exits non-zero (no such
directory, `DISPLAY` not set).

Run from a systemd user service, the `st` windows land in that service's
cgroup, so stopping or restarting the service kills them. Start `open` with
`systemd-run --user --scope` or set `KillMode=process` in the unit.

