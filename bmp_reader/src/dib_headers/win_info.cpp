#include <bmp_reader/dib_headers/win_info.hpp>

#include <cmath>

namespace bmp_reader {
namespace dib_headers {

auto CompressionMethod_to_string(CompressionMethod c) -> const char* {
    switch (c) {
        case CompressionMethod::RGB:            return "RGB";
        case CompressionMethod::RLE8:           return "RLE8";
        case CompressionMethod::RLE4:           return "RLE4";
        case CompressionMethod::BITFIELDS:      return "BITFIELDS";
        case CompressionMethod::JPEG:           return "JPEG";
        case CompressionMethod::PNG:            return "PNG";
        case CompressionMethod::ALPHABITFIELDS: return "ALPHABITFIELDS";
        case CompressionMethod::CMYK:           return "CMYK";
        case CompressionMethod::CMYKRLE8:       return "CMYKRLE8";
        case CompressionMethod::CMYKRLE4:       return "CMYKRLE4";
        default: return "<unknown compression method>";
    }
}

auto WinInfo::data_row_size() const -> size_t {
    size_t row_size_bits = num_bits_per_pixel * std::abs(width_pixels);
    size_t row_size_bytes_padded = ((row_size_bits + 31) / 32) * 4; // rows must be aligned to 4 bytes
    return row_size_bytes_padded;
}
auto WinInfo::min_data_size() const -> size_t {
    return data_row_size() * std::abs(height_pixels);
}
auto WinInfo::actual_data_size(BmpFileInfo info) const -> size_t {
    if (raw_data_size == 0)
        return info.bmp_file_size - info.pixel_array_offset;
    else
        return raw_data_size;
}
auto WinInfo::channels_count() const -> size_t {
    switch (compression_method) {
        case CompressionMethod::BITFIELDS:      return 3;
        case CompressionMethod::ALPHABITFIELDS: return 4;
        case CompressionMethod::CMYK:           return 4;
        case CompressionMethod::RGB:            return 3;

        default:
            return (num_bits_per_pixel % 3 == 0) ? 3 : 4;
    }
}
auto WinInfo::bits_per_channel() const -> size_t {
    return num_bits_per_pixel / channels_count();    
}

} // namespace dib_headers
} // namespace bmp_reader