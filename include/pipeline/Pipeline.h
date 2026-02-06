#pragma once

#include "../Image.h"
#include <functional>
#include <vector>

using Filter = std::function<Image(const Image&)>;

Image runPipeline(const Image& input, const std::vector<Filter>& filters);