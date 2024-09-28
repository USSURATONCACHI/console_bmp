#include <console_bmp/dib_headers/win_info.hpp>

#include <memory>
#include <format>

#include <console_bmp/print.hpp>
#include <console_bmp/bmp_reader.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {
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

WinInfoParser::WinInfoParser() {}

WinInfoParser::~WinInfoParser() {}

auto WinInfoParser::is_valid_header(BmpFileType, size_t header_size) -> bool {
    return header_size == WinInfo::IN_FILE_SIZE;
}

static void check_errors(WinInfo header);

auto WinInfoParser::parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase> {
    return std::make_unique<WinInfo>(parse_win_info(is));
}


auto WinInfoParser::parse_win_info(std::istream& is) -> dib_headers::WinInfo {
    println("Parsing with: {}", __PRETTY_FUNCTION__);

    WinInfo header;

    is.read(reinterpret_cast<char*>(&header.width_pixels), sizeof(header.width_pixels));
    is.read(reinterpret_cast<char*>(&header.height_pixels), sizeof(header.height_pixels));
    is.read(reinterpret_cast<char*>(&header.num_color_planes), sizeof(header.num_color_planes));
    is.read(reinterpret_cast<char*>(&header.num_bits_per_pixel), sizeof(header.num_bits_per_pixel));
    is.read(reinterpret_cast<char*>(&header.compression_method), sizeof(header.compression_method));
    is.read(reinterpret_cast<char*>(&header.raw_data_size), sizeof(header.raw_data_size));
    is.read(reinterpret_cast<char*>(&header.horz_resolution_ppm), sizeof(header.horz_resolution_ppm));
    is.read(reinterpret_cast<char*>(&header.vert_resolution_ppm), sizeof(header.vert_resolution_ppm));
    is.read(reinterpret_cast<char*>(&header.num_colors_in_pallete), sizeof(header.num_colors_in_pallete));
    is.read(reinterpret_cast<char*>(&header.num_important_colors), sizeof(header.num_important_colors));

    // println("Important: {}, total colors: {}", header.num_important_colors, header.num_colors_in_pallete);

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
} // namespace console_bmp