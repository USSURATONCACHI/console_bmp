#include <console_bmp/bmp_reader.hpp>
#include <console_bmp/dib_headers/win_info.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {
namespace dib_headers {

WinInfoParser::WinInfoParser() {}

WinInfoParser::~WinInfoParser() {}

auto WinInfoParser::is_valid_header(BmpFileType, size_t header_size) -> bool {
    return header_size == WinInfo::IN_FILE_SIZE;
}

auto WinInfoParser::parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase> {
    throw UnsupportedBmpKindException("Not supported yet");
}

} // namespace dib_headers
} // namespace console_bmp