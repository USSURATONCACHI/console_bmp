#pragma once

#include <istream>
#include <memory>
#include <stdexcept>
#include <vector>

#include <console_bmp/bmp.hpp>
#include <console_bmp/bmp_file_info.hpp>
#include <console_bmp/dib_headers/header_parser.hpp>

namespace console_bmp {

struct UnsupportedBmpKindException : public std::runtime_error { using std::runtime_error::runtime_error; };

struct BmpReader {
    BmpReader();
    virtual ~BmpReader(); 

    virtual auto read_bmp(std::istream& is) -> std::unique_ptr<Bmp>;
    virtual auto read_bmp_file_header(std::istream& is) -> BmpFileInfo;
    virtual void add_header_parser(std::unique_ptr<dib_headers::HeaderParser>&& ptr);

    protected:
    std::vector<std::unique_ptr<dib_headers::HeaderParser>> headers_parsers;
};

} // namespace console_bmp