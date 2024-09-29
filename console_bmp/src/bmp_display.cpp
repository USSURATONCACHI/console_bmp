#include <console_bmp/bmp_display.hpp>

#include <fstream>
#include <stdexcept>

#include <bmp_reader/images/rgba8.hpp>
#include <bmp_reader/util/print.hpp>
#include <bmp_reader/bmp_reader.hpp>

using bmp_reader::println;

namespace console_bmp {


BmpDisplay::BmpDisplay()
    : m_image({})
{}

void BmpDisplay::openBMP(const std::string& fileName) {
    std::ifstream ifs(fileName, std::ios::in | std::ios::binary);
    if (!ifs.is_open())
        throw std::runtime_error("Failed to open the file");

    
    bmp_reader::BmpReader reader;
    reader.add_default_parsers();

    println("Gonna read that BMP file...");
    bmp_reader::images::Rgba8 image = reader.read_bmp(ifs);

    println("Image size: {} x {}", image.width(), image.height());

}

void BmpDisplay::displayBMP() {

}

void BmpDisplay::closeBMP() {

}


} // namespace console_bmp