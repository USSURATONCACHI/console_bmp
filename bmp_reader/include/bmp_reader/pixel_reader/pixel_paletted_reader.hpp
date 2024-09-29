#pragma once

#include <span>

#include <bmp_reader/pixel_reader/pixel_reader.hpp>
#include <bmp_reader/images/pixels.hpp>


namespace bmp_reader {
namespace pixel_reader {


class PixelPalettedReader : public PixelReader {
public:
    PixelPalettedReader(std::span<images::Rgba8Pixel const> rgba_palette, size_t bits_per_pixel);
    virtual ~PixelPalettedReader() {}

    virtual auto read_rgba(BitView bv) -> images::Rgba8Pixel;

protected:
    std::span<images::Rgba8Pixel const> m_rgba_palette;
};


} // namespace pixel_reader
} // namespace bmp_reader