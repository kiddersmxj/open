#ifndef Kconfig
#define Kconfig                                

#include <iostream>

/* const std::string DisableCMD = R"(xinput --enable "$(xinput list | grep "AT" | awk -F ' ' '{print $7}' | awk -F '=' '{print $2}') ")"; */
/* const std::string EnableCMD = R"(xinput --disable "$(xinput list | grep "AT" | awk -F ' ' '{print $7}' | awk -F '=' '{print $2}') ")"; */

/* const std::string EnableCMD = "~/devel/bin-k/bash/keyboardctrl.sh 1"; */
/* const std::string DisableCMD = "~/devel/bin-k/bash/keyboardctrl.sh 0"; */

const std::string EnableCMD = "xinput --enable 11";
const std::string DisableCMD = "xinput --disable 11";

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
