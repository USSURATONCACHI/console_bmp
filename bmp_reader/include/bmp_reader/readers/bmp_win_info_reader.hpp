#pragma once

#include <istream>

#include <bmp_reader/readers/bmp_reader_base.hpp>
#include <bmp_reader/dib_headers/win_info.hpp>


namespace bmp_reader {
namespace readers {



class WinInfoReader : public BmpReaderHeaded<dib_headers::WinInfo> {
public:
    using Parent = BmpReaderHeaded<dib_headers::WinInfo>;
    using Parent::Parent;

    virtual auto read(std::istream& is) -> Rgba8Image;

    static auto is_compression_supported(dib_headers::CompressionMethod method) -> bool;
};


} // namespace readers
} // namespace bmp_reader