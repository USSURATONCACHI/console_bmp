#include "bmp_reader/images/rgba8.hpp"
#include <bmp_reader/bmp_reader.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <format>
#include <limits>
#include <memory>
#include <stdexcept> 

#include <bmp_reader/print.hpp>
#include <bmp_reader/bit_view.hpp>

#include <bmp_reader/dib_headers/header_base.hpp>
#include <bmp_reader/dib_headers/os21x.hpp>
#include <bmp_reader/dib_headers/os22x.hpp>
#include <bmp_reader/dib_headers/win_info.hpp>
#include <bmp_reader/dib_headers/win_core.hpp>

#include <bmp_reader/readers/win_info_reader.hpp>

namespace bmp_reader {

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

auto BmpReader::read_bmp(std::istream& is) -> std::unique_ptr<images::Image> {
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

        images::Rgba8 image = reader.read(is, header_downcast, info);
        return std::make_unique<images::Rgba8>(std::move(image));
    } else {
        throw std::runtime_error("Unsupported BMP image kind: " + std::string(header->type().name()));
        return nullptr;
    }

    return nullptr;
}

auto BmpReader::read_bmp_file_header(std::istream& is) -> BmpFileInfo {
    char file_type[2];
    uint32_t pix_array_offset;
    uint32_t bmp_file_size;
    
    is.read(file_type, 2);
    is.read(reinterpret_cast<char*>(&bmp_file_size), 4);
    is.ignore(4); // reserved 4 bytes
    is.read(reinterpret_cast<char*>(&pix_array_offset), 4);

    BmpFileInfo info;
    info.pixel_array_offset = static_cast<size_t>(pix_array_offset);
    info.bmp_file_size = bmp_file_size;

    if (std::memcmp(file_type, "BM", 2) == 0)      info.file_type = BmpFileType::WIN_BM;
    else if (std::memcmp(file_type, "BA", 2) == 0) info.file_type = BmpFileType::OS2_BA;
    else if (std::memcmp(file_type, "CI", 2) == 0) info.file_type = BmpFileType::OS2_CI;
    else if (std::memcmp(file_type, "CP", 2) == 0) info.file_type = BmpFileType::OS2_CP;
    else if (std::memcmp(file_type, "IC", 2) == 0) info.file_type = BmpFileType::OS2_IC;
    else if (std::memcmp(file_type, "PT", 2) == 0) info.file_type = BmpFileType::OS2_PT;
    else throw UnsupportedBmpKindException("File is not a supported BMP type.");

    return info;
}


} // namespace bmp_reader