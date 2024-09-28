#pragma once

#include <istream>
#include <console_bmp/dib_headers/win_info.hpp>
#include <console_bmp/images/rgba8.hpp>

namespace console_bmp {
namespace readers {

class WinInfoReader {
public:
    WinInfoReader();
    virtual auto read(std::istream& is, dib_headers::WinInfo header, BmpFileInfo info) -> images::Rgba8;
};

} // namespace readers
} // namespace console_bmp