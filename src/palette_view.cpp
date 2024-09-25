#include <console_bmp/palette_view.hpp>
#include <cstdint>

namespace console_bmp {

PaletteView::PaletteView(std::vector<uint8_t> data_block, size_t color_channels_count, size_t bits_per_channel)
    : data_block(data_block), color_channels_count(color_channels_count), bits_per_channel(bits_per_channel)
{}

auto PaletteView::view_at(size_t index) -> PalettePixelView {
    size_t pos_bits = index * color_channels_count * bits_per_channel;
    size_t pos_bytes = pos_bits / 8;
    size_t offset = pos_bits % 8;

    return PalettePixelView {
        BitView {
            .data_ptr = &data_block[pos_bytes],
            .bits_offset = static_cast<uint8_t>(offset),
        }
    };
}

} // namespace console_bmp