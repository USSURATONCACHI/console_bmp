#include <bmp_reader/util/bit_view.hpp>

#include <cstdint>
#include <limits>

namespace bmp_reader {


template<typename T>
static auto read_uint_shifted(T* read_from, size_t bit_offset) -> T;

template<typename T>
static void write_uint_shifted(T* write_to, size_t bit_offset, T data);


BitView::BitView(void* data, size_t bitshift)
    : data_ptr(reinterpret_cast<uint8_t*>(data)),
    bits_offset(bitshift)
{}

BitView::BitView(void* data)
    : BitView(data, 0)
{}

void BitView::read_into(size_t size_bits, size_t offset_bits, void* out_data) const{
    size_t pos_bits = this->bits_offset + offset_bits;
    size_t pos_bytes = pos_bits / 8;
    size_t rem_bits = pos_bits % 8;

    size_t i = 0;
    uint8_t *write_ptr = reinterpret_cast<uint8_t*>(out_data);
    while (size_bits >= 8) {
        write_ptr[i] = read_uint_shifted<uint8_t>(&data_ptr[pos_bytes + i], rem_bits);
        size_bits -= 8;
        i++;
    }

    if (size_bits > 0) {
        write_ptr[i] = read_uint_shifted<uint8_t>(&data_ptr[pos_bytes + i], rem_bits) & (std::numeric_limits<uint8_t>::max() >> (8 - size_bits));
    }
}

void BitView::write_from(size_t size_bits, size_t offset_bits, const void* in_data) const {
    size_t pos_bits = this->bits_offset + offset_bits;
    size_t pos_bytes = pos_bits / 8;
    size_t rem_bits = pos_bits % 8;

    size_t i = 0;
    const uint8_t *read_ptr = reinterpret_cast<const uint8_t*>(in_data);
    while (size_bits >= 8) {
        write_uint_shifted(&data_ptr[pos_bytes + i], rem_bits, read_ptr[i]);
        size_bits -= 8;
        i++;
    }

    if (size_bits > 0) {
        uint8_t read_last = read_uint_shifted<uint8_t>(&data_ptr[pos_bytes + i], rem_bits);
        uint8_t bitmask = std::numeric_limits<uint8_t>::max() >> (8 - size_bits);
        read_last &= ~bitmask;
        read_last |= (read_ptr[i] >> (8 - size_bits)) & bitmask;
        write_uint_shifted(&data_ptr[pos_bytes + i], rem_bits, read_last);
    }
}


// Helper functions
template<typename T>
static auto read_uint_shifted(T* read_from, size_t bit_offset) -> T {
    size_t byte_offset = bit_offset / (sizeof(T) * 8);
    bit_offset = bit_offset % (sizeof(T) * 8);

    T result = 0;
    result |= read_from[byte_offset] >> bit_offset;

    if (bit_offset != 0) {
        const size_t shift = ((sizeof(T) * 8) - bit_offset);
        const T bitmask = std::numeric_limits<T>::max() >> shift;
        result |= (read_from[byte_offset + 1] & bitmask) << shift;
    }

    return result;
}

template<typename T>
static void write_uint_shifted(T* write_to, size_t bit_offset, T data) {
    size_t byte_offset = bit_offset / 8;
    bit_offset = bit_offset % 8;

    write_to[byte_offset] &= ~(std::numeric_limits<T>::max() << bit_offset);
    write_to[byte_offset] |= data << bit_offset;

    if (bit_offset != 0) {
        write_to[byte_offset + 1] &= ~(std::numeric_limits<T>::max() >> ((sizeof(T) * 8) - bit_offset));
        write_to[byte_offset + 1] |= data >> ((sizeof(T) * 8) - bit_offset);
    }
}


} // namespace bmp_reader