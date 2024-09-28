#pragma once

#include <istream>
#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/images/rgba8.hpp>

namespace bmp_reader {
namespace readers {

class WinInfoReader {
public:
    WinInfoReader();
    virtual auto read(std::istream& is, dib_headers::WinInfo header, BmpFileInfo info) -> images::Rgba8;
};

} // namespace readers
} // namespace bmp_reader