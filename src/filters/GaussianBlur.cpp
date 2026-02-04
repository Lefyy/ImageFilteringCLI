#include "../../include/Image.h"

#include <cmath>
#include <thread>
#include <algorithm>

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

    void convolveHorizontal(
        const std::vector<unsigned char>& src,
        std::vector<float>& dst,
        int w, int h, int c,
        const std::vector<float>& kernel,
        int radius,
        int y0, int y1
    ) {
        for (int y = y0; y < y1; ++y) {
            for (int x = 0; x < w; ++x) {
                for (int ch = 0; ch < c; ++ch) {
                    float sum = 0.0f;
                    for (int k = -radius; k <= radius; ++k) {
                        int ix = reflect(x + k, w - 1);
                        sum += kernel[k + radius] * src[(y * w + ix) * c + ch];
                    }
                    dst[(y * w + x) * c + ch] = sum;
                }
            }
        }
    }

    void convolveVertical(
        const std::vector<float>& src,
        std::vector<unsigned char>& dst,
        int w, int h, int c,
        const std::vector<float>& kernel,
        int radius,
        int y0, int y1
    ) {
        for (int y = y0; y < y1; ++y) {
            for (int x = 0; x < w; ++x) {
                for (int ch = 0; ch < c; ++ch) {
                    float sum = 0.0f;
                    for (int k = -radius; k <= radius; ++k) {
                        int iy = reflect(y + k, h - 1);
                        sum += kernel[k + radius] * src[(iy * w + x) * c + ch];
                    }
                    dst[(y * w + x) * c + ch] = static_cast<unsigned char>(
                        std::clamp(sum + 0.5f, 0.0f, 255.0f)
                    );
                }
            }
        }
    }

    template<typename Func>
    void runThreads(int numThreads, int h, Func func) {
        if (numThreads <= 1) {
            func(0, h);
            return;
        }

        std::vector<std::thread> threads;
        int slice = h / numThreads;

        for (int i = 0; i < numThreads; ++i) {
            int y0 = i * slice;
            int y1 = (i == numThreads - 1) ? h : y0 + slice;
            threads.emplace_back(func, y0, y1);
        }

        for (auto& t : threads) t.join();
    }
}

Image gaussianBlurSingle(const Image& src, int radius) {
    int w = src.width();
    int h = src.height();
    int c = src.channels();

    auto kernel = makeGaussianKernel1D(radius);
    const auto& src_pixels = src.pixels();

    std::vector<float> temp(w * h * c);
    std::vector<unsigned char> out(w * h * c);

    convolveHorizontal(src_pixels, temp, w, h, c, kernel, radius, 0, h);
    convolveVertical(temp, out, w, h, c, kernel, radius, 0, h);

    return Image(w, h, c, std::move(out));
}

Image gaussianBlurParallel(const Image& src, int radius, int numThreads) {
    int w = src.width();
    int h = src.height();
    int c = src.channels();

    auto kernel = makeGaussianKernel1D(radius);
    const auto& src_pixels = src.pixels();

    std::vector<float> temp(w * h * c);
    std::vector<unsigned char> out(w * h * c);

    runThreads(numThreads, h, [&](int y0, int y1) {
        convolveHorizontal(src_pixels, temp, w, h, c, kernel, radius, y0, y1);
    });

    runThreads(numThreads, h, [&](int y0, int y1) {
        convolveVertical(temp, out, w, h, c, kernel, radius, y0, y1);
    });

    return Image(w, h, c, std::move(out));
}

namespace Filters {
    Image gaussianBlur(const Image& src, int radius, bool parallel, int numThreads) {
        if (!parallel) {
            return gaussianBlurSingle(src, radius);
        }

        if (numThreads <= 0) {
            numThreads = std::thread::hardware_concurrency();
            if (numThreads <= 0) numThreads = 4;
        }

        return gaussianBlurParallel(src, radius, numThreads);
    }
}