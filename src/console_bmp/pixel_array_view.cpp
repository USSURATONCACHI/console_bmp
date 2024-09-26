#include <console_bmp/pixel_array_view.hpp>

namespace console_bmp {


PixelArrayView::PixelArrayView(
    std::vector<uint8_t>&& data_block, 
    size_t image_width, 
    size_t bits_per_pixel
) 
    : data_block(std::move(data_block)),
    image_width(image_width),
    bits_per_pixel(bits_per_pixel)
{}

} // namespace console_bmp