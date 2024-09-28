#pragma once

#include <typeinfo>

namespace bmp_reader {
namespace dib_headers {

struct HeaderBase {
    virtual const std::type_info& type() const = 0;
};

} // namespace dib_headers
} // namespace bmp_reader