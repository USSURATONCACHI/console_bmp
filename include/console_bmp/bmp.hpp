#pragma once

#include <cstddef>
#include <cstdint>

namespace console_bmp {

struct Rgb8Pixel {
    uint8_t r, g, b;
};

// Represents an abstract image with an origin at the top-left
class Bmp {
public:
    virtual ~Bmp() = 0;

    virtual size_t width() const  = 0;
    virtual size_t height() const  = 0;

    virtual size_t num_color_channels() const = 0;
    virtual size_t bits_per_channel() const = 0;

    virtual void get_pixel(size_t x, size_t y, void* out_buffer) const = 0;
    virtual void set_pixel(size_t x, size_t y, const void* in_buffer) = 0;

    virtual Rgb8Pixel get_pixel_rgb8(size_t x, size_t y) const = 0;
    virtual void set_pixel_rgb8(size_t x, size_t y, Rgb8Pixel pixel) = 0;
};

}; // namespace console_bmp