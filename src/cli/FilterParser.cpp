#include "../../include/cli/FilterParser.h"
#include <stdexcept>

Filter parseFilter(char** argv, int& i, const std::unordered_map<std::string, FilterSpec>& registry) {
    std::string key = argv[i];

    auto it = registry.find(key);
    if (it == registry.end()) {
        throw std::runtime_error("Unknown filter " + key);
    }

    const FilterSpec& spec = it->second;

    std::vector<std::string> params;

    for (int p = 0; p < spec.paramCount; ++p) {
        params.push_back(argv[++i]);
    }

    return spec.factory(params);
}