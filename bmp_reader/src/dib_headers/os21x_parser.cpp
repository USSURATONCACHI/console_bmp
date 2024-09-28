#include <bmp_reader/dib_headers/os21x_parser.hpp>

namespace bmp_reader {
namespace dib_headers {

auto OS21X_Parser::is_valid_header(BmpFileType type, size_t header_size) -> bool {
    return header_size == OS21X_Parser::HEADER_IN_FILE_SIZE && BmpFileType_is_OS2(type);
}

auto OS21X_Parser::parse_os21x(std::istream& is) -> OS21X {
    OS21X header;

    is.read(reinterpret_cast<char*>(&header.width_pixels), sizeof(header.width_pixels));
    is.read(reinterpret_cast<char*>(&header.height_pixels), sizeof(header.height_pixels));
    is.read(reinterpret_cast<char*>(&header.num_color_planes), sizeof(header.num_color_planes));
    is.read(reinterpret_cast<char*>(&header.num_bits_per_pixel), sizeof(header.num_bits_per_pixel));

    return header;
}

} // namespace dib_headers
} // namespace bmp_reader