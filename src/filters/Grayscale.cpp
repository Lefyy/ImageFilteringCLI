#include "../../include/Image.h"

namespace {
    struct BT709 {
        static constexpr float R = 0.2126f;
        static constexpr float G = 0.7152f;
        static constexpr float B = 0.0722f;
    };
}

namespace Filters {
    Image grayscale(const Image& src) {
        int src_c = src.channels();
        const auto& src_pixels = src.pixels();

        if (src_c < 3) return src;

        int out_c = 1;
        std::vector<unsigned char> out_pixels(src.width() * src.height());

        for (size_t i = 0; i < src_pixels.size(); i += src_c) {

            unsigned char gray = static_cast<unsigned char>(
                BT709::R*src_pixels[i] + BT709::G*src_pixels[i+1] + BT709::B*src_pixels[i+2]
            );

            out_pixels[i/src_c] = gray;
        }

        return Image(src.width(), src.height(), out_c, std::move(out_pixels));
    }
}