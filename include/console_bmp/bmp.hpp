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
    virtual ~Bmp();

    virtual size_t width() const;
    virtual size_t height() const;

    virtual size_t num_color_channels() const;
    virtual size_t bits_per_channel() const;

    virtual void get_pixel(size_t x, size_t y, void* out_buffer) const;
    virtual void set_pixel(size_t x, size_t y, void* in_buffer) const;

    virtual Rgb8Pixel get_pixel_rgb8(size_t x, size_t y);
    virtual void set_pixel_rgb8(size_t x, size_t y, Rgb8Pixel pixel);
};

}; // namespace console_bmp