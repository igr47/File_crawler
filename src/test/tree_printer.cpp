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
    const std::string filename = "\033[38;5;116m"; // file names light teal
}

class DirectoryTreePrinter {
private:
    // Helper function to check if a directory entry should be ignored
    bool shouldIgnore(const fs::path& path) {
        std::string filename = path.filename().string();
        // Ignore hidden files/directories and common version control directories
        return (filename.front() == '.' ||
                filename == "node_modules" ||
                filename == "__pycache__" ||
                filename == "build" ||
                filename == "Debug" ||
                filename == "Release");
    }

    // Function to print the tree structure
    void printTree(const fs::path& path, const std::string& prefix, bool isLast) {
        // Print prefix with line color
        std::cout << color::line << prefix << color::reset;

        if (isLast) {
            std::cout << color::line << "└─── " << color::reset;
        } else {
            std::cout << color::line << "├─── " << color::reset;
        }

        // Print name: directory with color/italics, files without
        if (fs::is_directory(path)) {
            std::cout << color::dir << path.filename().string() << color::reset;
            std::cout << "-|" << std::endl;
        } else {
            std::cout << color::filename << path.filename().string() << color::reset << std::endl;
        }

        if (fs::is_directory(path)) {
            std::vector<fs::path> entries;

            // Collect all entries in the directory
            for (const auto& entry : fs::directory_iterator(path)) {
                if (!shouldIgnore(entry.path())) {
                    entries.push_back(entry.path());
                }
            }

            // Sort entries (directories first, then files)
            std::sort(entries.begin(), entries.end(),
                [](const fs::path& a, const fs::path& b) {
                    bool aIsDir = fs::is_directory(a);
                    bool bIsDir = fs::is_directory(b);
                    if (aIsDir != bIsDir) {
                        return aIsDir > bIsDir; // Directories first
                    }
                    return a.filename().string() < b.filename().string();
                });

            // Calculate new prefix
            std::string newPrefix = prefix;
            if (isLast) {
                newPrefix += "    ";
            } else {
                newPrefix += "│   ";
            }

            // Print each entry
            for (size_t i = 0; i < entries.size(); ++i) {
                bool isLastEntry = (i == entries.size() - 1);
                printTree(entries[i], newPrefix, isLastEntry);
            }
        }
    }

public:
    void printStructure(const fs::path& startPath = fs::current_path()) {
        if (!fs::exists(startPath)) {
            std::cerr << "Error: Path does not exist!" << std::endl;
            return;
        }

        if (!fs::is_directory(startPath)) {
            std::cerr << "Error: Path is not a directory!" << std::endl;
            return;
        }

        // Print the root directory name with color
        std::cout << color::dir << startPath.filename().string() << color::reset << "-|" << std::endl;

        std::vector<fs::path> entries;

        // Collect all entries in the root directory
        for (const auto& entry : fs::directory_iterator(startPath)) {
            if (!shouldIgnore(entry.path())) {
                entries.push_back(entry.path());
            }
        }

        // Sort entries (directories first, then files)
        std::sort(entries.begin(), entries.end(),
            [](const fs::path& a, const fs::path& b) {
                bool aIsDir = fs::is_directory(a);
                bool bIsDir = fs::is_directory(b);
                if (aIsDir != bIsDir) {
                    return aIsDir > bIsDir; // Directories first
                }
                return a.filename().string() < b.filename().string();
            });

        // Print each entry
        for (size_t i = 0; i < entries.size(); ++i) {
            bool isLastEntry = (i == entries.size() - 1);
            printTree(entries[i], "", isLastEntry);
        }
    }
};

int main(int argc, char* argv[]) {
    DirectoryTreePrinter printer;

    try {
        // If a path is provided as command line argument, use it
        if (argc > 1) {
            fs::path startPath = argv[1];
            printer.printStructure(startPath);
        } else {
            // Otherwise, use current working directory
            printer.printStructure();
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
