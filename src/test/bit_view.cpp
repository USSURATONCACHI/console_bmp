#include <cstdint>
#include <gtest/gtest.h>

#include <console_bmp/bit_view.hpp>

using namespace console_bmp;

TEST(BitView, AlignedRead) {
    uint8_t data[] = { 0x10, 0x32, 0x54, 0x76 };
    
    BitView view { .data_ptr = data, .bits_offset = 0 };

    EXPECT_EQ(view.read_as<uint8_t>(0), 0x10);
    EXPECT_EQ(view.read_as<uint8_t>(8), 0x32);
    EXPECT_EQ(view.read_as<uint8_t>(16), 0x54);
    EXPECT_EQ(view.read_as<uint8_t>(24), 0x76);

    EXPECT_EQ(view.read_as<uint32_t>(0), 0x76543210);
}


TEST(BitView, ShiftedRead1) {
    uint8_t data[] = { 0x10, 0x32, 0x54, 0x76 };
    
    BitView view { .data_ptr = data, .bits_offset = 4 };

    EXPECT_EQ(view.read_as<uint8_t>(0), 0x21);
    EXPECT_EQ(view.read_as<uint8_t>(8), 0x43);
    EXPECT_EQ(view.read_as<uint8_t>(16), 0x65);
    EXPECT_EQ(view.read_as<uint8_t>(24), 0x07);

    EXPECT_EQ(view.read_as<uint32_t>(0, 32 - 4), 0x07654321);
    EXPECT_EQ(view.read_as<uint32_t>(4, 32 - 8), 0x00765432);
    EXPECT_EQ(view.read_as<uint32_t>(8, 32 - 12), 0x00076543);
}

TEST(BitView, ShiftedRead2) {
    uint8_t data[] = { 0x10, 0x32, 0x54, 0x76 };
    
    BitView view { .data_ptr = data, .bits_offset = 2 };

    EXPECT_EQ(view.read_as<uint8_t>(2 + 0), 0x21);
    EXPECT_EQ(view.read_as<uint8_t>(2 + 8), 0x43);
    EXPECT_EQ(view.read_as<uint8_t>(2 + 16), 0x65);
    EXPECT_EQ(view.read_as<uint8_t>(2 + 24), 0x07);

    EXPECT_EQ(view.read_as<uint32_t>(2 + 0, 32 - 4), 0x07654321);
    EXPECT_EQ(view.read_as<uint32_t>(2 + 4, 32 - 8), 0x00765432);
    EXPECT_EQ(view.read_as<uint32_t>(2 + 8, 32 - 12), 0x00076543);
}


TEST(BitView, ShiftedRead3) {
    uint8_t data[] = { 0b00010000, 0b00110010, 0b01010100, 0b01110110 }; // 0b01110110010101000011001000010000
    
    BitView view { .data_ptr = data, .bits_offset = 0 };

    EXPECT_EQ(view.read_as<uint16_t>(3),  0b10000110'01000010);
    EXPECT_EQ(view.read_as<uint16_t>(5),  0b10100001'10010000);
    EXPECT_EQ(view.read_as<uint16_t>(7),  0b10101000'01100100);
    EXPECT_EQ(view.read_as<uint16_t>(11), 0b11001010'10000110);
    EXPECT_EQ(view.read_as<uint16_t>(13), 0b10110010'10100001);

    EXPECT_EQ(view.read_as<uint32_t>(7, 32 - 7), 0b0111011001010100001100100);
    EXPECT_EQ(view.read_as<uint32_t>(13, 32 - 13), 0b0111011001010100001);
    EXPECT_EQ(view.read_as<uint32_t>(19, 32 - 19), 0b0111011001010);
}


TEST(BitView, ShiftedWrite) {
    uint64_t data = 0x01234567'89ABCDEF;
    
    BitView view { .data_ptr = reinterpret_cast<uint8_t*>(&data), .bits_offset = 0 };

    view.write_as(12, static_cast<uint16_t>(0xCBA9));

    EXPECT_EQ(view.read_as<uint16_t>(12), static_cast<uint16_t>(0xCBA9));
    EXPECT_EQ(data, 0x01234567'8CBA9DEF);
}
