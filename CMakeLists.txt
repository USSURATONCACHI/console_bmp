cmake_minimum_required(VERSION 3.5)
project(console_bmp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For VSCode clang extension integraion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


add_executable(drawBmp
    src/main.cpp
    src/bmp_reader.cpp
    src/bmp_file_info.cpp

    src/dib_headers/os21x.cpp
    src/dib_headers/os22x.cpp
    src/dib_headers/win_core.cpp
    src/dib_headers/win_info.cpp
)
target_include_directories(drawBmp PUBLIC "include/")