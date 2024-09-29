#pragma once

#include <istream>

#include <bmp_reader/readers/bmp_reader_base.hpp>
#include <bmp_reader/dib_headers/os21x.hpp>


namespace bmp_reader {
namespace readers {


class OS21X_Reader : public BmpReaderHeaded<dib_headers::OS21X> {
public:
    using Parent = BmpReaderHeaded<dib_headers::OS21X>;
    using Parent::Parent;

    virtual auto read(std::istream& is) -> Rgba8Image;
};


} // namespace readers
} // namespace bmp_reader