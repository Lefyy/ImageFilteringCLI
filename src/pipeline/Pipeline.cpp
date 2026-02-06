#include "../../include/pipeline/Pipeline.h"

Image runPipeline(const Image& input, const std::vector<Filter>& filters) {
    Image current = input;

    for (const auto& f : filters) {
        current = f(current);
    }

    return current;
}