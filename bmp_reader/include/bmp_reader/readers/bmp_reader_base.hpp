#pragma once

#include <istream>
#include <cstdint>

#include <bmp_reader/dib_headers/header_base.hpp>
#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/rgba8_image.hpp>
#include <bmp_reader/readers/pixel_reader_base.hpp>

namespace bmp_reader {
namespace readers {


class BmpReaderBase {
public:
    BmpReaderBase(BmpFileInfo file_info);
    virtual ~BmpReaderBase() {}

    virtual auto read(std::istream& is) -> Rgba8Image = 0;

    virtual const dib_headers::HeaderBase& get_bmp_header() const = 0;

protected:
    auto read_row_by_row(std::istream& is, int64_t width, int64_t height, PixelReaderBase& pixel_reader) -> Rgba8Image;

protected:
    BmpFileInfo m_file_info;
};

// Some syntactic sugar
template<typename H>
class BmpReaderHeaded : public BmpReaderBase {
public:
    BmpReaderHeaded(BmpFileInfo file_info, const H& header)
        : BmpReaderBase(file_info), m_header(header)
    {}

    virtual const dib_headers::HeaderBase& get_bmp_header() const { return m_header; }

    virtual auto read(std::istream& is) -> Rgba8Image = 0;

protected:
    const H& m_header;
};

} // namespace readers
} // namespace bmp_reader