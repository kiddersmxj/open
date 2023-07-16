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
        //remove less than tag 4 (3 for zero based)
        T++;
        if(T > 4)
            AvailableTags.push_back(T);
    }
    if(AvailableTags.size() == 0) throw "No available tags";
}

void S::Screen::Add(std::string ProgramPath) {
    Programs.push_back(ProgramPath);
}

void S::Screen::Spawn(int Tag) {
    Moveto(Tag);

    for(std::string Program: Programs)
        Launch(Program);
}

void S::Screen::Spawn() {
    int Tag = AvailableTags.front();
    std::cout << Tag << std::endl;
    Spawn(Tag);
}

void S::Screen::Moveto(int Tag) {
    KeySym TagKey;
    bool Alt = 0;
    if(Tag >= 9) {
        Tag -= 10;
        Alt = 1;
    }
    TagKey = TagKeys.at(Tag);

    Display* display = XOpenDisplay(NULL); // Open the default display connection
    if (!display) {
        printf("Error opening the display.\n"); // If the display connection fails, print an error message
        throw "Error openning the display";
    }

    Send Send(display);

    Send.Hold(XK_Super_L);
    if(Alt)
        Send.Hold(XK_Alt_L);
    Send.Press(TagKey);
    if(Alt)
        Send.Release(XK_Alt_L);
    Send.Release(XK_Super_L);
    Send.EnableInput();

    XCloseDisplay(display); // Close the display connection
}

void S::Screen::Ranger(std::string Dir) {
    std::string Cmd = RLaunch1 + Dir + RLaunch2;
    k::ExecCmd(Cmd);
}

void S::Screen::Launch(std::string Program) {
    std::string Cmd = Launch1 + Program + Launch2;
    k::ExecCmd(Cmd);
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
