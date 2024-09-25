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

const char* BmpFileType_to_string(BmpFileType t) {
    switch (t) {
        case BmpFileType::OS2_BA: return "BA";
        case BmpFileType::OS2_CI: return "CI";
        case BmpFileType::OS2_CP: return "CP";
        case BmpFileType::OS2_IC: return "IC";
        case BmpFileType::OS2_PT: return "PT";
        case BmpFileType::WIN_BM: return "BM";
        default: return "<unknown>";
    }
}


} // namespace console_bmp