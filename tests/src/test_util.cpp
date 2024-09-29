#include "test_util.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <stdexcept>

auto is_hex_symbol(char c) -> bool {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F');
}

auto symbol_to_hex_val(char c) -> uint8_t {
    if (c >= '0' && c <= '9') 
        return static_cast<uint8_t>(c - '0');
    else
        return static_cast<uint8_t>(c - 'A' + 10);
}

void push_half_byte(std::vector<uint8_t>& push_into, bool& is_last_elem_half_byte, uint8_t value) {
    if (is_last_elem_half_byte) {
        push_into.back() |= value << 4;
    } else {
        push_into.push_back(value);
    }
    is_last_elem_half_byte = !is_last_elem_half_byte;
}

void read_word_big_endian(std::span<const char> word, std::vector<uint8_t>& push_into, bool& is_last_elem_half_byte) {
    for (auto it = word.end() - 1; it >= word.begin(); it--) {
        push_half_byte(push_into, is_last_elem_half_byte, symbol_to_hex_val(*it));
    }
}

auto read_string_as_hex(const std::string& data) -> std::vector<uint8_t> {
    std::vector<uint8_t> hex_data;
    bool is_last_elem_half_byte = false;

    bool is_reading_word = false;
    size_t word_start = 0;
    for (size_t i = 0; i < data.length(); i++) {
        char c = data[i];

        if (is_hex_symbol(c)) {
            if (is_reading_word) {
                // nothing
            } else {
                word_start = i;
                is_reading_word = true;
            }
        } else if (c == ' ') {
            if (is_reading_word) {
                std::span<const char> span = std::span { data.begin() + word_start, data.begin() + i };
                read_word_big_endian(span, hex_data, is_last_elem_half_byte);

                word_start = 0;
                is_reading_word = false;
            } else {
            }
        } else {
            throw std::runtime_error("non-hex non-space character present");
        }
    }

    if (is_reading_word) {
        std::span<const char> span = std::span { data.begin() + word_start, data.end() };
        read_word_big_endian(span, hex_data, is_last_elem_half_byte);
    }

    return hex_data;
}


TEST(ReadingStringAsHex, WholeNumbers) {
    // Whole numbers should be read in big endian
    const char* data = "FF884400";
    std::vector<uint8_t> byte_data = read_string_as_hex(data);

    EXPECT_EQ(byte_data[0],  0x00);
    EXPECT_EQ(byte_data[1],  0x44);
    EXPECT_EQ(byte_data[2],  0x88);
    EXPECT_EQ(byte_data[3],  0xFF);
}

TEST(ReadingStringAsHex, SeparateWords) {
    // Separate words should be read in little endian
    const char* data = "FF 88 44 00";
    std::vector<uint8_t> byte_data = read_string_as_hex(data);

    EXPECT_EQ(byte_data[0],  0xFF);
    EXPECT_EQ(byte_data[1],  0x88);
    EXPECT_EQ(byte_data[2],  0x44);
    EXPECT_EQ(byte_data[3],  0x00);
}

TEST(ReadingStringAsHex, SameAsCppArrays) {
    // Separate words should be read in little endian
    uint32_t data_must_be[] = {
        0xFF997722,
        0x12345678,
        0x98650321,
    };

    const char* data_str = "FF997722 "
        "12345678 "
        "98650321 ";
    std::vector<uint8_t> byte_data_u8 = read_string_as_hex(data_str);
    std::vector<uint32_t>& byte_data_u32 = *reinterpret_cast<std::vector<uint32_t>*>(&byte_data_u8);

    EXPECT_EQ(byte_data_u32[0],  data_must_be[0]);
    EXPECT_EQ(byte_data_u32[1],  data_must_be[1]);
    EXPECT_EQ(byte_data_u32[2],  data_must_be[2]);
}

TEST(TestUtil, VectorAsCharStreambufSeekg) {
    std::vector<uint8_t> byte_data = {
        0x01, 0x44, 0x88, 0xFF
    };
    VectorStream<uint8_t> stream(byte_data);
    // std::istream stream(&buffer);

    // Streams should support seekg
    uint8_t c = 0;
    stream.seekg(0);
    stream.read(reinterpret_cast<char*>(&c), 1);
    EXPECT_EQ(c, 0x01);

    stream.seekg(2);
    stream.read(reinterpret_cast<char*>(&c), 1);
    EXPECT_EQ(c, 0x88);

    stream.seekg(1);
    stream.read(reinterpret_cast<char*>(&c), 1);
    EXPECT_EQ(c, 0x44);

    stream.seekg(3);
    stream.read(reinterpret_cast<char*>(&c), 1);
    EXPECT_EQ(c, 0xFF);
}