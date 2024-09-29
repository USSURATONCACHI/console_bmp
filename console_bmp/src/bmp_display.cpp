#include <console_bmp/bmp_display.hpp>

#include <fstream>
#include <tuple>
#include <cmath>

#include <bmp_reader/rgba8_image.hpp>
#include <bmp_reader/util/print.hpp>
#include <bmp_reader/bmp_reader.hpp>


using bmp_reader::print;
using bmp_reader::println;

namespace console_bmp {


BmpDisplay::BmpDisplay(bool use_gradient, bool show_info)
    : m_image({}), m_show_info(show_info), m_use_gradient(use_gradient)
{}

void BmpDisplay::openBMP(const std::string& fileName) {
    std::ifstream ifs(fileName, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
        throw std::runtime_error("Failed to open the file");

    
    bmp_reader::BmpReader reader;
    reader.add_default_parsers();

    bmp_reader::Rgba8Image image = reader.read_bmp(ifs, m_show_info);
    
    m_image = image;
}

static auto get_shown_size(size_t w, size_t h, size_t max_width, size_t max_height) -> std::pair<size_t, size_t> {
    double scaling = 1.0;

    if (max_width != 0) {
        double scaling_w = static_cast<double>(w) / static_cast<double>(max_width);
        if (scaling_w > scaling)
            scaling = scaling_w;
    }
    if (max_height != 0) {
        double scaling_h = static_cast<double>(h) / static_cast<double>(max_height);
        if (scaling_h > scaling)
            scaling = scaling_h;
    }

    size_t shown_width = static_cast<size_t>(std::round(static_cast<double>(w) / scaling));
    size_t shown_height = static_cast<size_t>(std::round(static_cast<double>(h) / scaling));

    return {shown_width, shown_height};
}

void BmpDisplay::displayBMP(size_t max_width, size_t max_height) {
    if (!m_image.has_value())
        throw std::runtime_error("No image to display");

    const auto [shown_width, shown_height] = 
        get_shown_size(m_image->width(), m_image->height(), max_width, max_height);

    println("");
    const std::string gradient = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    for (size_t y = 0; y < shown_height; y++) {
        for (size_t x = 0; x < shown_width; x++) {
            size_t start_x = m_image->width() * x / shown_width;
            size_t start_y = m_image->height() * y / shown_height;

            size_t end_x = m_image->width() * (x + 1) / shown_width;
            size_t end_y = m_image->height() * (y + 1) / shown_height;

            auto [sum, count] = sum_pixels(start_x, start_y, end_x, end_y);
            size_t char_index;

            if (m_use_gradient) {
                char_index = gradient.length() - (sum * gradient.length() / count / 256) - 1;
            } else {
                char_index = (sum * 4 / count / 256) == 0 ? (gradient.length() - 1) : 0;
            }

            print("{}", gradient.at(char_index));
        }
        print("\n");
    }
}

std::tuple<uint64_t, uint64_t> BmpDisplay::sum_pixels(size_t start_x, size_t start_y, size_t end_x, size_t end_y) {
    uint64_t sum = 0;
    uint64_t count = 0;

    for (size_t y = start_y; y < end_y; y++) {
        for (size_t x = start_x; x < end_x; x++) {
            auto pixel = m_image->get_rgba8_lossy(x, y);
            sum += pixel.r * 77; count += 77;
            sum += pixel.g * 150; count += 150;
            sum += pixel.b * 29; count += 29;
        }
    }

    return std::tuple<uint64_t, uint64_t>(sum, count);
}
void BmpDisplay::closeBMP() {
    m_image = {};
}


} // namespace console_bmp