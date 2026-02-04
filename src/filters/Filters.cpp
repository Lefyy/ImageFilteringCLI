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
    std::vector<float> makeGaussianKernel1D(int radius) {
        int size = radius * 2 + 1;
        std::vector<float> kernel(size);

        float sigma = radius / 2.0f;
        float sum = 0.0f;

        for (int i = -radius; i <= radius; ++i) {
            float value = std::exp(-(i*i) / (2 * sigma * sigma));
            kernel[i+radius] = value;
            sum += value;
        }

        for (auto& v : kernel) {
            v /= sum;
        }

        return kernel;
    }

    int reflect(int p, int max) {
        if (p < 0) return -p;
        if (p > max) return 2*max - p - 2;
        return p;
    }
}

Image gaussianBlur(const Image& src, int radius) {
    int w = src.width();
    int h = src.height();
    int c = src.channels();

    auto kernel = makeGaussianKernel1D(radius);
    int size = radius * 2 + 1;

    const auto& src_pixels = src.pixels();

    std::vector<float> temp(w * h * c);
    std::vector<unsigned char> out_pixels(w * h * c);

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            for (int ch = 0; ch < c; ++ch) {
                
                float sum = 0.0f;

                for (int k = -radius; k <= radius; ++k) {
                    int ix = reflect(x + k, w - 1);
                    int idx = (y * w + ix) * c + ch;

                    sum += kernel[k + radius] * src_pixels[idx];
                }

                temp[(y * w + x) * c + ch] = sum;
            }
        }
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            for (int ch = 0; ch < c; ++ch) {
                
                float sum = 0.0f;

                for (int k = -radius; k <= radius; ++k) {
                    int iy = reflect(y + k, h - 1);
                    int idx = (iy * w + x) * c + ch;

                    sum += kernel[k + radius] * temp[idx];
                }

                out_pixels[(y * w + x) * c + ch] = static_cast<unsigned char>(std::clamp(sum + 0.5f, 0.0f, 255.0f));
            }
        }
    }

    return Image(w, h, c, std::move(out_pixels));
}