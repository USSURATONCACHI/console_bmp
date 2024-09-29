#pragma once


#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>
#include <vector>

#include <bmp_reader/pixels.hpp>

namespace bmp_reader {


class Rgba8Image {
protected:
    std::vector<Rgba8Pixel> m_data;
    int64_t m_width;    // Negative size = mirrored
    int64_t m_height;

public:
    Rgba8Image(std::vector<Rgba8Pixel>&& data, int64_t width, int64_t height)
        : m_data(std::move(data)), m_width(width), m_height(height)
    {
        size_t abs_size = std::abs(width) * std::abs(height);
        if (abs_size != m_data.size())
            throw std::invalid_argument("size of data array does not match");
    }

    Rgba8Image(std::vector<uint8_t>&& data, int64_t width, int64_t height) 
        : Rgba8Image(reinterpret_cast<std::vector<Rgba8Pixel>&&>(std::move(data)), width, height)
    {
        size_t abs_size = std::abs(width) * std::abs(height);
        if (abs_size * 4 != m_data.size())
            throw std::invalid_argument("size of data array does not match (1)");
    }

    virtual ~Rgba8Image() {};

    Rgba8Pixel* data() { return m_data.data(); }
    bool flipped_w() const { return m_width < 0; }
    bool flipped_h() const { return m_height < 0; }

    virtual size_t width() const  { return std::abs(m_width); }
    virtual size_t height() const { return std::abs(m_height); }

    virtual size_t num_color_channels() const { return 4; }
    virtual size_t bits_per_channel() const   { return 8; }

    virtual Rgba8Pixel get_rgba8_lossy(size_t x, size_t y) const {
        size_t abs_width = std::abs(m_width);
        size_t abs_height = std::abs(m_height);

        size_t actual_x = m_width < 0 ? (abs_width - 1 - x) : x;
        size_t actual_y = m_height < 0 ? (abs_height - 1 - y) : y;
        
        return m_data[actual_y * m_width + actual_x];
    }
};


} // namespace bmp_reader