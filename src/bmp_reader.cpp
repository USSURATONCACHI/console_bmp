#include "console_bmp/dib_headers/header_base.hpp"
#include <console_bmp/bmp_reader.hpp>

#include <cstddef>
#include <cstring>
#include <format>
#include <memory>

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


auto BmpReader::read_bmp(std::istream& is) -> std::unique_ptr<Bmp> {
    BmpFileInfo info = read_bmp_file_header(is);

    uint32_t dib_header_size = 0;
    is.read(reinterpret_cast<char*>(&dib_header_size), sizeof(dib_header_size));

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