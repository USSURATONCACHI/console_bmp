#pragma once

#include <cstdint>
#include <vector>

namespace console_bmp {

struct Rgba8Pixel {
    uint8_t b, g, r, a;
};

struct PalettePixelView {
    uint8_t* data_ptr;
    uint8_t bits_offset;


    void get_channel(size_t channel, size_t channel_bits, void* out_data) const;

    template<typename As>
    inline auto get_channel_as(size_t channel, size_t channel_bits) const -> As {
        As as;
        get_channel(channel, channel_bits, reinterpret_cast<void*>(&as));
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