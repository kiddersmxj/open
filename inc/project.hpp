#ifndef Kprojectproject
#define Kprojectproject

#include <string>
#include <iostream>
#include <filesystem>

#include "config.hpp"

class Project {
    public:
        Project();
        Project(std::string Proj);
        Project(std::string Proj, std::string File);
        /* ~Project(); */
        std::string Directory();
        std::vector<std::string> Files();
        std::vector<std::string> List();
        std::vector<std::string> List(std::string D);
    private:
        std::string Dir;
        std::vector<std::string> Fs;
};

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
