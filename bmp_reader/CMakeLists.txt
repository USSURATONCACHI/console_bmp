add_library(bmp_reader STATIC
    src/bmp_reader.cpp
    src/bmp_file_info.cpp

    src/util/bit_view.cpp
    src/util/bitshift.cpp
    
    src/readers/win_info_reader.cpp

    src/dib_headers/os21x_parser.cpp
    src/dib_headers/os22x_parser.cpp
    src/dib_headers/win_core_parser.cpp
    src/dib_headers/win_info_parser.cpp
    src/dib_headers/win_info.cpp
)
target_include_directories(bmp_reader PUBLIC "include/")