#pragma once

#include <istream>

#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/images/rgba8.hpp>
#include <bmp_reader/bitmasks_reader.hpp>
#include <bmp_reader/palette_reader.hpp>
#include <bmp_reader/bmp_file_info.hpp>
#include <memory>

namespace bmp_reader {
namespace readers {



class WinInfoReader {
public:
    WinInfoReader(
        const dib_headers::WinInfo& header,
        std::shared_ptr<BitmasksReader> bitmasks_reader,
        std::shared_ptr<PaletteReader> palette_reader,
        BmpFileInfo file_info
    );


    WinInfoReader(const dib_headers::WinInfo& header, BmpFileInfo file_info)
        : WinInfoReader(
            header, 
            std::make_shared<BitmasksReader>(header),
            std::make_shared<PaletteReader>(header.channels_count()),
            file_info
        )
    {}

    virtual ~WinInfoReader() {}

    virtual auto read(std::istream& is) -> images::Rgba8;

    static auto is_compression_supported(dib_headers::CompressionMethod method) -> bool;

private:
    const dib_headers::WinInfo& m_header;
    std::shared_ptr<BitmasksReader> m_bitmasks_reader; 
    std::shared_ptr<PaletteReader> m_palette_reader;
    BmpFileInfo m_file_info;
};

} // namespace readers
} // namespace bmp_reader