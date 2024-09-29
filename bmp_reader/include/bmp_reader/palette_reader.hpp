#pragma once

#include <istream>
#include <vector>

#include <bmp_reader/images/pixels.hpp>

namespace bmp_reader {

using BgraPalette = std::vector<images::Bgra8Pixel>;
using RgbaPalette = std::vector<images::Rgba8Pixel>;

class PaletteReader {
public:
    PaletteReader(size_t channels_count);

    virtual auto read_bgra(std::istream& is, size_t entries_count) -> BgraPalette;
    static auto bgra_palette_to_rgba(BgraPalette&& bgra) -> RgbaPalette;

private:
    size_t m_channels_count;
};


} // namespace bmp_reader