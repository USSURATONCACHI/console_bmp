#pragma once

#include <string>
#include <optional>
#include <bmp_reader/images/rgba8.hpp>

namespace console_bmp {

class BmpDisplay {
public:
    BmpDisplay();

    void openBMP(const std::string& fileName);
    void displayBMP();
    void closeBMP();

private:
    std::optional<bmp_reader::images::Rgba8> m_image;
};


} // namespace console_bmp