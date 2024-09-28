#include <bmp_reader/palette_reader.hpp>

#include <bmp_reader/images/pixels.hpp>

namespace bmp_reader {


auto PaletteReader::read_bgra(std::istream& is, size_t entries_count, size_t channels_count) -> BgraPalette {
    std::vector<images::Bgra8Pixel> palette_bgra(entries_count);
    is.read(reinterpret_cast<char*>(palette_bgra.data()), palette_bgra.size() * sizeof(palette_bgra[0]));

    if (channels_count < 4) {
        for (auto& pixel : palette_bgra) {
            pixel.r = channels_count < 1 ? 0 : pixel.r;
            pixel.g = channels_count < 1 ? 0 : pixel.g;
            pixel.b = channels_count < 1 ? 0 : pixel.b;
            pixel.a = channels_count < 1 ? 0 : pixel.a;
        }
    }

    return palette_bgra;
}
auto PaletteReader::bgra_palette_to_rgba(BgraPalette&& bgra) -> RgbaPalette {
    for (images::Bgra8Pixel& pixel : bgra) {
        images::Rgba8Pixel rgba_pix = images::bgra_to_rgba(pixel);

        static_assert(sizeof(images::Bgra8Pixel) == sizeof(images::Rgba8Pixel));
        auto ptr = reinterpret_cast<images::Rgba8Pixel*>(&pixel);
        *ptr = rgba_pix;
    }
    
    return reinterpret_cast<std::vector<images::Rgba8Pixel>&>(bgra);
}


} // namespace bmp_reader