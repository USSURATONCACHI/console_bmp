#pragma once

#include <istream>

#include <bmp_reader/readers/bmp_reader_base.hpp>
#include <bmp_reader/dib_headers/win_core.hpp>


namespace bmp_reader {
namespace readers {


class WinCoreReader : public BmpReaderHeaded<dib_headers::WinCore> {
public:
    using Parent = BmpReaderHeaded<dib_headers::WinCore>;
    using Parent::Parent;

    virtual auto read(std::istream& is) -> Rgba8Image;
};


} // namespace readers
} // namespace bmp_reader