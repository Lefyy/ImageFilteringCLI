#include "../../include/pipeline/Pipeline.h"

ImagePtr runPipeline(ImagePtr input, const std::vector<Filter>& filters) {

    for (const auto& f : filters) {
        input = f(std::move(input));
    }

    return input;
}