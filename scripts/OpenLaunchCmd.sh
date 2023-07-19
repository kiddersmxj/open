#!/bin/sh -i

source ~/.bashrc
#
# Iterate over the arguments passed
for arg in "$@"
do
    # Check if the argument contains a file path
    if [[ $arg == *"/"* ]]; then
        # Extract the file path
        File="${arg%/*}"

        # Check if the second last component matches "inc", "include", "src", or "source"
        second_last_component="${File##*/}"
        if [[ $second_last_component == @(inc|include|src|source) ]]; then
            # Remove the second last component
            File="${File%/*}"
        fi

        # Print the modified file path
        echo "$File"
        break  # Exit the loop after finding the first file path
    fi
done

cd $File
"$@"
# exec "$SHELL"
