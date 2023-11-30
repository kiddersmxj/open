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
const std::string UsageNotes = R"(usage: open [ -p project ] [ -f file in project ]
            [ -r ] [ -h/-v ]
options:
    -h / --help         show help and usage notes
    -v / --version      print version and exit
    -p / --project-name specifiy project name (pass '-p list' to list)
    -f . --file-name    specifiy file name (pass '-p name -f list' to list)
    -r / --ranger       also spawn a ranger client in passed project dir
    -t / --tag-here     opens request on current tag
    -d / --destroy      destorys window (lowkey useless possibly except for hamza)
)";

const std::string GetAvailableTagsCmd = "xsetroot -name \"fsignal:6\"; sleep 0.1";
const std::string AvailableTagsFilePath = HOME + "/.cache/dwm/emptytags.txt";

const std::string Launch1 = R"(st -e OpenLaunchCmd.sh )"; // Add program in middle of two
const std::string Launch2 = R"( > /dev/null 2>&1 &)";

const std::string RLaunch1 = R"(st -e RangerLaunchCmd.sh )"; // Add program in middle of two
const std::string RLaunch2 = R"( > /dev/null 2>&1 &)";

const std::string DevelPath = getenv("devel");

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
