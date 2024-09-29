#pragma once

#include <cstdint>
#include <cstddef>
#include <typeinfo>

#include <bmp_reader/bmp_file_info.hpp>
#include <bmp_reader/dib_headers/header_base.hpp>

namespace bmp_reader {
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
    uint32_t header_size;
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
    virtual auto bits_per_channel() const -> size_t;
    virtual auto data_row_size() const -> size_t;
    virtual auto min_data_size() const -> size_t;
    virtual auto get_bits_per_pixel() const -> size_t;
    virtual auto channels_count() const -> size_t;

    virtual auto actual_data_size(BmpFileInfo info) const -> size_t;

    virtual void print_info() const;

    static const size_t SizeV1Bytes = 40;
    static const size_t SizeV2Bytes = 52;
    static const size_t SizeV3Bytes = 56;
    static const size_t SizeV4Bytes = 108;
    static const size_t SizeV5Bytes = 124;
};


} // namespace dib_headers
} // namespace bmp_reader