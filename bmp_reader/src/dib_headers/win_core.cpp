#include <bmp_reader/dib_headers/win_core.hpp>

#include <memory>

#include <bmp_reader/print.hpp>
#include <bmp_reader/bmp_reader.hpp>
#include <bmp_reader/bmp_file_info.hpp>

namespace bmp_reader {
namespace dib_headers {

WinCoreParser::WinCoreParser() {}

WinCoreParser::~WinCoreParser() {}

auto WinCoreParser::is_valid_header(BmpFileType type, size_t header_size) -> bool {
    return header_size == WinCore::IN_FILE_SIZE && !BmpFileType_is_OS2(type);
}

auto WinCoreParser::parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase> {
    println("Parsing with: {}", __PRETTY_FUNCTION__);
    WinCore header;

    is.read(reinterpret_cast<char*>(&header.width_pixels), sizeof(header.width_pixels));
    is.read(reinterpret_cast<char*>(&header.height_pixels), sizeof(header.height_pixels));
    is.read(reinterpret_cast<char*>(&header.num_color_planes), sizeof(header.num_color_planes));
    is.read(reinterpret_cast<char*>(&header.num_bits_per_pixel), sizeof(header.num_bits_per_pixel));

    return std::make_unique<WinCore>(std::move(header));
}

} // namespace dib_headers
} // namespace bmp_reader