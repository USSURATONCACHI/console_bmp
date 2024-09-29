#include <bmp_reader/readers/bitmasks_reader.hpp>

#include <limits>

namespace bmp_reader {
namespace readers {

BitmasksReader::BitmasksReader(const dib_headers::WinInfo& header) 
    : m_header(header)
{}

auto BitmasksReader::read_bitmasks(std::istream& is) -> ColorBitmasks {
    switch (m_header.compression_method) {
        case dib_headers::CompressionMethod::BITFIELDS:
            if (m_header.header_size >= dib_headers::WinInfo::SizeV3Bytes || m_header.channels_count() > 3)
                return read_rgba_bitmasks(is);
            else
                return read_rgb_bitmasks(is);

        case dib_headers::CompressionMethod::ALPHABITFIELDS:
            return read_rgba_bitmasks(is);

        default:
            return get_default_bitmasks(m_header.bits_per_channel());
    }
}

auto BitmasksReader::read_rgb_bitmasks(std::istream& is) -> ColorBitmasks {
    ColorBitmasks bitmasks { 0, 0, 0, 0 };
    is.read(reinterpret_cast<char*>(&bitmasks.r), sizeof(bitmasks.r));
    is.read(reinterpret_cast<char*>(&bitmasks.g), sizeof(bitmasks.g));
    is.read(reinterpret_cast<char*>(&bitmasks.b), sizeof(bitmasks.b));
    return bitmasks;
}

auto BitmasksReader::read_rgba_bitmasks(std::istream& is) -> ColorBitmasks {
    ColorBitmasks bitmasks { 0, 0, 0, 0 };
    is.read(reinterpret_cast<char*>(&bitmasks.r), sizeof(bitmasks.r));
    is.read(reinterpret_cast<char*>(&bitmasks.g), sizeof(bitmasks.g));
    is.read(reinterpret_cast<char*>(&bitmasks.b), sizeof(bitmasks.b));
    is.read(reinterpret_cast<char*>(&bitmasks.a), sizeof(bitmasks.a));
    return bitmasks;
}

auto BitmasksReader::get_default_bitmasks(size_t bits_per_channel) -> ColorBitmasks {
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