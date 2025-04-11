#ifndef Kopenopen  // Header guard to prevent multiple inclusion
#define Kopenopen

#include "config.hpp"
#include "screen.hpp"
#include "project.hpp"

#include <std-k.hpp>
#include <chrono>
#include <thread>
#include <getopt.h>
#include <unistd.h>
#include <nlohmann/json.hpp>  // JSON parsing library
using json = nlohmann::json;  // Namespace alias for convenience

void Usage();
void Usage(std::string Message);
void PrintVersion();

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
