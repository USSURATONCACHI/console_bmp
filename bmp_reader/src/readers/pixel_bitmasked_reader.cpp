#include <bmp_reader/readers/pixel_bitmasked_reader.hpp>

namespace bmp_reader {
namespace readers {


PixelBitmaskedReader::PixelBitmaskedReader(ColorBitmasks masks, size_t bits_per_pixel, size_t channels_count)
    : PixelReaderBase(bits_per_pixel), 
    m_bitmasks(masks), 
    m_channels_count(channels_count)
{}

static auto shift_bitmasked_to_u8(uint32_t value, uint32_t bitmask) -> uint8_t;

auto PixelBitmaskedReader::read_pixel_rgba(BitView pixel_view) -> Rgba8Pixel {
    // uint32_t because bitmasks are only 4 bytes due to bitmap standart
    auto bits = pixel_view.read_as<uint32_t>(0, m_bits_per_pixel);

    Rgba8Pixel pixel {
        .r = shift_bitmasked_to_u8(bits & m_bitmasks.r, m_bitmasks.r),
        .g = shift_bitmasked_to_u8(bits & m_bitmasks.g, m_bitmasks.g),
        .b = shift_bitmasked_to_u8(bits & m_bitmasks.b, m_bitmasks.b),
        .a = shift_bitmasked_to_u8(bits & m_bitmasks.a, m_bitmasks.a),
    };

    if (m_channels_count < 4)
        pixel.a = 255; // no opacity

    return pixel;
}


static auto shift_bitmasked_to_u8(uint32_t value, uint32_t bitmask) -> uint8_t {
    int bitmask_end = 0;
    while (bitmask != 0) {
        bitmask_end++;
        bitmask >>= 1;
    }

    if (bitmask_end >= 8) {
        value >>= bitmask_end - 8;
    } else {
        value <<= 8 - bitmask_end;
    }

    return static_cast<uint8_t>(value);
}

} // namespace readers
} // namespace bmp_reader