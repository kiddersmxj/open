#include "../inc/open.hpp"

int main(int argc, char** argv) { 
    // Command line flags initialization
    int HelpFlag = 0;         // -h/--help flag
    int VersionFlag = 0;      // -v/--version flag
    int RangerFlag = 0;       // -r/--ranger flag to launch ranger file manager
    int HereFlag = 0;         // -t/--tag-here to spawn in current tag
    int DestroyFlag = 0;      // -d/--destroy to kill parent process
    std::string ProjectName;  // Project name from -p/--project-name
    std::string FileName;     // File name from -f/--file-name
    int opt;                  // Variable for getopt parsing

    // Long option definitions for getopt:
    // {name, has_arg, flag, val}
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

    // Command line option parsing loop
    while (1) {
        // Parse options using getopt_long:
        // - "hvp:f:rtd" specifies short options (colon = requires argument)
        // - Options structure defines long options mapping to flags
        opt = getopt_long(argc, argv, "hvp:f:rtd", Opts, 0);

        // Exit loop when no more options (-1 return value)
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

    // Get current working directory and split into components
    std::vector<std::string> Out;
    std::string Path = std::filesystem::current_path().string();
    k::SplitString(Path, '/', Out, true);  // Split path by '/' delimiter

    // If no project name specified, try to infer from current directory
    if(ProjectName == "") {
        Project Project;  // Initialize Project object
        // Check if current directory matches any known project
        for(std::string P: Project.List()) {
            if(P == Out.back()) {
                ProjectName = Out.back();
                break;
            }
        }
    }

    int CurrentTag = -1;  // Default tag value (will be set by window manager)
    
    // Handle case where only project name is specified
    if(ProjectName != "" && FileName == "") {
        try {
            // Special 'list' command to show available projects
            if(ProjectName == "list") {
                Project Project;
                k::VPrint(Project.List());  // Print project list
                return EXIT_SUCCESS;
            }
            // Initialize project and screen objects
            Project Project(ProjectName);  // Load specified project
            S::Screen Screen;              // Create screen management object
            Screen.Ranger(Project.Directory());  // Launch ranger in project dir
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
    // Handle case where both project and file name are specified
    } else if(ProjectName != "" && FileName != "") {
        try {
            // Special 'list' command to show files in project
            if(FileName == "list") {
                Project Project(ProjectName);
                k::VPrint(Project.List(Project.Directory()));  // Print file list
                return EXIT_SUCCESS;
            }
            // Initialize project with specific file
            Project Project(ProjectName, FileName);  // Load project+file combo
            S::Screen Screen;
            // Add all project files to screen management
            for(std::string File: Project.Files()) {
                Screen.Add(File);  // Register each file with screen manager
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

    // Handle destructive flag to terminate parent process
    if(DestroyFlag)
        k::ExecCmd("kill -4 " + std::to_string(getppid()));  // SIGILL to parent

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
