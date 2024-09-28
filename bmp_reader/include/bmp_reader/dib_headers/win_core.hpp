#pragma once

#include <cstdint>
#include <cstddef>
#include <typeinfo>

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_base.hpp>
#include <bmp_reader/dib_headers/header_parser.hpp>

namespace bmp_reader {
namespace dib_headers {


// BITMAPCOREHEADER
struct WinCore : public HeaderBase {
    int16_t width_pixels;
    int16_t height_pixels;
    uint16_t num_color_planes; // must be 1 ?
    uint16_t num_bits_per_pixel;

    virtual const std::type_info& type() const { return typeid(WinCore); }
};


} // namespace dib_headers
} // namespace bmp_reader