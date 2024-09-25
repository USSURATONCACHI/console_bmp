#pragma once

#include <cstdint>
#include <vector>

namespace console_bmp {


class PixelArrayView {
public:
    std::vector<std::vector<uint8_t>> data_block;
    size_t image_width;
    size_t bits_per_pixel;

public:
    PixelArrayView(std::vector<uint8_t> data_block, size_t color_channels_count, size_t bits_per_channel);
};

} // namespace console_bmp