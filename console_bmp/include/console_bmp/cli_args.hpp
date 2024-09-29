#pragma once

#include <cstddef>
#include <filesystem>

namespace console_bmp {


struct AppArguments {
    bool info;
    bool gradient;
    bool no_raw_text;
    bool no_auto_size;
    bool out_window;
    
    size_t width;
    size_t height;

    std::filesystem::path bmp_file_path;
};

auto parse_args(int argc, char** argv) -> AppArguments;


} // namespace console_bmp