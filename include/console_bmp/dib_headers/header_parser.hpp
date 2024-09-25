#pragma once

#include <cstddef>
#include <istream>
#include <memory>
#include <typeinfo>

#include <console_bmp/dib_headers/header_base.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {
namespace dib_headers {

struct HeaderParser {
    virtual ~HeaderParser() {}

    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool  = 0;
    virtual auto parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase>  = 0;
};

} // namespace dib_headers
} // namespace console_bmp