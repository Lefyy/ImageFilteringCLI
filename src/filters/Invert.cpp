#include "../../include/filters/Filters.h"

namespace Filters {
    ImagePtr invert(ImagePtr src) {
        const auto& src_pixels = src->pixels();
        std::vector<unsigned char> out_pixels(src_pixels.size());

        for (size_t i = 0; i < out_pixels.size(); ++i) {
            out_pixels[i] = 255 - src_pixels[i];
        }

        return std::make_unique<Image>(src->width(), src->height(), src->channels(), std::move(out_pixels));
    }
}