#pragma once

#include "Pipeline.h"
#include <unordered_map>
#include <string>

using FilterFactory = std::function<Filter(const std::vector<std::string>&)>;

struct FilterSpec {
    int paramCount;
    FilterFactory factory;
};

std::unordered_map<std::string, FilterSpec> createRegistry();