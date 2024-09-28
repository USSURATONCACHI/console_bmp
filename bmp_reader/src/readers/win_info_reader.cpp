#include <bmp_reader/dib_headers/win_info.hpp>

#include <cstdint>
#include <cstdlib>

#include <bmp_reader/util/bit_view.hpp>
#include <bmp_reader/util/print.hpp>
#include <bmp_reader/images/rgba8.hpp>
#include <bmp_reader/images/image.hpp>

#include <bmp_reader/win_info_reader/win_info_reader.hpp>

using bmp_reader::println;

namespace bmp_reader {
namespace readers {

// Helper functions
template<typename T>
static auto read_channel_to(BitView pixel_view, size_t channel_index, size_t bits_per_channel) -> T;
static auto read_bgra_pixel(BitView pixel_view, size_t bits_per_channel, size_t max_channels) -> images::Rgba8Pixel;


// Class functions
auto WinInfoReader::try_compression_if_supported(dib_headers::CompressionMethod method) -> bool {
    switch (method) {
        case dib_headers::CompressionMethod::JPEG:
        case dib_headers::CompressionMethod::PNG:
        case dib_headers::CompressionMethod::RLE8:
        case dib_headers::CompressionMethod::RLE4:
        case dib_headers::CompressionMethod::CMYKRLE8:
        case dib_headers::CompressionMethod::CMYKRLE4:
            return false;
        
        default:
            return true;
    }
    
}


void WinInfoReader::read_data_row_by_bitmasks(
    std::istream& is, 
    size_t bits_per_pixel, 
    size_t pixels_count,
    ColorBitmasks bitmasks, 
    std::span<images::Rgba8Pixel> read_into
) {

}

void WinInfoReader::read_data_row_by_palette(
    std::istream& is, 
    size_t bits_per_pixel, 
    size_t pixels_count,
    const std::span<images::Rgba8Pixel> palette,
    std::span<images::Rgba8Pixel> read_into
) {
    
}

auto WinInfoReader::read(std::istream& is, const dib_headers::WinInfo& header, BmpFileInfo info) -> images::Rgba8 {
    const size_t channels_count = header.channels_count();
    const ColorBitmasks bitmasks = this->read_bitmasks(is, header);
    const auto palette = this->read_palette(is, header, channels_count > 3);

    if (!try_compression_if_supported(header.compression_method))
        throw std::runtime_error("Unsupported compression method");

    // Check data blob
    size_t blob_size = header.actual_data_size(info);
    if (blob_size < header.min_data_size())
        throw std::runtime_error("Data blob has insufficient size. File might have been corrupted.");

    // Read data blob
    // std::vector<uint8_t> data_blob(blob_size);
    is.seekg(info.pixel_array_offset);
    // is.read(reinterpret_cast<char*>(data_blob.data()), data_blob.size());

    // Map data from the blob to pixel data
    // read with palette
    // read from bitmasks
    //  + convert from CMYK
    size_t image_pixel_width = static_cast<size_t>(std::abs(header.width_pixels));
    size_t image_pixel_height = static_cast<size_t>(std::abs(header.height_pixels));
    std::vector<images::Rgba8Pixel> pixels(image_pixel_width * image_pixel_height);

    for (size_t y = 0; y < image_pixel_height; y++) {
        for (size_t x = 0; x < image_pixel_width; x++) {
            size_t row_offset = y * row_size_bytes_padded;
            BitView pixel_view {
                .data_ptr = data_blob.data(),
                .bits_offset = row_offset * 8 + header.num_bits_per_pixel * x,
            };
            images::Rgba8Pixel& out_pixel = pixels[(image_pixel_height - 1 - y) * image_pixel_width + x];

            if (palette.size() == 0) {
                out_pixel = read_bgra_pixel(pixel_view, bits_per_channel, channels_count);
            } else {
                auto index = pixel_view.read_as<uint64_t>(0, header.num_bits_per_pixel);
                out_pixel = palette.at(index);
            }

        }
    }


    return images::Rgba8(std::move(pixels), header.width_pixels, header.height_pixels);
}

// Helper functions
template<typename T>
static auto read_channel_to(BitView pixel_view, size_t channel_index, size_t bits_per_channel) -> T {
    const size_t T_bits = sizeof(T) * 8;

    size_t bit_shift = 0;
    if (bits_per_channel > T_bits)
        bit_shift = bits_per_channel - T_bits;

    T raw_value = pixel_view.read_as<T>(channel_index * bits_per_channel + bit_shift, bits_per_channel);

    if (bits_per_channel < T_bits) {
        raw_value <<= T_bits - bits_per_channel;
    }

    return raw_value;
}

static auto read_bgra_pixel(BitView pixel_view, size_t bits_per_channel, size_t max_channels) -> images::Rgba8Pixel {
    images::Rgba8Pixel pixel { .r = 0, .g = 0, .b = 0, .a = 255 };
    uint8_t* const channels[] = { &pixel.b, &pixel.g, &pixel.r, &pixel.a };

    for (size_t c = 0; c < 4 && c < max_channels; c++) {
        *(channels[c]) = read_channel_to<uint8_t>(pixel_view, c, bits_per_channel);
    }

    return pixel;
}


} // namespace readers
} // namespace bmp_reader