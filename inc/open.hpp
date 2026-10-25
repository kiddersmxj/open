// Header guard to prevent multiple inclusion
#ifndef Kopenopen
#define Kopenopen

#include "config.hpp"
#include "screen.hpp"
#include "project.hpp"

#include <std-k.hpp>
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <thread>
#include <getopt.h>
#include <unistd.h>
// JSON parsing library
// JSON parsing library
#include <nlohmann/json.hpp>
// Namespace alias for convenience
using json = nlohmann::json;

void Usage();
void Usage(std::string Message);
void PrintVersion();
std::string TagName(int Tag);
int Remote(std::string Target, std::string Alias, bool DryRun);
std::string ResolveDir(std::string Target);

#endif

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
