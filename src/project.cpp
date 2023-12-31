#include "../inc/project.hpp"

namespace fs = std::filesystem;

Project::Project(std::string Proj) {
    for (const auto & entry : fs::directory_iterator(DevelPath)) {
        if(Proj == entry.path().filename()) {
            Dir = entry.path();
            break;
        }
    }
}

Project::Project() {};

Project::Project(std::string Proj, std::string File) {
    for (const auto & entry : fs::directory_iterator(DevelPath)) {
        if(Proj == entry.path().filename()) {
            Dir = entry.path();
            break;
        }
    }
    bool Found = 0;
    for(const auto & entry : fs::directory_iterator(Dir)) {
        if(File == entry.path().filename()) {
            Found = 1;
            Fs.push_back(entry.path());
            break;
        }
    }
    if(!Found) {
        for(const auto & entry : fs::directory_iterator(Dir + "/inc")) {
            if(File + ".hpp" == entry.path().filename()) {
                Found = 1;
                Fs.push_back(entry.path());
                break;
            }
        }
        for(const auto & entry : fs::directory_iterator(Dir + "/src")) {
            if(File + ".cpp" == entry.path().filename()) {
                Found = 1;
                Fs.push_back(entry.path());
                break;
            }
        }
    }
    /* k::VPrint(Fs); */
}

std::string Project::Directory() {
    return Dir;
}

std::vector<std::string> Project::Files() {
    return Fs;
}

std::vector<std::string> Project::List(std::string D) {
    std::vector<std::string> L;
    for(const auto & entry : fs::directory_iterator(D)) {
        L.push_back(entry.path().filename());
    }
    for(const auto & entry : fs::directory_iterator(D + "/inc")) {
        L.push_back(entry.path().filename());
    }
    for(const auto & entry : fs::directory_iterator(D + "/src")) {
        L.push_back(entry.path().filename());
    }
    return L;
}

std::vector<std::string> Project::List() {
    std::vector<std::string> L;
    for(const auto & entry : fs::directory_iterator(DevelPath)) {
        L.push_back(entry.path().filename());
    }
    return L;
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
