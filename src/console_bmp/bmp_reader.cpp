#include <console_bmp/bmp_reader.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <limits>
#include <memory>
#include <stdexcept> 

#include <console_bmp/print.hpp>
#include <console_bmp/bit_view.hpp>

#include <console_bmp/images/bmp_no_palette.hpp>

#include <console_bmp/dib_headers/header_base.hpp>
#include <console_bmp/dib_headers/os21x.hpp>
#include <console_bmp/dib_headers/os22x.hpp>
#include <console_bmp/dib_headers/win_info.hpp>
#include <console_bmp/dib_headers/win_core.hpp>

#include <console_bmp/readers/win_info_reader.hpp>

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

auto BmpReader::get_appropriate_parser(size_t header_size, BmpFileType type) -> dib_headers::HeaderParser& {
    for (auto& parser : headers_parsers) {
        bool is_valid = parser->is_valid_header(type, header_size);
        if (is_valid) {
            return *parser;
        }
    }

    throw UnsupportedBmpKindException(
        std::format(
            "Did not find suitable parser (header size: {}, type: {})",
            header_size,
            BmpFileType_to_string(type)
        )
    );
}

auto BmpReader::read_bmp(std::istream& is) -> std::unique_ptr<Image> {
    // Read file header
    BmpFileInfo info = read_bmp_file_header(is);

    // Read DIB header size
    uint32_t dib_header_size = 0;
    is.read(reinterpret_cast<char*>(&dib_header_size), sizeof(dib_header_size));

    // Read DIB header
    dib_headers::HeaderParser& parser = get_appropriate_parser(static_cast<size_t>(dib_header_size), info.file_type);
    std::unique_ptr<dib_headers::HeaderBase> header = parser.parse(is);

    if (header->type() == typeid(dib_headers::WinInfo)) {
        dib_headers::WinInfo& header_downcast = *dynamic_cast<dib_headers::WinInfo*>(header.get());
        readers::WinInfoReader reader;

        reader.read(is, header_downcast);

        throw std::runtime_error("Unimplemented");
        return nullptr;
    } else {
        throw std::runtime_error("Unsupported BMP image kind: " + std::string(header->type().name()));
        return nullptr;
    }

    // Read bitmasks
    // println("There are {} bitmasks", header->bitmasks_count());
    // std::vector<uint32_t> bitmasks(header->bitmasks_count());
    // for (uint32_t& bitmask : bitmasks)
    //     is.read(reinterpret_cast<char*>(&bitmask), sizeof(bitmask));

    // // Read palette
    // size_t palette_entry_size_bits = header->palette_bits_per_channel() * header->palette_num_channels();
    // size_t palette_size_bits = palette_entry_size_bits * header->palette_num_entries();
    // size_t palette_size_bytes = (palette_size_bits + 7) / 8;

    // std::vector<uint8_t> palette_data(palette_size_bytes);
    // is.read(reinterpret_cast<char*>(palette_data.data()), palette_size_bytes);

    // // Read the image itself
    // size_t row_size = ((header->bits_per_pixel() * std::abs(header->image_width()) + 31) / 32) * 4;
    // size_t pixel_array_size = row_size * std::abs(header->image_height());

    // std::vector<uint8_t> pixel_array(pixel_array_size);
    // is.read(reinterpret_cast<char*>(pixel_array.data()), pixel_array_size);

    // // Return
    // if (header->palette_num_entries() == 0) {
    //     BmpNoPalette palette(
    //         std::move(pixel_array),
    //         std::abs(header->image_width()),
    //         std::abs(header->image_height()),
    //         header->palette_num_channels(),
    //         header->bits_per_pixel() / header->palette_num_channels()
    //     );

    //     return std::make_unique<BmpNoPalette>(std::move(palette));
    // } else {
    //     throw std::runtime_error("Paletted images are not supported yet.");
    // }

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