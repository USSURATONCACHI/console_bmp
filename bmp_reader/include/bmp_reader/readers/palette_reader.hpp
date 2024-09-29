#pragma once

#include <istream>
#include <vector>

#include <bmp_reader/pixels.hpp>

namespace bmp_reader {
namespace readers {

using BgraPalette = std::vector<Bgra8Pixel>;
using RgbaPalette = std::vector<Rgba8Pixel>;

class PaletteReader {
public:
    PaletteReader(size_t channels_count);

    virtual auto read_palette_bgra(std::istream& is, size_t entries_count) -> BgraPalette;
    virtual auto read_palette_rgba(std::istream& is, size_t entries_count) -> RgbaPalette;
    
    static auto bgra_palette_to_rgba(BgraPalette&& bgra) -> RgbaPalette;

private:
    size_t m_channels_count;
};

} // namespace readers
} // namespace bmp_reader