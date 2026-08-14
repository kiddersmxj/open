#include "../inc/screen.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

#include <nlohmann/json.hpp>

S::Screen::Screen() {
    RefreshAvailable();
}

void S::Screen::RefreshAvailable() {
    k::ExecCmd(GetAvailableTagsCmd);
    AvailableTags.clear();
    for(std::string Tag: k::ReadFileLines(AvailableTagsFilePath)) {
        if(Tag == "") continue;
        int T = std::atoi(std::move(Tag.c_str()));
        /* if(T > 6) */
        /*     T++; */
        // Remove less than tag 3 (2 for zero based)
        if(T > FIRSTTAG)
            AvailableTags.push_back(T);
    }
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
    for(std::string Claude: Claudes) {
        k::Sleep(MAPDELAY);
        k::ExecCmd(Claude);
    }
    // Claude window is launched last so it holds focus, zoom it to master
    if(Claudes.size() > 0) {
        k::Sleep(ZOOMDELAY);
        Zoom();
    }
}

void S::Screen::Spawn() {
    if(AvailableTags.size() == 0) throw "No available tags";
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

// Switch to a tag without spawning anything (Spawn(int) ignores tag 0)
void S::Screen::Focus(int Tag) {
    Moveto(Tag);
}

// One vote per client, on the lowest tag it holds that is not the stash tag
void S::Screen::CountTags(unsigned int Mask, std::map<int, int> &Counts) {
    for(int Bit = 0; Bit < 32; Bit++) {
        if(Bit == STASHTAG) continue;
        if(Mask & (1u << Bit)) {
            Counts[Bit]++;
            return;
        }
    }
}

// Ask dwm for a fresh session snapshot and find the tag the project sits on,
// -1 if nothing is running in it
int S::Screen::FindProjectTag(std::string Dir) {
    namespace fs = std::filesystem;
    std::error_code Ec;

    fs::file_time_type Before{};
    bool Existed = fs::exists(SessionFilePath, Ec);
    if(Existed)
        Before = fs::last_write_time(SessionFilePath, Ec);

    k::ExecCmd(SaveSessionCmd);

    // dwm writes the raw dump straight away, session-enrich.py adds the cwds
    // a couple of hundred ms later
    for(int Waited = 0; Waited < SESSIONWAIT; Waited += SESSIONPOLL) {
        k::Sleep(SESSIONPOLL);
        if(!fs::exists(SessionFilePath, Ec)) continue;
        if(Existed && fs::last_write_time(SessionFilePath, Ec) == Before) continue;
        return SnapshotTag(Dir);
    }

    // enrich did not land in time, settle for the titles in the raw dump
    return RawTag(Dir);
}

// Match on each client's real cwd, so a window sitting in a subdir counts too
int S::Screen::SnapshotTag(std::string Dir) {
    std::ifstream File(SessionFilePath);
    if(!File.is_open()) return -1;

    nlohmann::json Session;
    try {
        File >> Session;
    } catch (const nlohmann::json::exception &) {
        return -1;
    }
    if(!Session.contains("clients")) return -1;

    std::map<int, int> Counts;
    for(const auto &Client: Session["clients"]) {
        if(!Client.contains("cwd") || !Client.contains("tags")) continue;
        if(!Client["cwd"].is_string() || !Client["tags"].is_number()) continue;
        std::string Cwd = Client["cwd"].get<std::string>();
        if(Cwd != Dir && Cwd.rfind(Dir + "/", 0) != 0) continue;
        CountTags(Client["tags"].get<unsigned int>(), Counts);
    }
    return BestTag(Counts);
}

// The tag holding most of the project's windows, ties going to the lower tag
int S::Screen::BestTag(std::map<int, int> &Counts) {
    if(Counts.empty()) return -1;
    return std::max_element(Counts.begin(), Counts.end(),
        [](const std::pair<int, int> &A, const std::pair<int, int> &B) {
            return A.second == B.second ? A.first > B.first : A.second < B.second;
        })->first;
}

// Fallback: the terminals title themselves 'st:<dir>', which the raw dump has
int S::Screen::RawTag(std::string Dir) {
    std::map<int, int> Counts;
    for(std::string Line: k::ReadFileLines(SessionRawFilePath)) {
        // C <mon> <tags> <floating> <fullscreen> <x> <y> <w> <h> <win> <title>
        std::istringstream Fields(Line);
        std::string Kind;
        unsigned int Mask;
        if(!(Fields >> Kind) || Kind != "C") continue;
        std::string Mon;
        if(!(Fields >> Mon >> Mask)) continue;
        for(int i = 0; i < 7; i++) Fields >> Kind;
        std::string Title;
        if(!std::getline(Fields, Title)) continue;
        if(!Title.empty() && Title.front() == ' ') Title.erase(0, 1);
        std::string Want = "st:" + Dir;
        if(Title != Want && Title.rfind(Want + "/", 0) != 0) continue;
        CountTags(Mask, Counts);
    }
    if(Counts.empty()) return -1;

    return std::max_element(Counts.begin(), Counts.end(),
        [](const std::pair<int, int> &A, const std::pair<int, int> &B) {
            return A.second == B.second ? A.first > B.first : A.second < B.second;
        })->first;
}

void S::Screen::Zoom() {
    Send Send;

    Send.Hold(XK_Super_L);
    Send.Press(XK_Return);
    Send.Release(XK_Super_L);
    Send.EnableInput();
}

void S::Screen::Ranger(std::string Dir) {
    std::string Cmd = RLaunch1 + Dir + RLaunch2;
    Rangers.push_back(Cmd);
}

void S::Screen::Claude(std::string Dir, std::string Alias) {
    // Claude window pushed last so it ends up master
    Claudes.push_back(CLaunch1 + "\"" + Dir + "\"" + CLaunch2);
    Claudes.push_back(CLaunch1 + "\"" + Dir + "\" " + Alias + CLaunch2);
}

void S::Screen::Launch(std::string Program) {
    std::string Cmd = Launch1 + "vim " + Program + Launch2;
    k::ExecCmd(Cmd);
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
