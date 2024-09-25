#include <console_bmp/bmp_reader.hpp>
#include <console_bmp/dib_headers/os21x.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {
namespace dib_headers {

OS21X_Parser::OS21X_Parser() {}

OS21X_Parser::~OS21X_Parser() {}

auto OS21X_Parser::is_valid_header(BmpFileType type, size_t header_size) -> bool {
    return header_size == OS21X::IN_FILE_SIZE && BmpFileType_is_OS2(type);
}

auto OS21X_Parser::parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase> {
    throw UnsupportedBmpKindException("Not supported yet");
}

} // namespace dib_headers
} // namespace console_bmp