#include "bmp_reader/bmp_file_info.hpp"
#include "bmp_reader/dib_headers/os21x.hpp"
#include "bmp_reader/dib_headers/os22x.hpp"
#include "bmp_reader/dib_headers/win_core.hpp"
#include "bmp_reader/dib_headers/win_info.hpp"
#include "bmp_reader/rgba8_image.hpp"
#include <bmp_reader/bmp_reader.hpp>

#include <format>
#include <cstring>

#include <bmp_reader/util/print.hpp>

#include <bmp_reader/dib_headers/os21x_parser.hpp>
#include <bmp_reader/dib_headers/os22x_parser.hpp>
#include <bmp_reader/dib_headers/win_core_parser.hpp>
#include <bmp_reader/dib_headers/win_info_parser.hpp>

#include <bmp_reader/readers/bmp_win_core_reader.hpp>
#include <bmp_reader/readers/bmp_win_info_reader.hpp>
#include <bmp_reader/readers/bmp_os21x_reader.hpp>

namespace bmp_reader {

BmpReader::BmpReader() 
    : headers_parsers()
{}

void BmpReader::add_default_parsers() {
    add_header_parser(std::make_unique<dib_headers::OS21X_Parser>());
    add_header_parser(std::make_unique<dib_headers::OS22X_Parser>());
    add_header_parser(std::make_unique<dib_headers::WinCoreParser>());
    add_header_parser(std::make_unique<dib_headers::WinInfoParser>());
}

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

static auto read_os21x_bmp(std::istream& is, BmpFileInfo info, dib_headers::OS21X& header) -> Rgba8Image {
    readers::OS21X_Reader reader(info, header);
    auto image = reader.read(is);
    return image;
}

static auto read_win_core_bmp(std::istream& is, BmpFileInfo info, dib_headers::WinCore& header) -> Rgba8Image {
    readers::WinCoreReader reader(info, header);
    auto image = reader.read(is);
    return image;
}

static auto read_win_info_bmp(std::istream& is, BmpFileInfo info, dib_headers::WinInfo& header) -> Rgba8Image {
    readers::WinInfoReader reader(info, header);
    auto image = reader.read(is);
    return image;
}

auto BmpReader::read_bmp(std::istream& is, bool show_info) -> Rgba8Image {
    // Read file header
    BmpFileInfo info = read_bmp_file_header(is);

    // Read DIB header size
    uint32_t dib_header_size = 0;
    size_t dib_header_start = is.tellg();
    is.read(reinterpret_cast<char*>(&dib_header_size), sizeof(dib_header_size));

    // Read DIB header
    dib_headers::HeaderParser& parser = get_appropriate_parser(static_cast<size_t>(dib_header_size), info.file_type);
    std::unique_ptr<dib_headers::HeaderBase> header = parser.parse(is);

    if (show_info)
        header->print_info();

    // Jump to start of data
    is.seekg(dib_header_start + dib_header_size);

    dib_headers::OS21X* os21x_header      = dynamic_cast<dib_headers::OS21X*>(header.get());
    dib_headers::WinCore* win_core_header = dynamic_cast<dib_headers::WinCore*>(header.get());
    dib_headers::WinInfo* win_info_header = dynamic_cast<dib_headers::WinInfo*>(header.get());
    
    if (os21x_header)
        return read_os21x_bmp(is, info, *os21x_header);
    else if (win_core_header)
        return read_win_core_bmp(is, info, *win_core_header);
    else if (win_info_header)
        return read_win_info_bmp(is, info, *win_info_header);
    else
        throw std::runtime_error("Unsupported BMP image kind: " + std::string(header->type().name()));
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