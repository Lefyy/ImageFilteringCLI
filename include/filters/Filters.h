#pragma once

#include "../Image.h"
#include <memory>

using ImagePtr = std::unique_ptr<Image>;

namespace Filters {
    ImagePtr invert(ImagePtr src);
    ImagePtr grayscale(ImagePtr src);
    ImagePtr gaussianBlur(ImagePtr src, int radius, bool parallel = true, int numThreads = 0);
}