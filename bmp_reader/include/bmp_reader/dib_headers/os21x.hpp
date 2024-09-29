#pragma once

#include <cstdint>
#include <typeinfo>

#include <bmp_reader/dib_headers/header_base.hpp>

namespace bmp_reader {
namespace dib_headers {

// OS21XBITMAPHEADER
struct OS21X : public HeaderBase {
    // size of this header, 4 bytes
    uint16_t width_pixels;
    uint16_t height_pixels;
    uint16_t num_color_planes; // must be 1 ?
    uint16_t num_bits_per_pixel;

    virtual const std::type_info& type() const { return typeid(OS21X); }
    virtual auto data_row_size() const -> size_t;
    virtual auto min_data_size() const -> size_t;
    virtual auto get_bits_per_pixel() const -> size_t;
    virtual auto channels_count() const -> size_t;
    virtual void print_info() const;
};

} // namespace dib_headers
} // namespace bmp_reader