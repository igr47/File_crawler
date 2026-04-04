#include "crawler.h"
#include <iostream>
#include <filesystem>
#include <cstring>

bool hasFlag(int argc, char* argv[], const char* flag) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], flag) == 0) {
            return true;
        }
    }
    return false;
}

std::string getPathFromArgs(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        // If argument doesn't start with '-', it's a path
        if (argv[i][0] != '-') {
            return argv[i];
        }
    }
    return ""; // No path specified
}

int main(int argc, char* argv[]) {
    DirectoryTreePrinter printer;
    
    // Check for -cc flag
    bool useColorCycling = hasFlag(argc, argv, "-cc");
    
    // Get the path (skip flags)
    std::string pathStr = getPathFromArgs(argc, argv);
    fs::path startPath;
    
    if (!pathStr.empty()) {
        startPath = pathStr;
    } else {
        startPath = fs::current_path();
    }

    try {
        if (useColorCycling) {
            std::cerr << "Using color cycling mode (each directory branch gets a unique color)" << std::endl;
            printer.printStructureWithColorCycling(startPath);
        } else {
            printer.printStructure(startPath);
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
