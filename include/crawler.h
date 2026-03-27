#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

class DirectoryTreePrinter {
    private:
        // Helper function to check if a directory entry should be ignored
        bool shouldIgnore(const fs::path& path);

        // Function to print the tree structure
        void printTree(const fs::path& path, const std::string& prefix, bool isLast);

        void printStructure(const fs::path& startpath = fs::currentpath());
}
