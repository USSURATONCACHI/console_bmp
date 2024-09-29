#pragma once

#include <istream>
#include <memory>
#include <vector>

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_parser.hpp>
#include <bmp_reader/rgba8_image.hpp>

namespace bmp_reader {

struct UnsupportedBmpKindException : public std::runtime_error { using std::runtime_error::runtime_error; };

class BmpReader {
public:
    BmpReader();
    virtual ~BmpReader() {}
    virtual void add_header_parser(std::unique_ptr<dib_headers::HeaderParser>&& ptr);
    virtual void add_default_parsers();

    // Reading logic
    virtual auto read_bmp(std::istream& is, bool show_info = false) -> Rgba8Image;

    // Reading helpers
    virtual auto read_bmp_file_header(std::istream& is) -> BmpFileInfo;
    virtual auto get_appropriate_parser(size_t header_size, BmpFileType type) -> dib_headers::HeaderParser&;

protected:
    std::vector<std::unique_ptr<dib_headers::HeaderParser>> headers_parsers;
};

} // namespace bmp_reader