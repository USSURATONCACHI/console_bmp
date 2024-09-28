#pragma once

#include <cstdint>
#include <cstddef>

namespace bmp_reader {

struct BitView {
    uint8_t* data_ptr;
    size_t bits_offset;

    void read_into(size_t size_bits, size_t offset_bits, void* out_data) const;
    void write_from(size_t size_bits, size_t offset_bits, const void* in_data) const;

    template<typename As>
    inline auto read_as(size_t offset_bits, size_t max_bits = sizeof(As) * 8) const -> As {
        As as = {};
        read_into(max_bits, offset_bits, reinterpret_cast<void*>(&as));
        return as;
    }

    template<typename As>
    inline auto write_as(size_t offset_bits, As as, size_t max_bits = sizeof(As) * 8) const {
        write_from(max_bits, offset_bits, reinterpret_cast<void*>(&as));
    }

    inline auto subview(size_t bitshift) -> BitView {
        return BitView {
            .data_ptr = data_ptr,
            .bits_offset = bits_offset + bitshift,
        };
    }
};

} // namespace bmp_reader