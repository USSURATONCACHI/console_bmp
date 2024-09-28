add_library(bmp_reader STATIC
    src/bmp_reader.cpp
    src/bmp_file_info.cpp
    src/bit_view.cpp
    
    src/readers/win_info_reader.cpp

    src/dib_headers/os21x.cpp
    src/dib_headers/os22x.cpp
    src/dib_headers/win_core.cpp
    src/dib_headers/win_info.cpp
)
target_include_directories(bmp_reader PUBLIC "include/")