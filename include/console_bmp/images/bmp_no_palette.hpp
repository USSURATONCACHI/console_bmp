#pragma once

#include <console_bmp/image.hpp>

#include <vector>
#include <console_bmp/bit_view.hpp>

namespace console_bmp {

class BmpNoPalette : public Image {
protected:
    std::vector<uint8_t> m_data;
    size_t m_width;
    size_t m_height;

    size_t m_channels_count;
    size_t m_bits_per_channel;

public:
    BmpNoPalette(
        std::vector<uint8_t>&& data, 
        size_t width, size_t height,
        size_t channels_count, size_t bits_per_channel
    );

    virtual ~BmpNoPalette();

    virtual size_t width() const;
    virtual size_t height() const;

    virtual size_t num_color_channels() const;
    virtual size_t bits_per_channel() const;

    virtual BitView pixel_view(size_t x, size_t y);

    virtual RgbaAnyPixel get_pixel_rgba(size_t x, size_t y);
    virtual void set_pixel_rgba(size_t x, size_t y, RgbaAnyPixel rgba);
};

} // namespace console_bmp