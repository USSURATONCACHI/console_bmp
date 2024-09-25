#include <console_bmp/dib_headers/os21x.hpp>

#include <memory>

#include <console_bmp/print.hpp>
#include <console_bmp/bmp_reader.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {
namespace dib_headers {

OS21X_Parser::OS21X_Parser() {}

OS21X_Parser::~OS21X_Parser() {}

auto OS21X_Parser::is_valid_header(BmpFileType type, size_t header_size) -> bool {
    return header_size == OS21X::IN_FILE_SIZE && BmpFileType_is_OS2(type);
}

auto OS21X_Parser::parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase> {
    println("Parsing with: {}", __PRETTY_FUNCTION__);
    OS21X header;

    is.read(reinterpret_cast<char*>(&header.width_pixels), sizeof(header.width_pixels));
    is.read(reinterpret_cast<char*>(&header.height_pixels), sizeof(header.height_pixels));
    is.read(reinterpret_cast<char*>(&header.num_color_planes), sizeof(header.num_color_planes));
    is.read(reinterpret_cast<char*>(&header.num_bits_per_pixel), sizeof(header.num_bits_per_pixel));

    return std::make_unique<OS21X>(std::move(header));
}

} // namespace dib_headers
} // namespace console_bmp