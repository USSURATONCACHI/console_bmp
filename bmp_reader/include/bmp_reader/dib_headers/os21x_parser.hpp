#pragma once

#include <cstddef>
#include <memory>
#include <istream>

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_base.hpp>
#include <bmp_reader/dib_headers/header_parser.hpp>
#include <bmp_reader/dib_headers/os21x.hpp>

namespace bmp_reader {
namespace dib_headers {

struct OS21X_Parser : public HeaderParser {
    OS21X_Parser() {}
    virtual ~OS21X_Parser() {}

    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse_os21x(std::istream& is) -> OS21X;

    virtual inline auto parse(std::istream& is) -> std::unique_ptr<HeaderBase> {
        return std::make_unique<OS21X>(parse_os21x(is));
    }
    static const size_t HEADER_IN_FILE_SIZE = 12;
};

} // namespace dib_headers
} // namespace bmp_reader