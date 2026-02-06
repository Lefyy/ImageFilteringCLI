#pragma once

#include "../Image.h"
#include <functional>
#include <vector>
#include <memory>

using ImagePtr = std::unique_ptr<Image>;
using Filter = std::function<ImagePtr(ImagePtr)>;

ImagePtr runPipeline(ImagePtr input, const std::vector<Filter>& filters);