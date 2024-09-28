#pragma once

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_base.hpp>
#include <bmp_reader/dib_headers/header_parser.hpp>
#include <bmp_reader/dib_headers/win_info.hpp>

namespace bmp_reader {
namespace dib_headers {


struct WinInfoParser : public HeaderParser {
    WinInfoParser() {}
    virtual ~WinInfoParser() {}

    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse_win_info(std::istream& is) -> WinInfo;


    virtual inline auto parse(std::istream& is) -> std::unique_ptr<HeaderBase> {
        return std::make_unique<WinInfo>(parse_win_info(is));
    }
    
    static const size_t HEADER_IN_FILE_SIZE = 40;
};


} // namespace dib_headers
} // namespace bmp_reader