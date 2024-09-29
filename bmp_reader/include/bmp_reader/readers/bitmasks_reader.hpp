#pragma once

#include <istream>

#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/color_bitmasks.hpp>

namespace bmp_reader {
namespace readers {


struct BitmasksReader {
public:
    BitmasksReader(const dib_headers::WinInfo& header);
    virtual ~BitmasksReader() {}

    virtual auto read_bitmasks(std::istream& is) -> ColorBitmasks;

    static auto read_bgr_bitmasks(std::istream& is) -> ColorBitmasks;
    static auto read_bgra_bitmasks(std::istream& is) -> ColorBitmasks;
    static auto get_default_bitmasks(size_t bits_per_channel) -> ColorBitmasks;

private:
    const dib_headers::WinInfo& m_header;
};


} // namespace readers
} // namespace bmp_reader