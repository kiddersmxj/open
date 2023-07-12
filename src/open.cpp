#include "../inc/open.hpp"

int main(int argc, char** argv) { 
    Display* display = XOpenDisplay(NULL); // Open the default display connection
    if (!display) {
        printf("Error opening the display.\n"); // If the display connection fails, print an error message
        return 1; // Return with an error code
    }

    Send Send(display);

    Send.Hold(XK_Super_L);
    Send.Press(XK_5);
    Send.Release(XK_Super_L);
    Send.EnableInput();

    XCloseDisplay(display); // Close the display connection
    return 0;
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
