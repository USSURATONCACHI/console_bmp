#include <bmp_reader/readers/win_info_reader.hpp>

#include <cstdint>
#include <cstdlib>
#include <memory>

#include <bmp_reader/util/bit_view.hpp>
#include <bmp_reader/util/print.hpp>
#include <bmp_reader/images/rgba8.hpp>
#include <bmp_reader/images/image.hpp>

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/palette_reader.hpp>
#include <bmp_reader/images/pixels.hpp>
#include <bmp_reader/pixel_reader/pixel_bitmasked_reader.hpp>
#include <bmp_reader/pixel_reader/pixel_paletted_reader.hpp>
#include <bmp_reader/pixel_reader/pixel_reader.hpp>


namespace bmp_reader {
namespace readers {


WinInfoReader::WinInfoReader(
    const dib_headers::WinInfo& header,
    std::shared_ptr<BitmasksReader> bitmasks_reader, 
    std::shared_ptr<PaletteReader> palette_reader,
    BmpFileInfo file_info
) 
    : m_header(header),
    m_bitmasks_reader(bitmasks_reader),
    m_palette_reader(palette_reader),
    m_file_info(file_info)
{}

// Class functions
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

auto WinInfoReader::read(std::istream& is) -> images::Rgba8 {
    // Read bitmasks
    const ColorBitmasks bitmasks = m_bitmasks_reader->read_bitmasks(is);
    
    // Read palette
    BgraPalette palette_bgra = m_palette_reader->read_bgra(is, m_header.num_colors_in_pallete);
    const RgbaPalette palette = PaletteReader::bgra_palette_to_rgba(std::move(palette_bgra));

    // Check data blob
    size_t blob_size = m_header.actual_data_size(m_file_info);
    if (blob_size < m_header.min_data_size())
        throw std::runtime_error("Data blob has insufficient size. File might have been corrupted.");

    // Pick a correct pixel reader
    std::unique_ptr<pixel_reader::PixelReader> pixel_reader = {};
    if (palette.empty())
        pixel_reader = std::make_unique<pixel_reader::PixelBitmaskedReader>(bitmasks, m_header.num_bits_per_pixel, m_header.channels_count());
    else
        pixel_reader = std::make_unique<pixel_reader::PixelPalettedReader>(std::span{palette}, m_header.num_bits_per_pixel);
    
    // Read data blob row-by-row
    const size_t abs_width = std::abs(m_header.width_pixels);
    const size_t abs_height = std::abs(m_header.height_pixels);
    is.seekg(m_file_info.pixel_array_offset);

    // Preallocate the resources
    std::vector<uint8_t> row_data(m_header.data_row_size());
    std::vector<images::Rgba8Pixel> final_pixels(abs_width * abs_height);

    for (ssize_t y = abs_height - 1; y >= 0; y--) {
        is.read(reinterpret_cast<char*>(row_data.data()), row_data.size());

        auto save_to = std::span(&final_pixels[y * abs_width], &final_pixels[(y + 1) * abs_width]);
        pixel_reader->read_row_rgba(BitView(row_data.data()), abs_width, save_to);
    }
    
    return images::Rgba8(std::move(final_pixels), m_header.width_pixels, m_header.height_pixels);
}


} // namespace readers
} // namespace bmp_reader