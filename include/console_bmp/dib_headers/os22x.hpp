#pragma once

#include <cstdint>
#include <console_bmp/dib_headers/header_base.hpp>
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
struct OS22X : public Win_Info {
    HalftoningAlg halftoning_alg;
    uint32_t halftoning_param_1; 
    uint32_t halftoning_param_2;

    virtual const std::type_info& type() const { return typeid(OS22X); }
    virtual size_t in_file_size() const        { return 64; }
};


} // namespace dib_headers
} // namespace console_bmp