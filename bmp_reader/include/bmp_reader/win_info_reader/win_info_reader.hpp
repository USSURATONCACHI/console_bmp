#pragma once

#include <istream>
#include <span>

#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/util/bit_view.hpp>
#include <bmp_reader/images/rgba8.hpp>
#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/win_info_reader/color_bitmasks.hpp>

namespace bmp_reader {
namespace readers {



class WinInfoReader {
public:
    WinInfoReader() {}
    virtual ~WinInfoReader() {}

    virtual auto read(std::istream& is, const dib_headers::WinInfo& header, BmpFileInfo info) -> images::Rgba8;

protected:
    // Simple metadata getters
    virtual auto try_compression_if_supported(dib_headers::CompressionMethod method) -> bool;

    // Reading data
    virtual void read_data_row_by_bitmasks(
        std::istream& is, 
        size_t bits_per_pixel, 
        size_t pixels_count,
        ColorBitmasks bitmasks, 
        std::span<images::Rgba8Pixel> read_into
    );
    virtual void read_data_row_by_palette(
        std::istream& is, 
        size_t bits_per_pixel, 
        size_t pixels_count,
        const std::span<images::Rgba8Pixel> palette,
        std::span<images::Rgba8Pixel> read_into
    );
};

} // namespace readers
} // namespace bmp_reader