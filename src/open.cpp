#include "../inc/open.hpp"

int main(int argc, char** argv) { 
    // Command line flags initialization
    // -h/--help flag
    int HelpFlag = 0;
    // -v/--version flag
    int VersionFlag = 0;
    // -r/--ranger flag to launch ranger file manager
    int RangerFlag = 0;
    // -t/--tag-here to spawn in current tag
    int HereFlag = 0;
    // -n/--new to force a fresh tag even if the project is already open
    int NewFlag = 0;
    // -d/--destroy to kill parent process
    int DestroyFlag = 0;
    // Project name from -p/--project-name
    std::string ProjectName;
    // File name from -f/--file-name
    std::string FileName;
    // Claude alias from -c/--claude ('-cc' -> clc, '-cr' -> clr)
    std::string ClaudeAlias;
    // Variable for getopt parsing
    int opt;

    // Long option definitions for getopt:
    // {name, has_arg, flag, val}
    struct option Opts[] = {
        { "help", no_argument, &HelpFlag, 1 },
        { "version", no_argument, &VersionFlag, 1 },
        { "project-name", required_argument, NULL, 'p' },
        { "file-name", required_argument, NULL, 'f' },
        { "ranger", no_argument, &RangerFlag, 1 },
        { "claude", optional_argument, NULL, 'c' },
        { "tag-here", no_argument, &HereFlag, 1 },
        { "new", no_argument, &NewFlag, 1 },
        { "destory", no_argument, &DestroyFlag, 1 },
        { 0 }
    };

    // Command line option parsing loop
    while (1) {
        // Parse options using getopt_long:
        // - "hvp:f:rtdnc::" specifies short options (colon = requires argument,
        //   double colon = optional argument that must be attached, eg '-cc')
        // - Options structure defines long options mapping to flags
        opt = getopt_long(argc, argv, "hvp:f:rtdnc::", Opts, 0);

        // Exit loop when no more options (-1 return value)
        if (opt == -1) {
            if(HelpFlag && VersionFlag) {
                Usage();
                return EXIT_FAILURE;
            }
            break;
        } 

        switch (opt) {
        // Long options with a flag pointer (--new, --ranger, ...) return 0
        // having already set their flag, they are not an error
        case 0:
            break;
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
        case 'c':
            ClaudeAlias = "cl";
            if(optarg) {
                if(std::string(optarg) != "c" && std::string(optarg) != "r") {
                    Usage("Unknown claude alias: -c" + std::string(optarg));
                    return EXIT_FAILURE;
                }
                ClaudeAlias += optarg;
            }
            break;
        case 't':
            HereFlag = 1;
            break;
        case 'n':
            NewFlag = 1;
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
    // Split path by '/' delimiter
    k::SplitString(Path, '/', Out, true);

    // If no project name specified, try to infer from current directory
    if(ProjectName == "") {
        // Initialize Project object
        Project Project;
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
                // Print project list
                k::VPrint(Project.List());
                return EXIT_SUCCESS;
            }
            // Initialize project and screen objects
            Project Project(ProjectName);  // Load specified project
            S::Screen Screen;              // Create screen management object
            // Is the project already running on a tag? (-n and -t skip the look up)
            int ExistingTag = -1;
#ifndef TEST
            if(!NewFlag && !HereFlag)
                ExistingTag = Screen.FindProjectTag(Project.Directory());
#endif
            if(ExistingTag >= 0) {
                // Already open: go to it and add only what was asked for, so a
                // bare '-p' is just a tag switch
                if(ClaudeAlias != "")
                    Screen.Claude(Project.Directory(), ClaudeAlias);
                if(RangerFlag)
                    Screen.Ranger(Project.Directory());
                std::cout << ProjectName << ": already open on tag "
                          << TagName(ExistingTag) << std::endl;
#ifndef TEST
                Screen.Focus(ExistingTag);
                Screen.Spawn(CurrentTag);
#endif
            } else {
                if(ClaudeAlias != "")
                    Screen.Claude(Project.Directory(), ClaudeAlias);  // Terminal + claude instead
                else
                    Screen.Ranger(Project.Directory());  // Launch ranger in project dir
#ifndef TEST
                if(HereFlag)
                    Screen.Spawn(CurrentTag);
                else
                    Screen.Spawn();
#endif
            }
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
                // Print file list
                k::VPrint(Project.List(Project.Directory()));
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
            if(ClaudeAlias != "")
                Screen.Claude(Project.Directory(), ClaudeAlias);
            // The files were asked for explicitly, so they get opened either
            // way - the only question is which tag they land on
            int ExistingTag = -1;
#ifndef TEST
            if(!NewFlag && !HereFlag)
                ExistingTag = Screen.FindProjectTag(Project.Directory());
#endif
            if(ExistingTag >= 0)
                std::cout << ProjectName << ": already open on tag "
                          << TagName(ExistingTag) << std::endl;
#ifndef TEST
            if(ExistingTag >= 0) {
                Screen.Focus(ExistingTag);
                Screen.Spawn(CurrentTag);
            } else if(HereFlag)
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
    // No project (and none inferred from cwd): just open the devel dir
    } else if(ProjectName == "" && FileName == "") {
        try {
            S::Screen Screen;
            if(ClaudeAlias != "")
                Screen.Claude(DevelPath, ClaudeAlias);
            else
                Screen.Ranger(DevelPath);
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
        // SIGILL to parent
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

// dwm labels the first nine tags 1-9 and the rest A1-A9
std::string TagName(int Tag) {
    if(Tag < 9)
        return std::to_string(Tag + 1);
    return "A" + std::to_string(Tag - 8);
}

void PrintVersion() {
    std::cout << ProgramName << ": version " << Version << std::endl;
}

// Copyright (c) 2023, Maxamilian Kidd-May
// All rights reserved.

// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this source tree. 
