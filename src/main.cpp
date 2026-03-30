#include "crawler.h"
#include <iostream>
#include <filesystem>

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

