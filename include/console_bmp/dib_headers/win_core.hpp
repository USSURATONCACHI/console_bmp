#pragma once

#include <cstdint>
#include <cstddef>
#include <typeinfo>
#include <console_bmp/dib_headers/header_base.hpp>

namespace console_bmp {
namespace dib_headers {


// BITMAPCOREHEADER
struct Win_Core : public HeaderBase {
    // size of this header, 4 bytes
    int16_t width_pixels;
    int16_t height_pixels;
    int16_t num_color_planes; // must be 1 ?
    int16_t num_bits_per_pixel;

    virtual const std::type_info& type() const { return typeid(Win_Core); }
    virtual size_t in_file_size() const        { return 12; }
};


} // namespace dib_headers
} // namespace console_bmp