#!/bin/sh

source ~/.bashrc
cd "$@"

logdatetimeFunc() {
    printf $(date '+%Y-%m-%d[%H:%M:%S]')
}

r() {
    temp_file="$(mktemp -t "ranger_cd.XXXXXXXXXX")"
    # Dynamically generate the log file path with the current datetime
    log_file="/log/ranger/$(logdatetimeFunc).ranger.log"
    python ~/git/cli/ranger/ranger.py --choosedir="$temp_file" --logfile "$log_file" -- "${@:-$PWD}"
    if chosen_dir="$(cat -- "$temp_file")" && [ -n "$chosen_dir" ] && [ "$chosen_dir" != "$PWD" ]; then
        cd -- "$chosen_dir"
    fi
    rm -f -- "$temp_file"
}

"r"

exec "$SHELL"
