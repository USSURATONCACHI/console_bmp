#include <console_bmp/bmp_reader.hpp>
#include <console_bmp/dib_headers/os22x.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {
namespace dib_headers {

OS22X_Parser::OS22X_Parser() {}

OS22X_Parser::~OS22X_Parser() {}

auto OS22X_Parser::is_valid_header(BmpFileType, size_t header_size) -> bool {
    return header_size == OS22X::IN_FILE_SIZE;
}

auto OS22X_Parser::parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase> {
    throw UnsupportedBmpKindException("Not supported yet");
}

} // namespace dib_headers
} // namespace console_bmp