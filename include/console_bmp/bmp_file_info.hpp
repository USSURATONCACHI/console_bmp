#pragma once

#include <cstddef>

namespace console_bmp {

// https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
enum class BmpFileType {
    WIN_BM,
    OS2_BA,
    OS2_CI,
    OS2_CP,
    OS2_IC,
    OS2_PT,
};

bool BmpFileType_is_OS2(BmpFileType t);
const char* BmpFileType_to_string(BmpFileType t);

struct BmpFileInfo {
    BmpFileType file_type;
    size_t bmp_file_size;
    size_t pixel_array_offset;
};

} // namespace console_bmp