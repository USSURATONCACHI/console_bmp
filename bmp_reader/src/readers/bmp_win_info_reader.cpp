#include "bmp_reader/util/print.hpp"
#include <bmp_reader/readers/bmp_win_info_reader.hpp>

#include <span>

#include <bmp_reader/readers/palette_reader.hpp>
#include <bmp_reader/readers/bitmasks_reader.hpp>
#include <bmp_reader/readers/pixel_paletted_reader.hpp>
#include <bmp_reader/readers/pixel_bitmasked_reader.hpp>
#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/util/bit_view.hpp>


namespace bmp_reader {
namespace readers {


auto WinInfoReader::is_compression_supported(dib_headers::CompressionMethod method) -> bool {
    switch (method) {
        case dib_headers::CompressionMethod::JPEG:
        case dib_headers::CompressionMethod::PNG:
        case dib_headers::CompressionMethod::RLE8:
        case dib_headers::CompressionMethod::RLE4:
        case dib_headers::CompressionMethod::CMYKRLE8:
        case dib_headers::CompressionMethod::CMYKRLE4:
            return false;
        
        default:
            return true;
    }
    
}

auto WinInfoReader::read(std::istream& is) -> Rgba8Image {
    // Read palette
    PaletteReader palette_reader(m_header.channels_count());
    RgbaPalette palette = palette_reader.read_palette_rgba(is, m_header.num_colors_in_pallete);
    PixelPalettedReader pixel_paletted_reader(m_header.num_bits_per_pixel, palette);

    // Read bitmasks
    BitmasksReader bitmasks_reader(m_header);
    auto bitmasks = bitmasks_reader.read_bitmasks(is);
    println("Bitmasks: {} {} {} {}", bitmasks.b, bitmasks.g, bitmasks.r, bitmasks.a);
    PixelBitmaskedReader pixel_masked_reader(bitmasks, m_header.num_bits_per_pixel, m_header.channels_count());
    
    // Check data blob
    size_t blob_size = m_header.actual_data_size(m_file_info);
    if (blob_size < m_header.min_data_size())
        throw std::runtime_error("Data blob has insufficient size. File might have been corrupted.");

    // Pick a correct pixel reader
    if (palette.empty())
        return read_row_by_row(is, m_header.width_pixels, m_header.height_pixels, pixel_masked_reader);
    else
        return read_row_by_row(is, m_header.width_pixels, m_header.height_pixels, pixel_paletted_reader);
}


} // namespace readers
} // namespace bmp_reader