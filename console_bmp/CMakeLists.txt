add_executable(console_bmp
    src/main.cpp
    src/cli_args.cpp
    src/bmp_display.cpp
    src/bmp_display_sfml.cpp
    src/get_terminal_size.cpp
)
target_include_directories(console_bmp PUBLIC include)
target_link_libraries(console_bmp PUBLIC bmp_reader argparse)

if (SFML_WINDOW_DISPLAY)
    target_include_directories(console_bmp PRIVATE ${SFML_INCLUDE_DIRECTORIES})
    target_link_libraries(console_bmp PUBLIC sfml-graphics)
    add_definitions(-DCONSOLE_BMP_SFML_WINDOW)
endif (SFML_WINDOW_DISPLAY)