#include "console_bmp/bmp.hpp"
#include <console_bmp/bmp_rgb8.hpp>
#include <cstdint>
#include <stdexcept>

namespace console_bmp {


ImageRgb8::ImageRgb8(std::vector<uint8_t>&& data, size_t width, size_t height)
    : image_data_rgb8(std::move(data)),
    image_width(width)
{
    if (width * height * 3 != data.size()) {
        throw std::runtime_error("Incorrect data array provided for given image size.");
    }
}

ImageRgb8::~ImageRgb8() {}

size_t ImageRgb8::width() const {
    return image_width;
}
size_t ImageRgb8::height() const {
    return image_data_rgb8.size() / image_width;
}

size_t ImageRgb8::num_color_channels() const {
    return 3;
}
size_t ImageRgb8::bits_per_channel() const {
    return 8;
}

void ImageRgb8::get_pixel(size_t x, size_t y, void* out_buffer) const {
    uint8_t* write_ptr = reinterpret_cast<uint8_t*>(out_buffer);
    Rgb8Pixel pixel = get_pixel_rgb8(x, y);
    write_ptr[0] = pixel.b;
    write_ptr[1] = pixel.g;
    write_ptr[2] = pixel.r;

}
void ImageRgb8::set_pixel(size_t x, size_t y, const void* in_buffer) {
    const uint8_t* read_ptr = reinterpret_cast<const uint8_t*>(in_buffer);
    Rgb8Pixel pixel {
        .r = read_ptr[2],
        .g = read_ptr[1],
        .b = read_ptr[0],
    };
    set_pixel_rgb8(x, y, pixel);
}

Rgb8Pixel ImageRgb8::get_pixel_rgb8(size_t x, size_t y) const {
    size_t pos = (y * image_width + x) * 3;
    return Rgb8Pixel {
        .r = image_data_rgb8[pos + 0],
        .g = image_data_rgb8[pos + 1],
        .b = image_data_rgb8[pos + 2],
    };
}
void ImageRgb8::set_pixel_rgb8(size_t x, size_t y, Rgb8Pixel pixel) {
    size_t pos = (y * image_width + x) * 3;
    image_data_rgb8[pos + 0] = pixel.r;
    image_data_rgb8[pos + 1] = pixel.g;
    image_data_rgb8[pos + 2] = pixel.b;
}

} // namespace console_bmp
