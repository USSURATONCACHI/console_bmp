#pragma once

#include <istream>

#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/color_bitmasks.hpp>

namespace bmp_reader {
namespace readers {


struct BitmasksReader {
    BitmasksReader() {}
    virtual ~BitmasksReader() {}

    virtual auto read_bitmasks(std::istream& is, const dib_headers::WinInfo& header) -> ColorBitmasks;

    virtual auto read_bgr_bitmasks(std::istream& is) -> ColorBitmasks;
    virtual auto read_bgra_bitmasks(std::istream& is) -> ColorBitmasks;
    virtual auto get_default_bitmasks(const dib_headers::WinInfo& header) -> ColorBitmasks;
};


} // namespace readers
} // namespace bmp_reader