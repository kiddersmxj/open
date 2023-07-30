#include "../inc/screen.hpp"

S::Screen::Screen() {
    RefreshAvailable();
}

void S::Screen::RefreshAvailable() {
    k::ExecCmd(GetAvailableTagsCmd);
    AvailableTags.clear();
    for(std::string Tag: k::ReadFileLines(AvailableTagsFilePath)) {
        if(Tag == "") throw "No available tags";
        int T = std::atoi(std::move(Tag.c_str()));
        /* if(T > 6) */
        /*     T++; */
        // Remove less than tag 3 (2 for zero based)
        if(T > 2)
            AvailableTags.push_back(T);
    }
    if(AvailableTags.size() == 0) throw "No available tags";
}

void S::Screen::Add(std::string ProgramPath) {
    Programs.push_back(ProgramPath);
}

void S::Screen::Spawn(int Tag) {
    if(Tag > 0)
        Moveto(Tag);

    for(std::string Program: Programs)
        Launch(Program);
    for(std::string Ranger: Rangers)
        k::ExecCmd(Ranger);
}

void S::Screen::Spawn() {
    int Tag = AvailableTags.front();
    // Tag int does not really correspond to actual tag no
    int ActualTag = Tag + 1;
    if(ActualTag > 9) {
        ActualTag++;
    }
    /* std::cout << ActualTag << std::endl; */
    Spawn(Tag);
}

void S::Screen::Moveto(int Tag) {
    KeySym TagKey;
    bool Alt = 0;
    if(Tag >= 9) {
        Tag -= 9;
        Alt = 1;
    }
    TagKey = TagKeys.at(Tag);


    Send Send;

    Send.Hold(XK_Super_L);
    if(Alt)
        Send.Hold(XK_Alt_L);
    Send.Press(TagKey);
    if(Alt)
        Send.Release(XK_Alt_L);
    Send.Release(XK_Super_L);
    Send.EnableInput();
}

void S::Screen::Ranger(std::string Dir) {
    std::string Cmd = RLaunch1 + Dir + RLaunch2;
    Rangers.push_back(Cmd);
}

void S::Screen::Launch(std::string Program) {
    std::string Cmd = Launch1 + "vim " + Program + Launch2;
    k::ExecCmd(Cmd);
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
