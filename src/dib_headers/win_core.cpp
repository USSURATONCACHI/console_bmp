#include <console_bmp/bmp_reader.hpp>
#include <console_bmp/dib_headers/win_core.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {
namespace dib_headers {

WinCoreParser::WinCoreParser() {}

WinCoreParser::~WinCoreParser() {}

auto WinCoreParser::is_valid_header(BmpFileType, size_t header_size) -> bool {
    return header_size == WinCore::IN_FILE_SIZE;
}

auto WinCoreParser::parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase> {
    throw UnsupportedBmpKindException("Not supported yet");
}

} // namespace dib_headers
} // namespace console_bmp