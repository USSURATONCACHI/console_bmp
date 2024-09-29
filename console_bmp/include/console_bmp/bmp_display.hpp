#pragma once

#include <string>
#include <optional>
#include <tuple>

#include <bmp_reader/rgba8_image.hpp>

namespace console_bmp {

class BmpDisplay {
public:
    BmpDisplay(bool use_gradient, bool show_info);

    void openBMP(const std::string& fileName);
    
    void displayBMP(size_t max_width, size_t max_height);

    void displayBMPInWindow();
    
    void closeBMP();

protected:
    std::tuple<uint64_t, uint64_t> sum_pixels(size_t start_x, size_t start_y, size_t end_x, size_t end_y);

protected:
    std::optional<bmp_reader::Rgba8Image> m_image;
    bool m_show_info;
    bool m_use_gradient;
};


} // namespace console_bmp