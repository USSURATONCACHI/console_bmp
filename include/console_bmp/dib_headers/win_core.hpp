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
};

struct WinCoreParser : public HeaderParser {
    WinCoreParser();
    virtual ~WinCoreParser();
    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase>;
};

} // namespace dib_headers
} // namespace console_bmp