#include <bmp_reader/pixel_reader/pixel_reader.hpp>

namespace bmp_reader {
namespace pixel_reader {


void PixelReader::read_row_rgba(
    BitView row_view, 
    size_t row_size, 
    std::span<images::Rgba8Pixel>& read_into
) {
    for (size_t x = 0; x < row_size; x++) {
        BitView pixel_view  = row_view.subview(m_bits_per_pixel * x);
        read_into[x] = this->read_rgba(pixel_view);
    }
}


} // namespace pixel_reader
} // namespace bmp_reader