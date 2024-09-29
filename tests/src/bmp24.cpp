#include <cstdint>
#include <vector>
#include <gtest/gtest.h>

#include <bmp_reader/bmp_reader.hpp>
#include <bmp_reader/util/print.hpp>

#include <bmp_reader/pixels.hpp>
#include "test_util.hpp"

using namespace bmp_reader;


TEST(Bmp24, Example1) {
    // https://en.wikipedia.org/wiki/BMP_file_format#Example_1
    const char* data =
        // File header
        "42 4D "
        "46 00 00 00 "
        "00 00 "
        "00 00 "
        "36 00 00 00 "

        // BMP data Header
        "28 00 00 00 "
        "02 00 00 00 "
        "02 00 00 00 "
        "01 00 "
        "18 00 "
        "00 00 00 00 "
        "10 00 00 00 "
        "13 0B 00 00 "
        "13 0B 00 00 "
        "00 00 00 00 "
        "00 00 00 00 "

        // Pixel array
        "00 00 FF "     // Red pixel
        "FF FF FF "     // White pixel
        "00 00 "
        "FF 00 00 "     // Blue pixel
        "00 FF 00 "     // Green pixel
        "00 00 ";
    
    std::vector<uint8_t> byte_data = read_string_as_hex(data);
    VectorStream<uint8_t> stream(byte_data);
    // std::istream stream(&buffer);

    BmpReader reader;
    reader.add_default_parsers();

    auto image = reader.read_bmp(stream, true);
    
    bmp_reader::Rgba8Pixel red   { .r = 255, .g = 0,   .b = 0,   .a = 255 };
    bmp_reader::Rgba8Pixel green { .r = 0,   .g = 255, .b = 0,   .a = 255 };
    bmp_reader::Rgba8Pixel blue  { .r = 0,   .g = 0,   .b = 255, .a = 255 };
    bmp_reader::Rgba8Pixel white { .r = 255, .g = 255, .b = 255, .a = 255 };

    EXPECT_EQ(image.get_rgba8_lossy(0, 0),  blue);
    EXPECT_EQ(image.get_rgba8_lossy(1, 0),  green);
    EXPECT_EQ(image.get_rgba8_lossy(0, 1),  red);
    EXPECT_EQ(image.get_rgba8_lossy(1, 1),  white);
}