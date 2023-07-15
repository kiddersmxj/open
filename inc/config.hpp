#ifndef Kconfig
#define Kconfig                                

#include <std-k.hpp>
#include <iostream>

/* const std::string DisableCMD = R"(xinput --enable "$(xinput list | grep "AT" | awk -F ' ' '{print $7}' | awk -F '=' '{print $2}') ")"; */
/* const std::string EnableCMD = R"(xinput --disable "$(xinput list | grep "AT" | awk -F ' ' '{print $7}' | awk -F '=' '{print $2}') ")"; */

/* const std::string EnableCMD = "~/devel/bin-k/bash/keyboardctrl.sh 1"; */
/* const std::string DisableCMD = "~/devel/bin-k/bash/keyboardctrl.sh 0"; */

const std::string EnableCMD = "xinput --enable 11";
const std::string DisableCMD = "xinput --disable 11";

const std::string ProgramName = "open";
const std::string Version = "0.0.0";
const std::string UsageNotes = R"(usage: open
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit
)";

const std::string GetAvailableTagsCmd = "xsetroot -name \"fsignal:6\"; sleep 0.1";
const std::string AvailableTagsFilePath = HOME + "/.cache/dwm/emptytags.txt";
const std::string Launch = "st -e";

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
