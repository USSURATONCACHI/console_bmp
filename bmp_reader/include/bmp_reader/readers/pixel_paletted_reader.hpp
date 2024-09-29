#pragma once

#include <span>

#include <bmp_reader/readers/pixel_reader_base.hpp>
#include <bmp_reader/pixels.hpp>


namespace bmp_reader {
namespace readers {


class PixelPalettedReader : public PixelReaderBase {
public:
    PixelPalettedReader(size_t bits_per_pixel, std::span<Rgba8Pixel const> rgba_palette);

    virtual auto read_pixel_rgba(BitView bv) -> Rgba8Pixel;

protected:
    std::span<Rgba8Pixel const> m_rgba_palette;
};


} // namespace readers
} // namespace bmp_reader