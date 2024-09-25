cmake_minimum_required(VERSION 3.0)
project(console_bmp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For VSCode clang extension integraion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


add_executable(drawBmp
    src/main.cpp
)
target_include_directories(drawBmp PUBLIC "include/")