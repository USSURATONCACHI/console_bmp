#pragma once

#include <cstdint>

namespace bmp_reader {

struct Rgba8Pixel {
    uint8_t r, g, b, a;
};

struct Bgra8Pixel {
    uint8_t b, g, r, a;
};

struct Cmyk8Pixel {
    uint8_t c, m, y, k;
};


static inline auto cmyk_to_rgb(Cmyk8Pixel p) -> Rgba8Pixel {
    return Rgba8Pixel {
        .r = static_cast<uint8_t>(255 * (1 - p.c) * (1 - p.k)),
        .g = static_cast<uint8_t>(255 * (1 - p.m) * (1 - p.k)),
        .b = static_cast<uint8_t>(255 * (1 - p.y) * (1 - p.k)),

        .a = 255
    };
}

static inline auto bgra_to_rgba(Bgra8Pixel p) -> Rgba8Pixel {
    return Rgba8Pixel {
        .r = p.r,
        .g = p.g,
        .b = p.b,
        .a = p.a,
    };
}

} // namespace bmp_reader