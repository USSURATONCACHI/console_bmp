#pragma once

#include <cstddef>
#include <istream>
#include <memory>
#include <stdexcept>
#include <typeinfo>

#include <bmp_reader/dib_headers/header_base.hpp>
#include <bmp_reader/bmp_file_info.hpp>

namespace bmp_reader {
namespace dib_headers {

struct InvalidHeaderException : public std::runtime_error { using std::runtime_error::runtime_error; };

struct HeaderParser {
    virtual ~HeaderParser() {}

    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool  = 0;
    virtual auto parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase>  = 0;
};

} // namespace dib_headers
} // namespace bmp_reader