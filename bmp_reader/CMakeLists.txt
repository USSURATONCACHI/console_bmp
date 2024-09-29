add_library(bmp_reader STATIC
    src/dib_headers/os21x_parser.cpp
    src/dib_headers/os21x.cpp
    src/dib_headers/os22x_parser.cpp
    src/dib_headers/win_core_parser.cpp
    src/dib_headers/win_core.cpp
    src/dib_headers/win_info_parser.cpp
    src/dib_headers/win_info.cpp

    src/readers/bitmasks_reader.cpp
    src/readers/bmp_reader_base.cpp
    src/readers/bmp_win_core_reader.cpp
    src/readers/bmp_win_info_reader.cpp
    src/readers/palette_reader.cpp
    src/readers/pixel_bitmasked_reader.cpp
    src/readers/pixel_paletted_reader.cpp
    src/readers/pixel_reader_base.cpp

    src/util/bit_view.cpp
    src/util/bitshift.cpp
    
    src/bmp_file_info.cpp
    src/bmp_reader.cpp

)
target_include_directories(bmp_reader PUBLIC "include/")