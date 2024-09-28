#pragma once

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_parser.hpp>
#include <bmp_reader/dib_headers/win_info_parser.hpp>
#include <bmp_reader/dib_headers/os22x.hpp>

namespace bmp_reader {
namespace dib_headers {

struct OS22X_Parser : public WinInfoParser {
    OS22X_Parser() {}
    virtual ~OS22X_Parser() {}

    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse_os22x(std::istream& is) -> OS22X;

    virtual inline auto parse(std::istream& is) -> std::unique_ptr<HeaderBase> {
        return std::make_unique<OS22X>(parse_os22x(is));
    }

    static const size_t HEADER_IN_FILE_SIZE = 64;

    protected:
    using WinInfoParser::parse_win_info;
};

} // namespace dib_headers
} // namespace bmp_reader