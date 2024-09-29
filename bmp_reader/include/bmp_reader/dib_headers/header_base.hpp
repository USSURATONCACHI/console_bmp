#pragma once

#include <typeinfo>
#include <cstddef>

namespace bmp_reader {
namespace dib_headers {

struct HeaderBase {
    virtual const std::type_info& type() const = 0;
    virtual auto data_row_size() const -> size_t = 0;
    virtual auto min_data_size() const -> size_t = 0;
    virtual auto get_bits_per_pixel() const -> size_t = 0;
    virtual auto channels_count() const -> size_t = 0;
};

} // namespace dib_headers
} // namespace bmp_reader