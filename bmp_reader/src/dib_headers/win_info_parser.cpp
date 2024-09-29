#include <bmp_reader/dib_headers/win_info_parser.hpp>

#include <format>

namespace bmp_reader {
namespace dib_headers {


auto WinInfoParser::is_valid_header(BmpFileType, size_t header_size) -> bool {
    const size_t info_size = WinInfoParser::HEADER_IN_FILE_SIZE;
    return header_size == info_size ||  // BITMAPINFOHEADER
           header_size == 52 ||         // BITMAPV2INFOHEADER
           header_size == 56 ||         // BITMAPV3INFOHEADER
           header_size == 108 ||        // BITMAPV4HEADER
           header_size == 124;          // BITMAPV5HEADER
}

static void check_errors(WinInfo header);

auto WinInfoParser::parse_win_info(std::istream& is) -> dib_headers::WinInfo {
    WinInfo header;

    is.read(reinterpret_cast<char*>(&header.width_pixels),          sizeof(header.width_pixels));
    is.read(reinterpret_cast<char*>(&header.height_pixels),         sizeof(header.height_pixels));
    is.read(reinterpret_cast<char*>(&header.num_color_planes),      sizeof(header.num_color_planes));
    is.read(reinterpret_cast<char*>(&header.num_bits_per_pixel),    sizeof(header.num_bits_per_pixel));
    is.read(reinterpret_cast<char*>(&header.compression_method),    sizeof(header.compression_method));
    is.read(reinterpret_cast<char*>(&header.raw_data_size),         sizeof(header.raw_data_size));
    is.read(reinterpret_cast<char*>(&header.horz_resolution_ppm),   sizeof(header.horz_resolution_ppm));
    is.read(reinterpret_cast<char*>(&header.vert_resolution_ppm),   sizeof(header.vert_resolution_ppm));
    is.read(reinterpret_cast<char*>(&header.num_colors_in_pallete), sizeof(header.num_colors_in_pallete));
    is.read(reinterpret_cast<char*>(&header.num_important_colors),  sizeof(header.num_important_colors));

    check_errors(header);

    return header;
}


static void check_errors(WinInfo header) {
    if (header.num_color_planes != 1) {
        throw InvalidHeaderException(
            std::format(
                "number of color planes must be 1, but it is {} instead", 
                header.num_color_planes
            )
        );
    }

    if (header.raw_data_size == 0 && header.compression_method != CompressionMethod::RGB) {
        throw InvalidHeaderException(
            std::format(
                "image data size is set to zero, but compression method is not RGB, but {}", 
                CompressionMethod_to_string(header.compression_method)
            )
        );
    }

    if (header.compression_method == CompressionMethod::RLE8 && header.num_bits_per_pixel != 8) {
        throw InvalidHeaderException(
            std::format(
                "compression is RLE8, but bits/pixel is {} (must be 8)", 
                header.num_bits_per_pixel
            )
        );
    }

    if (header.compression_method == CompressionMethod::RLE4 && header.num_bits_per_pixel != 4) {
        throw InvalidHeaderException(
            std::format(
                "compression is RLE4, but bits/pixel is {} (must be 4)", 
                header.num_bits_per_pixel
            )
        );
    }
}


} // namespace dib_headers
} // namespace bmp_reader