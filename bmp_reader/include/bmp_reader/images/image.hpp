#pragma once

#include <cstddef>
#include <bmp_reader/images/pixels.hpp>

namespace bmp_reader {
namespace images {


class Image {
public:
    virtual ~Image() {};

    virtual size_t width() const  = 0;
    virtual size_t height() const  = 0;

    virtual size_t num_color_channels() const = 0;
    virtual size_t bits_per_channel() const = 0;

    virtual Rgba8Pixel get_rgba8_lossy(size_t x, size_t y) const = 0;
};

} //namespace images
} // namespace bmp_reader