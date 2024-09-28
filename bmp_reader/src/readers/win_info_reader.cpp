#include <bmp_reader/readers/win_info_reader.hpp>

#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <bmp_reader/bit_view.hpp>
#include <bmp_reader/print.hpp>
#include <bmp_reader/images/rgba8.hpp>
#include <bmp_reader/images/image.hpp>

using bmp_reader::println;

namespace bmp_reader {
namespace readers {

// class WinInfoReader {
// public:
// };
WinInfoReader::WinInfoReader() 
{}

struct PaletteEntry {
    uint8_t b, g, r, a;
};

static auto channel_to_8bit(uint64_t channel, size_t bits) -> uint8_t {
    if (bits < 8) {
        channel >>= 8 - bits;
    } else if (bits > 8) {
        channel <<= bits - 8;
    }
    return channel;
}

static auto read_bgr_pixel(BitView pixel_view, size_t bits_per_channel) -> images::Rgba8Pixel {
    uint64_t b = pixel_view.read_as<uint64_t>(0 * bits_per_channel, bits_per_channel);
    uint64_t g = pixel_view.read_as<uint64_t>(1 * bits_per_channel, bits_per_channel);
    uint64_t r = pixel_view.read_as<uint64_t>(2 * bits_per_channel, bits_per_channel);

    return images::Rgba8Pixel {
        .r = channel_to_8bit(r, bits_per_channel),
        .g = channel_to_8bit(g, bits_per_channel),
        .b = channel_to_8bit(b, bits_per_channel),
        .a = 255,
    };
}

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

auto WinInfoReader::read(std::istream& is, dib_headers::WinInfo header, BmpFileInfo info) -> images::Rgba8 {
    if (header.compression_method != dib_headers::CompressionMethod::RGB) {
        throw std::runtime_error("Compression is not supported");
    }

    const size_t channels_count = (header.num_bits_per_pixel % 3 == 0) ? 3 : 4;
    const size_t bits_per_channel = header.num_bits_per_pixel / channels_count;

    // Read palette (RGB only for now)
    size_t palette_size = header.num_colors_in_pallete;
    if (header.num_bits_per_pixel <= 16 && palette_size == 0) {
        println("Setting palette size to be 2^{}", header.num_bits_per_pixel);
        palette_size = 1 << header.num_bits_per_pixel;
    }

    std::vector<PaletteEntry> palette_bgra(palette_size);
    is.read(reinterpret_cast<char*>(palette_bgra.data()), palette_size * sizeof(PaletteEntry));

    std::vector<images::Rgba8Pixel> palette(palette_bgra.size());
    for (size_t i = 0; i < palette.size(); i++) {
        palette[i].r = palette_bgra[i].r;
        palette[i].g = palette_bgra[i].g;
        palette[i].b = palette_bgra[i].b;
        palette[i].a = 255;
    }

    println("Palette has {} colors", palette.size());
    println("Bits per pixel is: {}", header.num_bits_per_pixel);
    // for (images::Rgba8Pixel p : palette) {
    //     println("{} {} {} {}", static_cast<uint64_t>(p.r), static_cast<uint64_t>(p.g), static_cast<uint64_t>(p.b), static_cast<uint64_t>(p.a));
    // }

    // Check data blob
    size_t data_blob_size = info.bmp_file_size - info.pixel_array_offset;
    
    size_t row_size_bits = header.num_bits_per_pixel * std::abs(header.width_pixels);
    size_t row_size_bytes_padded = ((row_size_bits + 31) / 32) * 4; // rows must be aligned to 4 bytes
    size_t blob_size_must_be = row_size_bytes_padded * std::abs(header.height_pixels);

    if (data_blob_size < blob_size_must_be) {
        println("Blob size must be {} bytes, but it is {} bytes", blob_size_must_be, data_blob_size);
        throw std::runtime_error("Data blob has incorrect size");
    }

    // Read data blob
    std::vector<uint8_t> data_blob(blob_size_must_be);
    is.seekg(info.pixel_array_offset);
    is.read(reinterpret_cast<char*>(data_blob.data()), data_blob.size());

    // Map data from the blob to pixel data
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

} // namespace readers
} // namespace bmp_reader