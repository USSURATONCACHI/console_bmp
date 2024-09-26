#include "console_bmp/pixel_array_view.hpp"
#include <console_bmp/bmp_reader.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <limits>
#include <memory>

#include <console_bmp/print.hpp>
#include <console_bmp/palette_view.hpp>
#include <console_bmp/dib_headers/header_base.hpp>
#include <console_bmp/dib_headers/os21x.hpp>
#include <console_bmp/dib_headers/os22x.hpp>
#include <console_bmp/dib_headers/win_core.hpp>
#include <console_bmp/dib_headers/win_info.hpp>

namespace console_bmp {

BmpReader::BmpReader() {
    add_header_parser(std::make_unique<dib_headers::OS21X_Parser>());
    add_header_parser(std::make_unique<dib_headers::OS22X_Parser>());
    add_header_parser(std::make_unique<dib_headers::WinCoreParser>());
    add_header_parser(std::make_unique<dib_headers::WinInfoParser>());
}
BmpReader::~BmpReader() {}

void BmpReader::add_header_parser(std::unique_ptr<dib_headers::HeaderParser>&& ptr) {
    headers_parsers.push_back(std::move(ptr));
}

template<typename T>
static T pop_front_array(T* array, size_t size, size_t bits_to_pop) {
    if (size == 0)
        return 0;

    T bitmask = std::numeric_limits<T>::max() >> (32 - bits_to_pop);

    T return_value = array[0] & bitmask;

    for (size_t i = 0; i < size - 1; i++) {
        array[i] <<= bits_to_pop;
        array[i] |= (array[i + 1] & bitmask) << (32 - bits_to_pop);
    }
    array[size - 1] <<= bits_to_pop;

    return return_value;
    
}

auto BmpReader::read_bmp(std::istream& is) -> std::unique_ptr<Bmp> {
    // Read file header
    BmpFileInfo info = read_bmp_file_header(is);

    // Read DIB header size
    uint32_t dib_header_size = 0;
    is.read(reinterpret_cast<char*>(&dib_header_size), sizeof(dib_header_size));

    // Read DIB header
    std::unique_ptr<dib_headers::HeaderBase> header;
    bool is_init = false;

    for (auto& parser : headers_parsers) {
        bool is_valid = parser->is_valid_header(info.file_type, static_cast<size_t>(dib_header_size));
        if (is_valid) {
            header = parser->parse(is);
            is_init = true;
            break;
        }
    }

    if (!is_init) {
        throw UnsupportedBmpKindException(
            std::format(
                "Did not find suitable parser (header size: {}, type: {})",
                dib_header_size,
                BmpFileType_to_string(info.file_type)
            )
        );
    }

    // Read bitmasks
    println("There are {} bitmasks", header->bitmasks_count());
    std::vector<uint32_t> bitmasks(header->bitmasks_count());
    for (uint32_t& bitmask : bitmasks)
        is.read(reinterpret_cast<char*>(&bitmask), sizeof(bitmask));

    // Read palette
    println("Bits per pixel: {}", header->bits_per_pixel());
    println("Palette has {} entires", header->palette_num_entries());

    size_t bits_palette = header->palette_bits_per_channel();
    size_t num_channels = header->palette_num_channels();
    size_t palette_size = (bits_palette * num_channels * header->palette_num_entries()) / 8;

    std::vector<uint8_t> palette_data(palette_size);
    is.read(reinterpret_cast<char*>(palette_data.data()), palette_size);

    PaletteView view(std::move(palette_data), num_channels, bits_palette);

    // Read the image itself
    println("Image size: {} x {}", header->image_width(), header->image_height());
    size_t row_size = ((header->bits_per_pixel() * std::abs(header->image_width()) + 31) / 32) * 4;
    size_t pixel_array_size = row_size * std::abs(header->image_height());
    println("Pixel array size: {}", pixel_array_size);

    std::vector<uint8_t> pixel_array(pixel_array_size);
    is.seekg(info.pixel_array_offset);
    is.read(reinterpret_cast<char*>(pixel_array.data()), pixel_array_size);

    PixelArrayView pixel_array_view(std::move(pixel_array), header->image_width(), header->bits_per_pixel());

    

    return nullptr;
}

auto BmpReader::read_bmp_file_header(std::istream& is) -> BmpFileInfo {
    char file_type[2];
    uint32_t pix_array_offset;
    
    is.read(file_type, 2);
    is.ignore(4); // file size in bytes, 4 bytes long
    is.ignore(4); // reserved 4 bytes
    is.read((char*)&pix_array_offset, 4);

    BmpFileInfo info;
    info.pixel_array_offset = static_cast<size_t>(pix_array_offset);

    if (std::memcmp(file_type, "BM", 2) == 0)      info.file_type = BmpFileType::WIN_BM;
    else if (std::memcmp(file_type, "BA", 2) == 0) info.file_type = BmpFileType::OS2_BA;
    else if (std::memcmp(file_type, "CI", 2) == 0) info.file_type = BmpFileType::OS2_CI;
    else if (std::memcmp(file_type, "CP", 2) == 0) info.file_type = BmpFileType::OS2_CP;
    else if (std::memcmp(file_type, "IC", 2) == 0) info.file_type = BmpFileType::OS2_IC;
    else if (std::memcmp(file_type, "PT", 2) == 0) info.file_type = BmpFileType::OS2_PT;
    else throw UnsupportedBmpKindException("File is not a supported BMP type.");

    return info;
}


} // namespace console_bmp