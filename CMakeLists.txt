cmake_minimum_required(VERSION 3.5)
project(console_bmp VERSION 0.1.0 LANGUAGES C CXX)

option(SFML_WINDOW_DISPLAY "Implement `--window` flag for CLI to show image in window" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For VSCode clang extension integraion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ggdb")

if (SFML_WINDOW_DISPLAY)
    find_package(SFML COMPONENTS graphics REQUIRED)
endif(SFML_WINDOW_DISPLAY)

add_subdirectory(third_party/argparse)
add_subdirectory(third_party/googletest)

add_subdirectory(bmp_reader)
add_subdirectory(console_bmp)
add_subdirectory(tests)
