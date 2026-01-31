#include "Filters.h"


Image invert(const Image& src) {
    const auto& src_pixels = src.pixels();
    std::vector<unsigned char> out_pixels(src_pixels.size());

    for (size_t i = 0; i < out_pixels.size(); ++i) {
        out_pixels[i] = 255 - src_pixels[i];
    }

    return Image(src.width(), src.height(), src.channels(), std::move(out_pixels));
}

struct BT601 {
    static constexpr float R = 0.299f;
    static constexpr float G = 0.587f;
    static constexpr float B = 0.114f;
};

Image grayscale(const Image& src) {
    int src_c = src.channels();
    const auto& src_pixels = src.pixels();

    if (src_c < 3) return src;

    int out_c = 1;
    std::vector<unsigned char> out_pixels(src.width() * src.height());

    for (size_t i = 0; i < src_pixels.size(); i += src_c) {

        unsigned char gray = static_cast<unsigned char>(
            BT601::R*src_pixels[i] + BT601::G*src_pixels[i+1] + BT601::B*src_pixels[i+2]
        );

        out_pixels[i/src_c] = gray;
    }

    return Image(src.width(), src.height(), out_c, std::move(out_pixels));
}