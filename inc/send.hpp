#ifndef Ksendsend
#define Ksendsend

#include "config.hpp"

#include <std-k.hpp>
#include <algorithm>
#include <vector>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

class Send {
    public:
        Send(Display* display);
        /* ~Send(); */
        void Press(KeySym Key);
        void Hold(KeySym Key);
        void Release(KeySym Key);
        void DisableInput();
        void EnableInput();
    private:
        Display* display;
        std::vector<KeySym> Keys;
};

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
