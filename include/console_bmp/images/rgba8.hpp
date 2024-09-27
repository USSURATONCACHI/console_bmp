#pragma once

#include <console_bmp/images/image.hpp>

#include <cstdlib>
#include <stdexcept>
#include <vector>
#include <iterator> // for ssize_t

namespace console_bmp {
namespace images {


class Rgba8 : public Image {
protected:
    std::vector<Rgba8Pixel> m_data;
    ssize_t m_width;    // Negative size = mirrored
    ssize_t m_height;

public:
    Rgba8(std::vector<Rgba8Pixel>&& data, ssize_t width, ssize_t height)
        : m_data(std::move(data)), m_width(width), m_height(height)
    {
        size_t abs_size = std::abs(width) * std::abs(height);
        if (abs_size != data.size())
            throw std::invalid_argument("size of data array does not match");
    }

    Rgba8(std::vector<uint8_t>&& data, ssize_t width, ssize_t height) 
        : Rgba8(reinterpret_cast<std::vector<Rgba8Pixel>&&>(std::move(data)), width, height)
    {
        size_t abs_size = std::abs(width) * std::abs(height);
        if (abs_size * 4 != data.size())
            throw std::invalid_argument("size of data array does not match");
    }

    virtual ~Rgba8() {};

    virtual size_t width() const  { return std::abs(m_width); }
    virtual size_t height() const { return std::abs(m_height); }

    virtual size_t num_color_channels() const { return 4; }
    virtual size_t bits_per_channel() const   { return 8; }

    virtual Rgba8Pixel get_rgba8_lossy(size_t x, size_t y) const {
        size_t abs_width = std::abs(m_width);
        size_t abs_height = std::abs(m_height);

        size_t actual_x = m_width < 0 ? abs_width - 1 - x : x;
        size_t actual_y = m_height < 0 ? abs_height - 1 - y : y;
        
        return m_data[(actual_y * m_width + actual_x) * 4 ];
    }
};
};

} // namespace console_bmp