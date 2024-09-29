#include <bmp_reader/readers/pixel_paletted_reader.hpp>
#include <bmp_reader/pixels.hpp>

namespace bmp_reader {
namespace readers {


PixelPalettedReader::PixelPalettedReader(size_t bits_per_pixel, std::span<Rgba8Pixel const> rgba_palette) 
    : PixelReaderBase(bits_per_pixel), m_rgba_palette(rgba_palette)
{}

auto PixelPalettedReader::read_pixel_rgba(BitView pixel_view) -> Rgba8Pixel {
    auto index = pixel_view.read_as<uint64_t>(0, m_bits_per_pixel);
    return m_rgba_palette[index];
}


} // namespace readers
} // namespace bmp_reader