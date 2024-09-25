#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {

bool BmpFileType_is_OS2(BmpFileType t) {
    switch (t) {
        case BmpFileType::OS2_BA:
        case BmpFileType::OS2_CI:
        case BmpFileType::OS2_CP:
        case BmpFileType::OS2_IC:
        case BmpFileType::OS2_PT:
            return true;

        case BmpFileType::WIN_BM:
        default:
            return false;
    }
}

} // namespace console_bmp