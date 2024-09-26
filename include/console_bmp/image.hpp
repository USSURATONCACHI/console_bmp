#pragma once

#include <cstddef>
#include <cstdint>

namespace console_bmp {

struct RgbaAnyPixel {
    uint64_t r, g, b, a;
};

// Represents an abstract image with an origin at the top-left
class Image {
public:
    virtual ~Image() {};

    virtual size_t width() const  = 0;
    virtual size_t height() const  = 0;

    virtual size_t num_color_channels() const = 0;
    virtual size_t bits_per_channel() const = 0;

    virtual RgbaAnyPixel get_pixel_rgba(size_t x, size_t y) = 0;
    virtual void set_pixel_rgba(size_t x, size_t y, RgbaAnyPixel rgba) = 0;
};

}; // namespace console_bmp