#pragma once

#include <cstddef>
#include <typeinfo>

namespace console_bmp {
namespace dib_headers {

struct HeaderBase {
    virtual const std::type_info& type() const = 0;
    virtual size_t in_file_size() const = 0; 
};

} // namespace dib_headers
} // namespace console_bmp