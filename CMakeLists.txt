cmake_minimum_required (VERSION 3.11)
 
# --- Fetch libs --------------------------------------------------------------

include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(std-k
  GIT_REPOSITORY https://github.com/kiddersmxj/std-k
  GIT_TAG main
)
 
FetchContent_GetProperties(std-k)
if(NOT std-k_POPULATED)
  FetchContent_Populate(std-k)
  add_subdirectory(${std-k_SOURCE_DIR} ${std-k_BINARY_DIR})
endif()

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)
 
# ------------------------------------------------------------------------------

project(open
    LANGUAGES CXX
    VERSION 0.0.0
)

add_executable(open src/open.cpp)

target_include_directories(open 
    PRIVATE inc
    SHARED ${std-k_SOURCE_DIR}/inc
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


link_directories(${std-k_BINARY_DIR})
target_link_libraries(open
    PUBLIC std-k
    PUBLIC X11
    PUBLIC Xtst
    PRIVATE nlohmann_json::nlohmann_json
)

# Copyright (c) 2023, Maxamilian Kidd-May
# All rights reserved.

# This source code is licensed under the MIT license found in the
# LICENSE file in the root directory of this source tree. 
