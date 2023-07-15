#ifndef Kscreenscreen
#define Kscreenscreen

#include "config.hpp"
#include "send.hpp"

#include <iostream>
#include <std-k.hpp>

namespace S {
    class Screen {
        public:
            Screen();
            void Add(std::string ProgramPath);
            void Spawn(int Tag);
            void Spawn();
        private:
            std::vector<int> AvailableTags;
            std::vector<std::string> Programs;
            std::vector<KeySym> TagKeys = { XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_7, XK_8, XK_9 };
            void RefreshAvailable();
            void Moveto(int Tag);
    };
}

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
