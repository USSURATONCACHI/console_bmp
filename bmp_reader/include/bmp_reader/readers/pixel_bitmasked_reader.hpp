#pragma once

#include <bmp_reader/readers/pixel_reader_base.hpp>
#include <bmp_reader/color_bitmasks.hpp>
#include <bmp_reader/pixels.hpp>


namespace bmp_reader {
namespace readers {


class PixelBitmaskedReader : public PixelReaderBase {
public:
    PixelBitmaskedReader(ColorBitmasks masks, size_t bits_per_pixel, size_t channels_count);
    virtual ~PixelBitmaskedReader() {}

    virtual auto read_pixel_rgba(BitView bv) -> Rgba8Pixel;

protected:
    ColorBitmasks m_bitmasks;
    size_t m_channels_count;
};


} // namespace readers
} // namespace bmp_reader