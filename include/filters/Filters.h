#pragma once

#include "../Image.h"

namespace Filters {
    Image invert(const Image& src);
    Image grayscale(const Image& src);
    Image gaussianBlur(const Image& src, int radius, bool parallel = true, int numThreads = 0);
}