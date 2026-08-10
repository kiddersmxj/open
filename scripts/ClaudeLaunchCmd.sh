#!/bin/sh

source ~/.bashrc
[ -n "$1" ] && cd "$1"

# cl/clc/clr are interactive aliases so source them and expand by hand; a full
# 'bash -i' picks them up too but pulls in the whole bashrc chain, costing ~1s
# Title set as _prompt_func would, claude never runs a prompt to do it itself
if [ -n "$2" ]; then
    printf "\033]0;%s:%s\007" "st" "$PWD"
    shopt -s expand_aliases
    source $HOME/devel/home-k/scripts/vars.sh
    source $HOME/devel/home-k/scripts/aliases.sh
    eval "$2"
fi

exec "$SHELL"
