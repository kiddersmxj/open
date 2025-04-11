cmake_minimum_required(VERSION 3.26)
cmake_policy(VERSION 3.26)



include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)

# --- Fetch external libraries ------------------------------------------------
#
# Fetch the updated JSON library (v3.12.0) via URL.
FetchContent_Declare(
  json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

# Make both dependencies available.
FetchContent_MakeAvailable(json)
 
project(open
    LANGUAGES CXX
    VERSION 0.0.0
)

add_executable(open src/open.cpp)

find_package(std-k REQUIRED)

target_include_directories(open 
    PRIVATE inc
)

target_sources(open
    PRIVATE src/open.cpp
    PRIVATE src/send.cpp
    PRIVATE src/screen.cpp
    PRIVATE src/project.cpp
)

install(
    TARGETS open
    DESTINATION /usr/local/bin/
)

if(test)
    message("Testing enabled")
    add_compile_definitions(TEST)
endif()


target_link_libraries(open
    PRIVATE std-k
    PUBLIC X11
    PUBLIC Xtst
    PRIVATE nlohmann_json::nlohmann_json
)

# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree. 
