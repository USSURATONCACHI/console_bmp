add_library(bmp_reader STATIC
    src/dib_headers/os21x_parser.cpp
    src/dib_headers/os22x_parser.cpp
    src/dib_headers/win_core_parser.cpp
    src/dib_headers/win_info_parser.cpp
    src/dib_headers/win_info.cpp

    src/pixel_reader/pixel_bitmasked_reader.cpp
    src/pixel_reader/pixel_paletted_reader.cpp
    src/pixel_reader/pixel_reader.cpp

    src/readers/win_info_reader.cpp

    src/util/bit_view.cpp
    src/util/bitshift.cpp
    
    src/bitmasks_reader.cpp
    src/bmp_file_info.cpp
    src/bmp_reader.cpp
    src/palette_reader.cpp

)
target_include_directories(bmp_reader PUBLIC "include/")