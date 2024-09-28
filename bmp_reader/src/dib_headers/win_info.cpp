#include <bmp_reader/dib_headers/win_info.hpp>

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

} // namespace dib_headers
} // namespace bmp_reader