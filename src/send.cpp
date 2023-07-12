#include "../inc/send.hpp"

Send::Send(Display* display) : display(display) {}

/* Send::~Send() { */
/*     for(auto key: Keys) { */
/*         KeyCode Key = XKeysymToKeycode(display, key); */
/*         XTestFakeKeyEvent(display, Key, False, 0); */
/*     } */
/* } */

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

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
