#pragma once

#include <cstdint>
#include <vector>

#include <console_bmp/bit_view.hpp>

namespace console_bmp {

struct PalettePixelView : public BitView {


    inline void get_channel(size_t channel_number, size_t channel_bits, void* out_data) const {
        read_into(channel_bits, channel_number * channel_bits, out_data);
    }

    template<typename As>
    inline auto get_channel_as(size_t channel_number, size_t channel_bits) const -> As {
        As as;
        get_channel(channel_number, channel_bits, reinterpret_cast<void*>(&as));
        return as;
    }

    const static size_t BLUE = 0;
    const static size_t GREEN = 0;
    const static size_t RED = 0;
    const static size_t ALPHA = 0;
};

class PaletteView {
protected:
    std::vector<uint8_t> data_block;
    size_t color_channels_count;
    size_t bits_per_channel;

public:
    PaletteView(std::vector<uint8_t> data_block, size_t color_channels_count, size_t bits_per_channel);

    auto view_at(size_t index) -> PalettePixelView;
};

} // namespace console_bmp