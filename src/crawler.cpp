#include "crawler.h"

bool DirectoryTreePrinter::shouldIgnore(const fs::path& path) {
    std::string filename = path.filename().string();
    // Ignore hidden files/directories and common version control directories
    return (filename.front() == '.' ||
        filename == "node_modules" ||
        filename == "__pycache__" ||
        filename == "build" ||
        filename == "Debug" ||
        filename == "Release");
}

std::string DirectoryTreePrinter::getNextColor() {
    std::string colorCode = color::cyclingColors[currentColorIndex % color::cyclingColors.size()];
    currentColorIndex++;
    return colorCode;
}

std::string DirectoryTreePrinter::getBranchColor(const fs::path& path, const fs::path& branchRoot) {
    std::string branchKey = branchRoot.string();
    auto it = branchColors.find(branchKey);
    if (it != branchColors.end()) {
        return it->second;
    }
    
    // Assign new color to this branch
    std::string newColor = getNextColor();
    branchColors[branchKey] = newColor;
    return newColor;
}

void DirectoryTreePrinter::resetColorCycling() {
    branchColors.clear();
    currentColorIndex = 0;
}

void DirectoryTreePrinter::printTree(const fs::path& path, const std::string& prefix, bool isLast) {
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

void DirectoryTreePrinter::printTreeWithCycling(const fs::path& path, const std::string& prefix, bool isLast, const fs::path& branchRoot) {
    // Get the color for this branch (files and subdirectories inherit from the root of this branch)
    std::string branchColor = getBranchColor(path, branchRoot);
    
    // Print prefix with line color
    std::cout << color::line << prefix << color::reset;

    if (isLast) {
        std::cout << color::line << "└─── " << color::reset;
    } else {
        std::cout << color::line << "├─── " << color::reset;
    }

    // Print name with the branch color
    if (fs::is_directory(path)) {
        // Directories: colored + italic
        std::cout << branchColor << "\033[3m" << path.filename().string() << color::reset;
        std::cout << "-|" << std::endl;
        
        // For subdirectories, they become new branch roots
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
            // For subdirectories, they become the new branch root
            // For files, they inherit the current branch's color
            if (fs::is_directory(entries[i])) {
                // Subdirectory starts a NEW branch with its own color
                printTreeWithCycling(entries[i], newPrefix, isLastEntry, entries[i]);
            } else {
                // File inherits the color from the current branch
                printTreeWithCycling(entries[i], newPrefix, isLastEntry, branchRoot);
            }
        }
    } else {
        // Files: colored with the branch color (no italics)
        std::cout << branchColor << path.filename().string() << color::reset << std::endl;
    }
}

void DirectoryTreePrinter::printStructure(const fs::path& startPath) {
    if (!fs::exists(startPath)) {
        std::cerr << "Error: Path does not exist!" << std::endl;
        return;
    }

    if (!fs::is_directory(startPath)) {
        std::cerr << "Error: Path is not a directory!" << std::endl;
        return;
    }

    if (useColorCycling) {
        // For color cycling mode, start with the root directory as its own branch
        std::string rootColor = getBranchColor(startPath, startPath);
        std::cout << rootColor << "\033[3m" << startPath.filename().string() << color::reset << "-|" << std::endl;
        
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
            if (fs::is_directory(entries[i])) {
                // Subdirectory starts a NEW branch with its own color
                printTreeWithCycling(entries[i], "", isLastEntry, entries[i]);
            } else {
                // Root-level files inherit the root's color
                printTreeWithCycling(entries[i], "", isLastEntry, startPath);
            }
        }
    } else {
        // Original behavior without color cycling
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
}

void DirectoryTreePrinter::printStructureWithColorCycling(const fs::path& startPath) {
    resetColorCycling();
    useColorCycling = true;
    printStructure(startPath);
    useColorCycling = false;
}
