# FileCrawler 🌲

A beautiful, colorized directory tree visualizer for the command line. FileCrawler displays directory structures with ANSI colors, making it easy to understand your file hierarchy at a glance.

## Features ✨

- **Tree Visualization**: Displays directory structure in a familiar tree format
- **Color Coding**:
  - Directories appear in cyan with italics
  - Files appear in a soft nord blue
  - Tree lines in subtle gray
- **Color Cycling Mode** (`-cc` flag): Each directory branch gets its own unique color that propagates to all its files
- **Smart Sorting**: Directories appear before files, both sorted alphabetically
- **Automatic Ignoring**: Skips hidden files/directories (starting with `.`) and common build directories (`node_modules`, `__pycache__`, `build`, `Debug`, `Release`)
- **Cross-Platform**: Works on Linux, macOS, and Windows (with proper terminal support)

## Installation & Compilation 🔧

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.10 or higher
- Terminal with ANSI color support

### Build Instructions

#### 1. Clone or download the source code

```bash
# If using git
git clone <your-repo-url>
cd filecrawler

# Or just place all source files in a directory:
# - CMakeLists.txt
# - src/crawler.cpp
# - src/main.cpp
# - include/crawler.h
```
#### 2. Create build directory and compile

```bash
# Create a build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build the project
cmake --build .

# On Unix-like systems (Linux/macOS), you can also use:
make
```

### Usage 🚀
### Basic Usage

```bash
# Display current directory structure
./file_crawler

# Display structure of a specific path
./file_crawler /path/to/directory

# Display structure with color cycling mode
./file_crawler -cc

# Display structure of specific path with color cycling
./file_crawler /path/to/directory -cc
# or
./file_crawler -cc /path/to/directory

```

### Command Line Options
--------------------------------------------------------------------------------------
|Option  |	Description                                                               |
--------------------------------------------------------------------------------------
|-cc	 |  Enable color cycling mode - each directory branch gets a unique color     |
|no flag |	Normal mode - consistent colors (cyan for dirs, blue for files)           |
---------------------------------------------------------------------------------------

### Examples
### Normal Mode Output:

Documents-|
├─── projects-|
│   ├─── include-|
│   │   ├─── utils.h
│   │   └─── math.h
│   ├─── src-|
│   │   ├─── main.cpp
│   │   └─── utils.cpp
│   └─── README.md
└─── photos-|
    ├─── vacation.jpg
    └─── family.png

### Color Cycling Mode Output (conceptual):

Documents-|                    (Color 1: Red)
├─── projects-|                (Color 2: Green)
│   ├─── include-|             (Color 3: Blue)
│   │   ├─── utils.h           (Inherits Blue)
│   │   └─── math.h            (Inherits Blue)
│   ├─── src-|                 (Color 4: Purple)
│   │   ├─── main.cpp          (Inherits Purple)
│   │   └─── utils.cpp         (Inherits Purple)
│   └─── README.md             (Inherits Green)
└─── photos-|                  (Color 5: Orange)
    ├─── vacation.jpg          (Inherits Orange)
    └─── family.png            (inherits Orange)

### What to Expect 📋
### Normal Mode
- Directories: Displayed in cyan with italics followed by -|

- Files: Displayed in soft blue (nord theme)

- Tree lines: Displayed in gray

- Consistent coloring throughout the entire tree

### Color Cycling Mode (-cc)
- Each directory gets a unique color from a 20-color palette

- Files inherit the color of their parent directory

- Subdirectories get their own new colors (different from parent)

- Colors cycle through the palette (reuses colors after 20 unique ones)

- Perfect for visualizing branch ownership and file organization

### Ignored Items
The following are automatically excluded from display:

- Hidden files/directories (starting with .)

    node_modules/

    __pycache__/

    build/

    Debug/

    Release/

### Color Palette 🎨
The color cycling mode uses these 20 vibrant colors (in order):

    Fire Red

    Grass Green

    Sapphire Blue

    Premium Gold

    Bright Magenta

    Bright Cyan

    Orange

    Purple

    Mint

    Pink

    Sky Blue

    Dark Orange

    Light Purple

    Teal

    Magenta

    Light Orange

    Lavender

    Cyan

    Red-Orange

    Light Green

### Troubleshooting 🔍
- Colors not showing correctly
- Issue: No colors or weird characters appear

### Solutions:

Ensure your terminal supports ANSI colors (most modern terminals do)

Try compiling with -DDISABLE_COLORS=ON to disable colors

On Windows, ensure you're using Windows Terminal, PowerShell, or an updated Command Prompt

Compilation errors on older GCC
Issue: std::filesystem not found

Solution: The CMake file automatically handles this, but if you're using GCC 7 or 8, ensure you have the experimental filesystem:

bash
# The CMake file will automatically link stdc++fs for older GCC versions
Permission denied
Issue: Cannot access certain directories

Solution: FileCrawler will show a filesystem error but continue. Run with appropriate permissions if you need to view protected directories.

### Development 🛠️
### Project Structure

filecrawler/
├── CMakeLists.txt          # CMake build configuration
├── include/
│   └── crawler.h          # Header file with class definitions
├── src/
│   ├── crawler.cpp        # DirectoryTreePrinter implementation
│   └── main.cpp           # Main entry point and argument parsing
└── README.md              # This file
Building for Development
bash
# Debug build with symbols
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_DEBUG_OUTPUT=ON ..
make

# Run with debug output
./bin/file_crawler -cc
Running Tests
bash
# Configure with tests enabled
cmake -DBUILD_TESTS=ON ..
make

# Run tests
ctest
```

### License 📄
This project is open source. Feel free to use, modify, and distribute as needed.

### Contributing 🤝
- Contributions are welcome! Areas for potential improvement:

- Add more ignore patterns

- Implement configuration file for custom ignore lists

- Add support for showing file sizes or modification dates

- Create export functionality (to file, JSON, etc.)

- Add more color schemes

### Acknowledgments 🙏
Color palette inspired by various terminal themes (Nord, Solarized, GitHub Dark)

Tree drawing algorithm based on classic Unix tree command

ANSI color codes for beautiful terminal output
