# open [![CMake](https://img.shields.io/github/actions/workflow/status/kiddersmxj/open/cmake.yml?style=for-the-badge)](https://github.com/kiddersmxj/open/actions/workflows/cmake.yml)

Opens a project from `$devel` onto the next free dwm tag, spawning `st` clients
for the files, a ranger client, or a claude session.

## install

```sh
./install.sh
```

Builds to `/usr/local/bin/open` and symlinks the supporting scripts
(`OpenLaunchCmd.sh`, `RangerLaunchCmd.sh`, `ClaudeLaunchCmd.sh`) alongside it.

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
    -d / --destroy      destorys window (lowkey useless possibly except for hamza)
```

The project name is inferred from the working directory when `-p` is omitted, so
running `open` inside a project dir is enough. Outside one, bare `open` falls
back to a ranger client in `$devel` itself (`-c` gives claude there instead).

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
