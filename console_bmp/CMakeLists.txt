
add_executable(console_bmp
    src/main.cpp
)
target_include_directories(console_bmp PUBLIC include)
target_link_libraries(console_bmp PUBLIC bmp_reader argparse)