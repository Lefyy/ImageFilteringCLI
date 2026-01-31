#include "Filters.h"


Image invert(const Image& src) {
    const auto& old_pixels = src.pixels();
    std::vector<unsigned char> new_pixels(old_pixels.size());

    for (size_t i = 0; i < new_pixels.size(); ++i) {
        new_pixels[i] = 255 - old_pixels[i];
    }

    return Image(src.width(), src.height(), src.channels(), std::move(new_pixels));
}