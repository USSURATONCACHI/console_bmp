#include <console_bmp/readers/win_info_reader.hpp>

#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <console_bmp/print.hpp>
#include <console_bmp/images/rgba8.hpp>

using console_bmp::println;

namespace console_bmp {
namespace readers {

// class WinInfoReader {
// public:
// };
WinInfoReader::WinInfoReader() 
{}

auto WinInfoReader::read(std::istream& is, dib_headers::WinInfo header, BmpFileInfo info) -> images::Rgba8 {
    if (header.compression_method != dib_headers::CompressionMethod::RGB) {
        throw std::runtime_error("Compression is not supported");
    }

    size_t data_blob_size = info.bmp_file_size - info.pixel_array_offset;
    
    size_t row_size_bits = header.num_bits_per_pixel * std::abs(header.width_pixels);
    size_t row_size_aligned = ((row_size_bits + 31) / 32) * 4; // rows must be aligned to 4 bytes
    size_t blob_size_must_be = row_size_aligned * std::abs(header.height_pixels);

    if (data_blob_size != blob_size_must_be) {
        println("Blob size must be {} bytes, but it is {} bytes", blob_size_must_be, data_blob_size);
        throw std::runtime_error("Data blob has incorrect size");
    }

    std::vector<uint8_t> data_blob(data_blob_size);
    is.seekg(info.pixel_array_offset);
    is.read(reinterpret_cast<char*>(data_blob.data()), data_blob_size);

    return images::Rgba8(std::move(data_blob), header.width_pixels, header.height_pixels);
}

} // namespace readers
} // namespace console_bmp