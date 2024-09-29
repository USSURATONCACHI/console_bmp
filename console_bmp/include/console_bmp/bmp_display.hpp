#pragma once

#include <string>
#include <optional>
#include <bmp_reader/images/rgba8.hpp>
#include <tuple>

namespace console_bmp {

class BmpDisplay {
public:
    BmpDisplay();

    void openBMP(const std::string& fileName);
    
    void displayBMP(size_t max_width, size_t max_height);
    
    void closeBMP();

protected:
    std::tuple<uint64_t, uint64_t> sum_pixels(size_t start_x, size_t start_y, size_t end_x, size_t end_y);

    std::optional<bmp_reader::images::Rgba8> m_image;
};


} // namespace console_bmp