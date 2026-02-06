#pragma once

#include "../pipeline/Pipeline.h"
#include "../pipeline/Registry.h"
#include <unordered_map>

Filter parseFilter(char** argv, int& i, const std::unordered_map<std::string, FilterSpec>& registry);