#!/bin/bash

BashInstall="/usr/local/bin"
OpenLaunchFile="OpenLaunchCmd.sh"
RangerLaunchFile="RangerLaunchCmd.sh"
InstallPath="$(pwd)"

if [[ $1 != "" ]]; then
    ARGS="-D$1"
fi

if [ ! -f $BashInstall/$OpenLaunchFile ]; then
    echo "Please install supporting scripts"
    sudo ln -sf $InstallPath/scripts/$OpenLaunchFile $BashInstall/$OpenLaunchFile
fi
if [ ! -f $BashInstall/$RangerLaunchFile ]; then
    echo "Please install supporting scripts"
    sudo ln -sf $InstallPath/scripts/$RangerLaunchFile $BashInstall/$RangerLaunchFile
fi

mkdir -p build
script -q -c "cmake -B build $ARGS && cmake --build build && sudo cmake --install build" | tee build/build.log


# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree. 
