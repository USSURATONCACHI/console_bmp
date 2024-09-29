#include <cstdint>
#include <vector>
#include <gtest/gtest.h>

#include <bmp_reader/bmp_reader.hpp>
#include <bmp_reader/pixels.hpp>

#include "test_util.hpp"

using namespace bmp_reader;


TEST(Bmp32, Example1) {
    // https://en.wikipedia.org/wiki/BMP_file_format#Example_2
    const char* data =
        // File header
        "42 4D "
        "9A 00 00 00 "
        "00 00 00 00 "
        "7A 00 00 00 "

        // BMP data Header
        "6C 00 00 00 "
        "04 00 00 00 "
        "02 00 00 00 "
        "01 00 "
        "20 00 "
        "03 00 00 00 " // BI_BITFIELDS
        "20 00 00 00 " // 0x20 bytes of pixel data
        "13 0B 00 00 "
        "13 0B 00 00 "
        "00 00 00 00 " // 0 palette colors
        "00 00 00 00 " // 0 important colors
        "00 00 FF 00 " // red bitmask
        "00 FF 00 00 " // green bitmask
        "FF 00 00 00 " // blue bitmask
        "00 00 00 FF " // alpha bitmask
        "20 6E 69 57 "
            // 0x24 * 00 CIEXYZTRIPLE Color Space endpoints
            "00 00 00 00 "
            "00 00 00 00 "
            "00 00 00 00 "
            "00 00 00 00 "
            "00 00 00 00 "
            "00 00 00 00 "
            "00 00 00 00 "
            "00 00 00 00 "
            "00 00 00 00 "
            // 0x24 * 00 CIEXYZTRIPLE Color Space endpoints
        "00 00 00 00 "
        "00 00 00 00 "
        "00 00 00 00 "

        // Pixel array
        "FF 00 00 7F " // Blue
        "00 FF 00 7F " // Green
        "00 00 FF 7F " // Red
        "FF FF FF 7F " // White
        "FF 00 00 FF " // Blue
        "00 FF 00 FF " // Green
        "00 00 FF FF " // Red
        "FF FF FF FF ";// White 
    
    std::vector<uint8_t> byte_data = read_string_as_hex(data);
    VectorStream<uint8_t> stream(byte_data);

    BmpReader reader;
    reader.add_default_parsers();

    auto image = reader.read_bmp(stream, true);
    
    EXPECT_EQ(image.get_rgba8_lossy(0, 0),  (bmp_reader::Rgba8Pixel { 0,   0,   255, 255 }));
    EXPECT_EQ(image.get_rgba8_lossy(1, 0),  (bmp_reader::Rgba8Pixel { 0,   255, 0,   255 }));
    EXPECT_EQ(image.get_rgba8_lossy(2, 0),  (bmp_reader::Rgba8Pixel { 255, 0,   0,   255 }));
    EXPECT_EQ(image.get_rgba8_lossy(3, 0),  (bmp_reader::Rgba8Pixel { 255, 255, 255, 255 }));

    EXPECT_EQ(image.get_rgba8_lossy(0, 1),  (bmp_reader::Rgba8Pixel { 0,   0,   255, 127 }));
    EXPECT_EQ(image.get_rgba8_lossy(1, 1),  (bmp_reader::Rgba8Pixel { 0,   255, 0,   127 }));
    EXPECT_EQ(image.get_rgba8_lossy(2, 1),  (bmp_reader::Rgba8Pixel { 255, 0,   0,   127 }));
    EXPECT_EQ(image.get_rgba8_lossy(3, 1),  (bmp_reader::Rgba8Pixel { 255, 255, 255, 127 }));
}