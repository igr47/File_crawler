#ifndef CRAWLER_H
#define CRAWLER_H

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

// ANSI color codes
namespace color {
    const std::string reset = "\033[0m";
    const std::string line = "\033[90m";       // gray for tree lines
    const std::string dir = "\033[36m\033[3m"; // cyan + italics for directories
    const std::string filename = "\033[38;5;116m"; // file names nord can
}

class DirectoryTreePrinter {
private:
    // Helper function to check if a directory entry should be ignored
    bool shouldIgnore(const fs::path& path);

    // Function to print the tree structure
    void printTree(const fs::path& path, const std::string& prefix, bool isLast);

public:
    void printStructure(const fs::path& startPath = fs::current_path());

};

#endif



