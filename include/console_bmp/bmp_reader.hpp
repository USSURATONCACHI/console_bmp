#pragma once

#include <istream>
#include <stdexcept>

#include <console_bmp/bmp.hpp>
#include <console_bmp/bmp_file_info.hpp>

namespace console_bmp {

struct UnsupportedBmpKindException : public std::runtime_error { using std::runtime_error::runtime_error; };

class BmpReader {
public:
    virtual ~BmpReader(); 
    virtual Bmp* read_bmp(std::istream& is);

    virtual BmpFileInfo read_bmp_file_header(std::istream& is);
};

} // namespace console_bmp