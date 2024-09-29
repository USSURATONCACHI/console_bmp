#include <bmp_reader/readers/pixel_reader_base.hpp>

namespace bmp_reader {
namespace readers {


void PixelReaderBase::read_pixel_row_rgba(
    BitView row_view, 
    size_t row_size, 
    std::span<Rgba8Pixel> read_into
) {
    for (size_t x = 0; x < row_size; x++) {
        BitView pixel_view  = row_view.subview(m_bits_per_pixel * x);
        read_into[x] = this->read_pixel_rgba(pixel_view);
    }
}


} // namespace readers
} // namespace bmp_reader