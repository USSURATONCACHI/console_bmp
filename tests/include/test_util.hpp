#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <span>
#include <iostream>
#include <istream>
#include <streambuf>


template<typename T>
class VectorStreamBuffer : public std::streambuf {
public:
    VectorStreamBuffer(std::vector<T>& vec) {
        char* begin = reinterpret_cast<char*>(vec.data());
        setg(begin, begin, begin + vec.size() * sizeof(T));
    }

protected:
    // Override seekoff to provide seekg functionality
    std::streampos seekoff(std::streamoff off, std::ios_base::seekdir dir, std::ios_base::openmode = std::ios_base::in) override {
        if (dir == std::ios_base::beg) {
            setg(eback(), eback() + off, egptr());
        } else if (dir == std::ios_base::cur) {
            setg(eback(), gptr() + off, egptr());
        } else if (dir == std::ios_base::end) {
            setg(eback(), egptr() + off, egptr());
        }
        return gptr() - eback();
    }

    // Override seekpos to provide tellg functionality
    std::streampos seekpos(std::streampos pos, std::ios_base::openmode which = std::ios_base::in) override {
        return seekoff(pos, std::ios_base::beg, which);
    }
};

template<typename T>
class VectorStream : public std::basic_istream<char> {
public:
    VectorStream(std::vector<T>& vec) 
        : std::basic_istream<char>(&buffer), buffer(vec) {}

private:
    VectorStreamBuffer<T> buffer;
};


auto is_hex_symbol(char c) -> bool;
auto symbol_to_hex_val(char c) -> uint8_t;

void push_half_byte(std::vector<uint8_t>& push_into, bool& is_last_elem_half_byte, uint8_t value);

void read_word_big_endian(std::span<char> word, std::vector<uint8_t>& push_into, bool& is_last_elem_half_byte);
auto read_string_as_hex(const std::string& data) -> std::vector<uint8_t>;