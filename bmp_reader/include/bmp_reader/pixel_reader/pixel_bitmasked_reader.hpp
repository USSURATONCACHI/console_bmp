#pragma once

#include <bmp_reader/pixel_reader/pixel_reader.hpp>
#include <bmp_reader/color_bitmasks.hpp>
#include <bmp_reader/images/pixels.hpp>


namespace bmp_reader {
namespace pixel_reader {


class PixelBitmaskedReader : public PixelReader {
public:
    PixelBitmaskedReader(ColorBitmasks masks, size_t bits_per_pixel, size_t channels_count);
    virtual ~PixelBitmaskedReader() {}

    virtual auto read_rgba(BitView bv) -> images::Rgba8Pixel;

private:
    ColorBitmasks m_bitmasks;
    size_t m_bits_per_pixel;
    size_t m_channels_count;
};


} // namespace pixel_reader
} // namespace bmp_reader