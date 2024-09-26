#include "console_bmp/bit_view.hpp"
#include "console_bmp/image.hpp"
#include <console_bmp/images/bmp_no_palette.hpp>
#include <cstdint>

namespace console_bmp {

BmpNoPalette::BmpNoPalette(
    std::vector<uint8_t>&& data, 
    size_t width, size_t height,
    size_t channels_count, size_t bits_per_channel
) 
    :m_data(data),
    m_width(width),
    m_height(height),
    m_channels_count(channels_count),
    m_bits_per_channel(bits_per_channel)
{}

BmpNoPalette::~BmpNoPalette() {}

size_t BmpNoPalette::width() const {
    return m_width;
}
size_t BmpNoPalette::height() const {
    return m_height;
}

size_t BmpNoPalette::num_color_channels() const {
    return m_channels_count;
}
size_t BmpNoPalette::bits_per_channel() const {
    return m_bits_per_channel;
}

BitView BmpNoPalette::pixel_view(size_t x, size_t y) {
    const size_t row_width_bits_unpadded = 
        m_channels_count * m_bits_per_channel * m_width;
    
    const size_t row_width_bytes_padded = 
        (row_width_bits_unpadded + 31 / 32) * 4;

    return BitView {
        .data_ptr = m_data.data(),
        .bits_offset = (y * row_width_bytes_padded) * 8 
                + x * m_channels_count * m_bits_per_channel,
    };
}


RgbaAnyPixel BmpNoPalette::get_pixel_rgba(size_t x, size_t y) {
    BitView view = pixel_view(x, y);

    RgbaAnyPixel pixel {0, 0, 0, 0};

    if (num_color_channels() <= 1)
        pixel.b = view.read_as<uint64_t>(0 * bits_per_channel(), bits_per_channel()); 
    if (num_color_channels() <= 2)
        pixel.g = view.read_as<uint64_t>(1 * bits_per_channel(), bits_per_channel()); 
    if (num_color_channels() <= 3)
        pixel.r = view.read_as<uint64_t>(2 * bits_per_channel(), bits_per_channel()); 
    if (num_color_channels() <= 4)
        pixel.a = view.read_as<uint64_t>(3 * bits_per_channel(), bits_per_channel()); 

    return pixel;
}

void BmpNoPalette::set_pixel_rgba(size_t x, size_t y, RgbaAnyPixel rgba) {
    BitView view = pixel_view(x, y);

    if (num_color_channels() <= 1)
        view.write_as(0 * bits_per_channel(), rgba.b, bits_per_channel());
    if (num_color_channels() <= 2)
        view.write_as(0 * bits_per_channel(), rgba.g, bits_per_channel());
    if (num_color_channels() <= 3)
        view.write_as(0 * bits_per_channel(), rgba.r, bits_per_channel());
    if (num_color_channels() <= 4)
        view.write_as(0 * bits_per_channel(), rgba.a, bits_per_channel());
}

} // namespace console_bmp