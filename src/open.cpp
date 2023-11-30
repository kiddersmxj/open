#include "../inc/open.hpp"

int main(int argc, char** argv) { 
    int HelpFlag = 0;
    int VersionFlag = 0;
    int RangerFlag = 0;
    int HereFlag = 0;
    int DestroyFlag = 0;
    std::string ProjectName;
    std::string FileName;
    int opt;

    // Get opt option defenitions
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "project-name", required_argument, NULL, 'p' },
        { "file-name", required_argument, NULL, 'f' },
        { "ranger", no_argument, &RangerFlag, 1 },
        { "tag-here", no_argument, &HereFlag, 1 },
        { "destory", no_argument, &DestroyFlag, 1 },
        { 0 }
    };

    // Infinite loop, to be broken when we are done parsing options
    while (1) {
        opt = getopt_long(argc, argv, "hvp:f:rtd", Opts, 0);

        // A return value of -1 indicates that there are no more options
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();
                return EXIT_FAILURE;
            }
            break;
        } 

        switch (opt) {
        case 'h':
            HelpFlag = 1;
            break;
        case 'v':
            VersionFlag = 1;
            break;
        case 'p':
            ProjectName = optarg;
            break;
        case 'f':
            FileName = optarg;
            break;
        case 'r':
            RangerFlag = 1;
            break;
        case 't':
            HereFlag = 1;
            break;
        case 'd':
            DestroyFlag = 1;
            break;
        case '?':
            Usage();
            return EXIT_FAILURE;
        default:
            Usage();
            break;
        }
    }

    if(HelpFlag) {
        Usage();
        return EXIT_SUCCESS;
    }
    if(VersionFlag) {
    }

    std::vector<std::string> Out;
    std::string Path = std::filesystem::current_path().string();
    k::SplitString(Path, '/', Out, true);

    if(ProjectName == "") {
        Project Project;
        for(std::string P: Project.List()) {
            if(P == Out.back()) {
                ProjectName = Out.back();
                break;
            }
        }
    }

    int CurrentTag = -1;
    if(ProjectName != "" && FileName == "") {
        try {
            if(ProjectName == "list") {
                Project Project;
                k::VPrint(Project.List());
                return EXIT_SUCCESS;
            }
            Project Project(ProjectName);
            S::Screen Screen;
            Screen.Ranger(Project.Directory());
#ifndef TEST
            if(HereFlag)
                Screen.Spawn(CurrentTag);
            else
                Screen.Spawn();
#endif
        } catch (const char *Message) {
            Usage(Message);
            return 1;
        } catch (std::string Message) {
            Usage(Message);
            return 1;
        }
    } else if(ProjectName != "" && FileName != "") {
        try {
            if(FileName == "list") {
                Project Project(ProjectName);
                k::VPrint(Project.List(Project.Directory()));
                return EXIT_SUCCESS;
            }
            Project Project(ProjectName, FileName);
            S::Screen Screen;
            for(std::string File: Project.Files()) {
                Screen.Add(File);
            }
            if(RangerFlag)
                Screen.Ranger(Project.Directory());
#ifndef TEST
            if(HereFlag)
                Screen.Spawn(CurrentTag);
            else
                Screen.Spawn();
#endif
        } catch (const char *Message) {
            Usage(Message);
            return 1;
        } catch (std::string Message) {
            Usage(Message);
            return 1;
        }
    }

    if(DestroyFlag)
        k::ExecCmd("kill -4 " + std::to_string(getppid()));

    return 0;
}

void Usage() {
    std::cout << UsageNotes << std::endl;
}

void Usage(std::string Message) {
    std::cout << Message << std::endl;
    std::cout << UsageNotes << std::endl;
}

void PrintVersion() {
    std::cout << ProgramName << ": version " << Version << std::endl;
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
