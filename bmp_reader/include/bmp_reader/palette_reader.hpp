#pragma once

#include <istream>
#include <vector>

#include <bmp_reader/images/pixels.hpp>

namespace bmp_reader {

using BgraPalette = std::vector<images::Bgra8Pixel>;
using RgbaPalette = std::vector<images::Rgba8Pixel>;

class PaletteReader {
public:
    PaletteReader() {}

    virtual auto read_bgra(std::istream& is, size_t entries_count, size_t channels_count) -> BgraPalette;
    virtual auto bgra_palette_to_rgba(BgraPalette&& bgra) -> RgbaPalette;
};


} // namespace bmp_reader