#pragma once

#include <cstddef>
#include <memory>
#include <istream>

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_parser.hpp>
#include <bmp_reader/dib_headers/win_core.hpp>

namespace bmp_reader {
namespace dib_headers {


struct WinCoreParser : public HeaderParser {
    WinCoreParser() {}
    virtual ~WinCoreParser() {}

    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse_win_core(std::istream& is) -> WinCore;

    virtual inline auto parse(std::istream& is) -> std::unique_ptr<HeaderBase> {
        return std::make_unique<WinCore>(parse_win_core(is));
    }
    
    static const size_t HEADER_IN_FILE_SIZE = 12;
};


} // namespace dib_headers
} // namespace bmp_reader