#pragma once

#include <span>

#include <bmp_reader/util/bit_view.hpp>
#include <bmp_reader/pixels.hpp>

namespace bmp_reader {
namespace readers {


class PixelReaderBase {
public:
    PixelReaderBase(size_t bits_per_pixel) : m_bits_per_pixel(bits_per_pixel) {}
    virtual ~PixelReaderBase() {}

    virtual auto read_pixel_rgba(BitView bv) -> Rgba8Pixel = 0;
    virtual void read_pixel_row_rgba(BitView bv, size_t row_size, std::span<Rgba8Pixel> read_into);

protected:
    size_t m_bits_per_pixel;
};


} // namespace readers
} // namespace bmp_reader