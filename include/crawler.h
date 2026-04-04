#ifndef CRAWLER_H
#define CRAWLER_H

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>

namespace fs = std::filesystem;

// ANSI color codes
namespace color {
    const std::string reset = "\033[0m";
    const std::string line = "\033[90m";       // gray for tree lines
    const std::string dir = "\033[36m\033[3m"; // cyan + italics for directories
    const std::string filename = "\033[38;5;116m"; // file names nord can
    
    // Extended color palette for cycling
    const std::vector<std::string> cyclingColors = {
        "\033[38;5;196m", // Fire Red
        "\033[38;5;46m",  // GH Green
        "\033[38;5;140m",  // NORD 3 
        "\033[38;5;220m", // Premium Gold
        "\033[38;5;201m", // Bright Magenta
        "\033[38;5;51m",  // Bright Cyan
        "\033[38;5;208m", // Orange
        "\033[38;5;129m", // Purple
        "\033[38;5;85m",  // Mint
        "\033[38;5;213m", // Pink
        "\033[38;5;39m",  // Sky Blue
        "\033[38;5;202m", // Dark Orange
        "\033[38;5;105m", // Light Purple
        "\033[38;5;49m",  // Teal
        "\033[38;5;165m", // Magenta
        "\033[38;5;214m", // Light Orange
        "\033[38;5;141m", // Lavender
        "\033[38;5;50m",  // Cyan
        "\033[38;5;203m", // Red-Orange
        "\033[38;5;47m"   // Light Green
    };
}

class DirectoryTreePrinter {
private:
    bool useColorCycling = false;
    std::map<std::string, std::string> branchColors; // Cache colors by root branch path
    size_t currentColorIndex = 0;
    
    // Helper function to check if a directory entry should be ignored
    bool shouldIgnore(const fs::path& path);
    
    // Get next color from the cycling palette
    std::string getNextColor();
    
    // Get color for a specific branch (cached by the topmost directory of that branch)
    std::string getBranchColor(const fs::path& path, const fs::path& rootBranch);
    
    // Reset color cycling state
    void resetColorCycling();
    
    // Function to print the tree structure
    void printTree(const fs::path& path, const std::string& prefix, bool isLast);
    
    // Print tree with color cycling (each branch gets its own color)
    void printTreeWithCycling(const fs::path& path, const std::string& prefix, bool isLast, const fs::path& branchRoot);

public:
    void printStructure(const fs::path& startPath = fs::current_path());
    void printStructureWithColorCycling(const fs::path& startPath = fs::current_path());
    void setColorCycling(bool enabled) { useColorCycling = enabled; }
};

#endif

