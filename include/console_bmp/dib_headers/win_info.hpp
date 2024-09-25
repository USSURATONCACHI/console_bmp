#pragma once

#include <cstdint>
#include <cstddef>
#include <typeinfo>
#include <typeinfo>
#include <memory>
#include <istream>

#include <console_bmp/bmp_file_info.hpp>
#include <console_bmp/dib_headers/header_base.hpp>
#include <console_bmp/dib_headers/header_parser.hpp>

namespace console_bmp {
namespace dib_headers {


enum class CompressionMethod : uint32_t {
    RGB = 0,
    RLE8 = 1,
    RLE4 = 2,
    BITFIELDS = 3,
    JPEG = 4,
    PNG = 5,
    ALPHABITFIELDS = 6,
    CMYK = 11,
    CMYKRLE8 = 12,
    CMYKRLE4 = 13,
};

auto CompressionMethod_to_string(CompressionMethod c) -> const char*;

// BITMAPINFOHEADER
struct WinInfo : public HeaderBase {
    // size of this header, 4 bytes
    int32_t width_pixels;
    int32_t height_pixels;
    int16_t num_color_planes; // must be 1 ?
    int16_t num_bits_per_pixel;
    CompressionMethod compression_method;
    uint32_t raw_data_size; // can be zero
    int32_t horz_resolution_ppm; // pixels per metre
    int32_t vert_resolution_ppm; // pixels per metre
    uint32_t num_colors_in_pallete;
    uint32_t num_important_colors;

    virtual const std::type_info& type() const { return typeid(WinInfo); }

    static const size_t IN_FILE_SIZE = 40;
    virtual size_t in_file_size() const { return IN_FILE_SIZE; }
    virtual size_t bitmasks_count() const { 
        switch (compression_method) {
            case CompressionMethod::ALPHABITFIELDS: return 4;
            case CompressionMethod::BITFIELDS:      return 3;
            default: return 0;
        }
    }
    virtual size_t bits_per_pixel() const { return static_cast<size_t>(num_bits_per_pixel); }

    virtual size_t palette_num_entries() const      { return static_cast<size_t>(num_colors_in_pallete); }
    virtual size_t palette_num_channels() const     { return 4; }
    virtual size_t palette_bits_per_channel() const { return 8; }
    virtual ssize_t image_width() const  { return static_cast<ssize_t>(width_pixels); }
    virtual ssize_t image_height() const { return static_cast<ssize_t>(height_pixels); }
};

struct WinInfoParser : public HeaderParser {
    WinInfoParser();
    virtual ~WinInfoParser();
    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase>;
    
    virtual auto parse_win_info(std::istream& is) -> dib_headers::WinInfo;
};


} // namespace dib_headers
} // namespace console_bmp