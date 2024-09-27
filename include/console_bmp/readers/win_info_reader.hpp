#pragma once

#include <istream>
#include <console_bmp/dib_headers/win_info.hpp>

namespace console_bmp {
namespace readers {

class WinInfoReader {
public:
    WinInfoReader();
    virtual void read(std::istream& is, dib_headers::WinInfo header);
};

} // namespace readers
} // namespace console_bmp