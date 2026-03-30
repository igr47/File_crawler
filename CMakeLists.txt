cmake_minimum_required(VERSION 3.10)

# Project name and version
project(FileCrawler VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(ENABLE_DEBUG_OUTPUT "Enable debug output" OFF)
option(DISABLE_COLORS "Disable ANSI colors" OFF)

# Compiler-specific settings
if(MSVC)
    # Windows Visual Studio
    add_compile_options(/W4)
    # Enable ANSI colors on Windows
    add_compile_definitions(ENABLE_VIRTUAL_TERMINAL_PROCESSING)
else()
    # GCC/Clang
    add_compile_options(-Wall -Wextra -Wpedantic)

    # Some older GCC versions need explicit linking with stdc++fs
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lstdc++fs")
    endif()
endif()

# Add debug flags if enabled
if(ENABLE_DEBUG_OUTPUT)
    add_compile_definitions(DEBUG_OUTPUT)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-g -O0)
    endif()
endif()

# Disable colors if requested
if(DISABLE_COLORS)
    add_compile_definitions(DISABLE_ANSI_COLORS)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/crawler.cpp
    src/main.cpp
)

# Define executable
add_executable(file_crawler ${SOURCES})

# Link libraries
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
    target_link_libraries(file_crawler stdc++fs)
endif()

# Set properties
set_target_properties(file_crawler PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "file_crawler"
)

# Install rules
install(TARGETS file_crawler
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Optional: Add a simple test
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()

    # Create a test directory structure
    add_custom_command(TARGET file_crawler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/test_dir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/test_dir/subdir1
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/test_dir/subdir2
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/test_dir/file1.txt
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/test_dir/subdir1/file2.txt
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/test_dir/subdir1/nested_dir
        COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/test_dir/subdir1/nested_dir/file3.cpp
        COMMENT "Creating test directory structure"
    )

    # Add a test that runs the executable
    add_test(NAME file_crawler_test
        COMMAND file_crawler ${CMAKE_BINARY_DIR}/test_dir
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endif()

# Print configuration summary
message(STATUS "Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install Prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Debug Output: ${ENABLE_DEBUG_OUTPUT}")
message(STATUS "  Disable Colors: ${DISABLE_COLORS}")
message(STATUS "  Build Tests: ${BUILD_TESTS}")
