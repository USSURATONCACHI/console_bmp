#pragma once

#include <cstdint>
#include <cstddef>
#include <typeinfo>
#include <memory>
#include <istream>

#include <console_bmp/dib_headers/header_base.hpp>
#include <console_bmp/dib_headers/header_parser.hpp>

namespace console_bmp {
namespace dib_headers {

// OS21XBITMAPHEADER
struct OS21X : public HeaderBase {
    // size of this header, 4 bytes
    uint16_t width_pixels;
    uint16_t height_pixels;
    uint16_t num_color_planes; // must be 1 ?
    uint16_t num_bits_per_pixel;

    virtual const std::type_info& type() const { return typeid(OS21X); }

    static const size_t IN_FILE_SIZE = 12;
    virtual size_t in_file_size() const        { return IN_FILE_SIZE; }
};

struct OS21X_Parser : public HeaderParser {
    OS21X_Parser();
    virtual ~OS21X_Parser();
    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase>;
};

} // namespace dib_headers
} // namespace console_bmp