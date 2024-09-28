#pragma once

#include <istream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <bmp_reader/images/image.hpp>
#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_parser.hpp>

namespace bmp_reader {

struct UnsupportedBmpKindException : public std::runtime_error { using std::runtime_error::runtime_error; };

struct BmpReader {
    BmpReader();
    virtual ~BmpReader(); 

    virtual auto read_bmp(std::istream& is) -> std::unique_ptr<images::Image>;
    virtual auto read_bmp_file_header(std::istream& is) -> BmpFileInfo;
    virtual void add_header_parser(std::unique_ptr<dib_headers::HeaderParser>&& ptr);

    virtual auto get_appropriate_parser(size_t header_size, BmpFileType type) -> dib_headers::HeaderParser&;


    protected:
    std::vector<std::unique_ptr<dib_headers::HeaderParser>> headers_parsers;
};

} // namespace bmp_reader