#pragma once

#include <span>

#include <bmp_reader/util/bit_view.hpp>
#include <bmp_reader/images/pixels.hpp>

namespace bmp_reader {
namespace pixel_reader {


class PixelReader {
public:
    PixelReader(size_t bits_per_pixel) : m_bits_per_pixel(bits_per_pixel) {}
    virtual ~PixelReader() {}

    virtual auto read_rgba(BitView bv) -> images::Rgba8Pixel = 0;
    virtual void read_row_rgba(BitView bv, size_t row_size, std::span<images::Rgba8Pixel> read_into);

private:
    size_t m_bits_per_pixel;
};


} // namespace pixel_reader
} // namespace bmp_reader