#pragma once

#include <cstddef>
#include <typeinfo>
#include <iterator>

namespace console_bmp {
namespace dib_headers {

struct HeaderBase {
    virtual ssize_t image_width() const = 0;
    virtual ssize_t image_height() const = 0;

    virtual const std::type_info& type() const = 0;
    virtual size_t in_file_size() const = 0;

    virtual size_t bitmasks_count() const = 0;
    virtual size_t bits_per_pixel() const = 0;

    virtual size_t palette_size() const = 0;
    virtual size_t palette_num_channels() const = 0;
    virtual size_t palette_bits_per_channel() const = 0;
};

} // namespace dib_headers
} // namespace console_bmp