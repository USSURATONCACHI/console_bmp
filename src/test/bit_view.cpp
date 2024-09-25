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