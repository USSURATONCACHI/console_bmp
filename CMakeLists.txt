cmake_minimum_required(VERSION 3.5)
project(console_bmp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For VSCode clang extension integraion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ggdb")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



add_library(console_bmp STATIC
    src/console_bmp/bmp_reader.cpp
    src/console_bmp/bmp_file_info.cpp
    src/console_bmp/bit_view.cpp
    
    src/console_bmp/readers/win_info_reader.cpp

    src/console_bmp/dib_headers/os21x.cpp
    src/console_bmp/dib_headers/os22x.cpp
    src/console_bmp/dib_headers/win_core.cpp
    src/console_bmp/dib_headers/win_info.cpp
)
target_include_directories(console_bmp PUBLIC "include/")


add_executable(drawBmp
    src/main.cpp
)
target_include_directories(drawBmp PUBLIC "include/")
target_link_libraries(drawBmp PUBLIC console_bmp)


enable_testing()

add_executable(run_test
    src/test/bit_view.cpp
)
target_link_libraries(run_test GTest::gtest_main console_bmp)

include(GoogleTest)
gtest_discover_tests(run_test)
