cmake_minimum_required(VERSION 3.5)
project(console_bmp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # For VSCode clang extension integraion
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



add_library(bmp_parse
    src/bmp_parse/bmp_reader.cpp
    src/bmp_parse/bmp_file_info.cpp
    src/bmp_parse/palette_view.cpp
    src/bmp_parse/bit_view.cpp
    src/bmp_parse/dib_headers/os21x.cpp
    src/bmp_parse/dib_headers/os22x.cpp
    src/bmp_parse/dib_headers/win_core.cpp
    src/bmp_parse/dib_headers/win_info.cpp
)
target_include_directories(bmp_parse PUBLIC "include/")


add_executable(drawBmp
    src/main.cpp
)
target_include_directories(drawBmp PUBLIC "include/")
target_link_libraries(drawBmp PUBLIC bmp_parse)


enable_testing()

add_executable(run_test
    src/test/test.cpp
)
target_link_libraries(run_test GTest::gtest_main bmp_parse)

include(GoogleTest)
gtest_discover_tests(run_test)
