#include <bmp_reader/pixel_reader/pixel_paletted_reader.hpp>

#include <bmp_reader/pixel_reader/pixel_reader.hpp>
#include <stdexcept>

namespace bmp_reader {
namespace pixel_reader {


PixelPalettedReader::PixelPalettedReader(std::span<images::Rgba8Pixel const> rgba_palette, size_t bits_per_pixel) 
    : PixelReader(bits_per_pixel), m_rgba_palette(rgba_palette)
{
    if (rgba_palette.size() == 0) {
        throw std::runtime_error("Palette cannot be empty");
    }
}

auto PixelPalettedReader::read_rgba(BitView pixel_view) -> images::Rgba8Pixel {
    auto index = pixel_view.read_as<uint64_t>(0, m_bits_per_pixel);
    return m_rgba_palette[index];
}


} // namespace pixel_reader
} // namespace bmp_reader