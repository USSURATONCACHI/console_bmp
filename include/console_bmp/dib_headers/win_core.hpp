#pragma once

#include <cstdint>
#include <cstddef>
#include <typeinfo>
#include <memory>
#include <istream>

#include <console_bmp/bmp_file_info.hpp>
#include <console_bmp/dib_headers/header_base.hpp>
#include <console_bmp/dib_headers/header_parser.hpp>

namespace console_bmp {
namespace dib_headers {


// BITMAPCOREHEADER
struct WinCore : public HeaderBase {
    // size of this header, 4 bytes
    int16_t width_pixels;
    int16_t height_pixels;
    uint16_t num_color_planes; // must be 1 ?
    uint16_t num_bits_per_pixel;

    virtual const std::type_info& type() const { return typeid(WinCore); }

    static const size_t IN_FILE_SIZE = 12;
    virtual size_t in_file_size() const { return IN_FILE_SIZE; }
    virtual size_t bitmasks_count() const { return 0; }
    virtual size_t bits_per_pixel() const { return static_cast<size_t>(num_bits_per_pixel); }
    virtual size_t palette_size() const   { return 1 << bits_per_pixel(); }

    virtual size_t palette_num_channels() const     { return 4; }
    virtual size_t palette_bits_per_channel() const { return 8; }
    virtual ssize_t image_width() const  { return static_cast<ssize_t>(width_pixels); }
    virtual ssize_t image_height() const { return static_cast<ssize_t>(height_pixels); }
};

struct WinCoreParser : public HeaderParser {
    WinCoreParser();
    virtual ~WinCoreParser();
    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase>;
};

} // namespace dib_headers
} // namespace console_bmp