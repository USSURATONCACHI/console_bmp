#include <bmp_reader/readers/bmp_win_core_reader.hpp>

#include <bmp_reader/pixels.hpp>
#include <bmp_reader/rgba8_image.hpp>

#include <bmp_reader/readers/palette_reader.hpp>
#include <bmp_reader/readers/pixel_paletted_reader.hpp>

namespace bmp_reader {
namespace readers {


auto WinCoreReader::read(std::istream& is) -> Rgba8Image {
    // Read palette
    PaletteReader palette_reader(m_header.channels_count());
    RgbaPalette palette = palette_reader.read_palette_rgba(is, static_cast<size_t>(1) << m_header.num_bits_per_pixel);
    
    PixelPalettedReader pixel_reader(m_header.num_bits_per_pixel, palette);

    // Check data blob
    size_t blob_size = m_file_info.bmp_file_size - m_file_info.pixel_array_offset;
    if (blob_size < m_header.min_data_size())
        throw std::runtime_error("Data blob has insufficient size. File might have been corrupted.");

    // Prepare to read data
    return read_row_by_row(is, m_header.width_pixels, m_header.height_pixels, pixel_reader);
}


} // namespace readers
} // namespace bmp_reader