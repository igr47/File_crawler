#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>

namespace fs = std::filesystem;

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
        std::cout << prefix;

        if (isLast) {
            std::cout << "└─── ";
        } else {
            std::cout << "├─── ";
        }

        std::cout << path.filename().string();

        if (fs::is_directory(path)) {
            std::cout << "-|" << std::endl;
        } else {
            std::cout << std::endl;
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

        // Print the root directory name
        std::cout << startPath.filename().string() << "-|" << std::endl;

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
