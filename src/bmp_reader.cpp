#include <console_bmp/bmp_reader.hpp>

#include <cstring>

namespace console_bmp {

Bmp* BmpReader::read_bmp(std::istream& is) {
    BmpFileInfo info = read_bmp_file_header(is);

    return nullptr;
}

BmpFileInfo BmpReader::read_bmp_file_header(std::istream& is) {
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

BmpReader::~BmpReader() {}

} // namespace console_bmp