#pragma once

#include <console_bmp/bmp.hpp>

#include <vector>

namespace console_bmp {

class ImageRgb8 : public Bmp {
protected:
    std::vector<uint8_t> image_data_rgb8;
    size_t image_width;

public:
    ImageRgb8(std::vector<uint8_t>&& data, size_t width, size_t height);

    virtual ~ImageRgb8();

    virtual size_t width() const;
    virtual size_t height() const;

    virtual size_t num_color_channels() const;
    virtual size_t bits_per_channel() const;

    virtual void get_pixel(size_t x, size_t y, void* out_buffer) const;
    virtual void set_pixel(size_t x, size_t y, const void* in_buffer);

    virtual Rgb8Pixel get_pixel_rgb8(size_t x, size_t y) const;
    virtual void set_pixel_rgb8(size_t x, size_t y, Rgb8Pixel pixel);
};

} // namespace console_bmp