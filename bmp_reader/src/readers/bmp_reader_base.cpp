#include <bmp_reader/readers/bmp_reader_base.hpp>

#include <span>
#include <cstdint>


namespace bmp_reader {
namespace readers {

BmpReaderBase::BmpReaderBase(BmpFileInfo file_info)
    : m_file_info(file_info)
{}

auto BmpReaderBase::read_row_by_row(std::istream& is, int64_t width, int64_t height, PixelReaderBase& pixel_reader) -> Rgba8Image {
    const size_t abs_width = std::abs(width);
    const size_t abs_height = std::abs(height);
    is.seekg(m_file_info.pixel_array_offset);

    std::vector<uint8_t> row_data(get_bmp_header().data_row_size());
    std::vector<Rgba8Pixel> final_pixels(abs_width * abs_height);

    // Read
    for (int64_t y = abs_height - 1; y >= 0; y--) {
        is.read(reinterpret_cast<char*>(row_data.data()), row_data.size());

        auto save_to = std::span(&final_pixels[y * abs_width], &final_pixels[(y + 1) * abs_width]);
        pixel_reader.read_pixel_row_rgba(BitView(row_data.data()), abs_width, save_to);
    }
    
    return Rgba8Image(std::move(final_pixels), width, height);
}


} // namespace readers
} // namespace bmp_reader