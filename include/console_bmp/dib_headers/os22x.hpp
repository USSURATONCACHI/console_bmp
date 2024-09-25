#pragma once

#include <cstdint>
#include <memory>
#include <istream>

#include <console_bmp/bmp_file_info.hpp>
#include <console_bmp/dib_headers/header_base.hpp>
#include <console_bmp/dib_headers/header_parser.hpp>
#include <console_bmp/dib_headers/win_info.hpp>

namespace console_bmp {
namespace dib_headers {



enum class HalftoningAlg : uint16_t {
    NONE = 0,
    ERROR_DIFFUSION = 1,
    PANDA = 2,    // PANDA: Processing Algorithm for Noncoded Document Acquisition
    SUPER_CIRCLE = 3,
};

// OS22XBITMAPHEADER
struct OS22X : public WinInfo {
    HalftoningAlg halftoning_alg;
    uint32_t halftoning_param_1; 
    uint32_t halftoning_param_2;

    OS22X(WinInfo);
    virtual const std::type_info& type() const { return typeid(OS22X); }
    
    static const size_t IN_FILE_SIZE = 64;
    virtual size_t in_file_size() const        { return IN_FILE_SIZE; }
};


struct OS22X_Parser : public WinInfoParser {
    OS22X_Parser();
    virtual ~OS22X_Parser();
    virtual auto is_valid_header(BmpFileType type, size_t header_size) -> bool;
    virtual auto parse(std::istream& is) -> std::unique_ptr<dib_headers::HeaderBase>;

    protected:
    using WinInfoParser::parse_win_info;
};

} // namespace dib_headers
} // namespace console_bmp