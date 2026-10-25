#ifndef Kscreenscreen
#define Kscreenscreen

#include "config.hpp"
#include "send.hpp"

#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <std-k.hpp>

namespace S {
    class Screen {
        public:
            Screen();
            void Add(std::string ProgramPath);
            void Spawn(int Tag);
            void Spawn();
            void Ranger(std::string Dir);
            void Claude(std::string Dir, std::string Alias);
            void Focus(int Tag);
            int FindProjectTag(std::string Dir);
            bool RefreshSession();
            int LiveTag(std::string Dir, bool Fresh);
            std::string FindProjectWorkspace(std::string Dir);
            std::vector<std::string> WorkspaceNames();
            int FreeTag();
            void Place(std::string Instance, std::string Dir, std::string Alias, bool Terminal);
        private:
            std::vector<int> AvailableTags;
            std::vector<std::string> Programs;
            std::vector<std::string> Rangers;
            std::vector<std::string> Claudes;
            std::vector<KeySym> TagKeys = { XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9 };
            void RefreshAvailable();
            void Moveto(int Tag);
            void CountTags(unsigned int Mask, std::map<int, int> &Counts);
            int BestTag(std::map<int, int> &Counts);
            int SnapshotTag(std::string Dir);
            int RawTag(std::string Dir);
            bool ReadSession(nlohmann::json &Session);
            bool InDir(std::string Cwd, std::string Dir);
            std::string Quote(std::string Arg);
            void Zoom();
            void Launch(std::string Program);
    };
}

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
