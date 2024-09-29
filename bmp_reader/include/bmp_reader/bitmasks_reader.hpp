#pragma once

#include <istream>

#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/color_bitmasks.hpp>

namespace bmp_reader {


struct BitmasksReader {
public:
    BitmasksReader(const dib_headers::WinInfo& header);
    virtual ~BitmasksReader() {}

    virtual auto read_bitmasks(std::istream& is) -> ColorBitmasks;

    virtual auto read_bgr_bitmasks(std::istream& is) -> ColorBitmasks;
    virtual auto read_bgra_bitmasks(std::istream& is) -> ColorBitmasks;
    virtual auto get_default_bitmasks() -> ColorBitmasks;

private:
    const dib_headers::WinInfo& m_header;
};


} // namespace bmp_reader