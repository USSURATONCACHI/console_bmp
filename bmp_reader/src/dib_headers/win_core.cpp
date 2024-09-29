#include <bmp_reader/dib_headers/win_core.hpp>

#include <cmath>
#include <bmp_reader/util/print.hpp>

namespace bmp_reader {
namespace dib_headers {

auto WinCore::data_row_size() const -> size_t {
    size_t row_size_bits = num_bits_per_pixel * std::abs(width_pixels);
    size_t row_size_bytes_padded = ((row_size_bits + 31) / 32) * 4; // rows must be aligned to 4 bytes
    return row_size_bytes_padded;
}
auto WinCore::min_data_size() const -> size_t {
    return data_row_size() * std::abs(height_pixels);
}
auto WinCore::get_bits_per_pixel() const -> size_t {
    return num_bits_per_pixel;
}
auto WinCore::channels_count() const -> size_t {
    return 3;
}

void WinCore::print_info() const {
    println("BITMAPCOREHEADAER");
    println("Image size: {} {}", width_pixels, height_pixels);
    println("Bits per pixel: {}", num_bits_per_pixel);
}


} // namespace dib_headers
} // namespace bmp_reader