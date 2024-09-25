#include <console_bmp/palette_view.hpp>
#include <cstdint>
#include <limits>

namespace console_bmp {

// class PaletteView {
// protected:
//     std::vector<uint8_t> data_block;
//     size_t color_channels_count;
//     size_t bits_per_channel;
// };


void PalettePixelView::get_channel(size_t channel, size_t channel_bits, void* out_data) const{
    size_t cur_bit_offset = bits_offset + channel * channel_bits;

    size_t pos = cur_bit_offset / 8;
    size_t remainder = cur_bit_offset % 8;
    size_t size = (channel_bits + 7) / 8;

    uint8_t bitmask = std::numeric_limits<uint8_t>::max() >> ((sizeof(uint8_t) * 8) - remainder);
    
    uint8_t* write_ptr = reinterpret_cast<uint8_t*>(out_data);
    for (size_t i = 0; i < size; i++) {
        uint8_t current_byte = 0;

        current_byte |= data_ptr[pos + i] >> remainder;

        if (remainder > 0)
            current_byte |= (data_ptr[pos + i + 1] & bitmask) << ((sizeof(uint8_t) * 8) - remainder);
        
        write_ptr[i] = current_byte;
    }
}

// public:
PaletteView::PaletteView(std::vector<uint8_t> data_block, size_t color_channels_count, size_t bits_per_channel)
    : data_block(data_block), color_channels_count(color_channels_count), bits_per_channel(bits_per_channel)
{}

auto PaletteView::view_at(size_t index) -> PalettePixelView {
    size_t pos_bits = index * color_channels_count * bits_per_channel;
    size_t pos_bytes = pos_bits / 8;
    size_t offset = pos_bits % 8;

    return PalettePixelView {
        .data_ptr = &data_block[pos_bytes],
        .bits_offset = static_cast<uint8_t>(offset),
    };
}

} // namespace console_bmp