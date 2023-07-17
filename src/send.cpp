#include "../inc/send.hpp"

Send::Send() {
    display = XOpenDisplay(NULL); // Open the default display connection
    if (!display) {
        printf("Error opening the display.\n"); // If the display connection fails, print an error message
        throw "Error openning the display";
    }
}

Send::~Send() {
    for(auto key: Keys) {
        KeyCode Key = XKeysymToKeycode(display, key);
        XTestFakeKeyEvent(display, Key, False, 0);
    }
    XCloseDisplay(display); // Close the display connection
}

void Send::Press(KeySym key) {
    Keys.push_back(key);
    KeyCode Key = XKeysymToKeycode(display, key);
    XTestFakeKeyEvent(display, Key, True, 0);
    XTestFakeKeyEvent(display, Key, False, 0);
}

void Send::Hold(KeySym key) {
    Keys.push_back(key);
    KeyCode Key = XKeysymToKeycode(display, key);
    XTestFakeKeyEvent(display, Key, True, 0);
}

void Send::Release(KeySym key) {
    Keys.erase(std::remove(Keys.begin(), Keys.end(), key), Keys.end());
    KeyCode Key = XKeysymToKeycode(display, key);
    XTestFakeKeyEvent(display, Key, False, 0);
}

void Send::DisableInput() {
    k::ExecCmd(DisableCMD);
}

void Send::EnableInput() {
    k::ExecCmd(EnableCMD);
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
