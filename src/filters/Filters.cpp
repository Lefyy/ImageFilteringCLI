#include "Filters.h"

#include <cmath>
#include <algorithm>


Image invert(const Image& src) {
    const auto& src_pixels = src.pixels();
    std::vector<unsigned char> out_pixels(src_pixels.size());

    for (size_t i = 0; i < out_pixels.size(); ++i) {
        out_pixels[i] = 255 - src_pixels[i];
    }

    return Image(src.width(), src.height(), src.channels(), std::move(out_pixels));
}

namespace {
    struct BT709 {
        static constexpr float R = 0.2126f;
        static constexpr float G = 0.7152f;
        static constexpr float B = 0.0722f;
    };
}

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

namespace {
    std::vector<float> makeGaussianKernel(int radius) {
        int size = radius * 2 + 1;
        std::vector<float> kernel(size * size);

        float sigma = radius / 2.0f;
        float sum = 0.0f;

        for (int y = -radius; y <= radius; ++y) {
            for (int x = -radius; x <= radius; ++x) {
                float value = std::exp(-(x*x + y*y) / (2 * sigma * sigma));
                kernel[(y+radius)*size + (x+radius)] = value;
                sum += value;
            }
        }

        for (auto& p : kernel) {
            p /= sum;
        }

        return kernel;
    }
}

Image gaussianBlur(const Image& src, int radius) {
    int w = src.width();
    int h = src.height();
    int c = src.channels();

    auto kernel = makeGaussianKernel(radius);
    int size = radius * 2 + 1;

    const auto& src_pixels = src.pixels();
    std::vector<unsigned char> out_pixels(src_pixels.size());

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            for (int ch = 0; ch < c; ++ch) {
                
                float sum = 0.0f;

                for (int ky = -radius; ky <= radius; ++ky) {
                    for (int kx = -radius; kx <= radius; ++kx) {

                        int ix = std::clamp(x + kx, 0, w - 1);
                        int iy = std::clamp(y + ky, 0, h - 1);

                        int img_idx = (iy * w + ix) * c + ch;
                        int k_idx = (ky + radius) * size + (kx + radius);

                        sum += kernel[k_idx] * src_pixels[img_idx];
                    }
                }

                out_pixels[(y * w + x) * c + ch] = static_cast<unsigned char>(sum);
            }
        }
    }

    return Image(w, h, c, std::move(out_pixels));
}