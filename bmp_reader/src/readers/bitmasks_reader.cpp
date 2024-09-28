#include <bmp_reader/win_info_reader/bitmasks_reader.hpp>

#include <limits>

namespace bmp_reader {
namespace readers {


auto BitmasksReader::read_bitmasks(std::istream& is, const dib_headers::WinInfo& header) -> ColorBitmasks {
    switch (header.compression_method) {
        case dib_headers::CompressionMethod::BITFIELDS:
            return read_bgr_bitmasks(is);

        case dib_headers::CompressionMethod::ALPHABITFIELDS:
            return read_bgra_bitmasks(is);

        default:
            return get_default_bitmasks(header);
    }
}

auto BitmasksReader::read_bgr_bitmasks(std::istream& is) -> ColorBitmasks {
    ColorBitmasks bitmasks { 0, 0, 0, 0 };
    is.read(reinterpret_cast<char*>(&bitmasks.b), sizeof(bitmasks.b));
    is.read(reinterpret_cast<char*>(&bitmasks.g), sizeof(bitmasks.g));
    is.read(reinterpret_cast<char*>(&bitmasks.r), sizeof(bitmasks.r));
    return bitmasks;
}

auto BitmasksReader::read_bgra_bitmasks(std::istream& is) -> ColorBitmasks {
    ColorBitmasks bitmasks { 0, 0, 0, 0 };
    is.read(reinterpret_cast<char*>(&bitmasks.b), sizeof(bitmasks.b));
    is.read(reinterpret_cast<char*>(&bitmasks.g), sizeof(bitmasks.g));
    is.read(reinterpret_cast<char*>(&bitmasks.r), sizeof(bitmasks.r));
    is.read(reinterpret_cast<char*>(&bitmasks.a), sizeof(bitmasks.a));
    return bitmasks;
}

auto BitmasksReader::get_default_bitmasks(const dib_headers::WinInfo& header) -> ColorBitmasks {
    const size_t bits_per_channel = header.bits_per_channel();
    const uint32_t channel_mask = std::numeric_limits<uint32_t>::max() >> (32 - bits_per_channel);

    return ColorBitmasks {
        .b = channel_mask << (0 * bits_per_channel),
        .g = channel_mask << (1 * bits_per_channel),
        .r = channel_mask << (2 * bits_per_channel),
        .a = channel_mask << (3 * bits_per_channel),
    };
}


} // namespace readers
} // namespace bmp_reader