#include "../../include/pipeline/Registry.h"
#include "../../include/filters/Filters.h"

std::unordered_map<std::string, FilterSpec> createRegistry() {
    return {
        {"--invert",
            {0, [](auto) {
                return [](ImagePtr img) {
                    return Filters::invert(std::move(img));
                };
            }}
        },

        {"--grayscale", 
            {0, [](auto) {
                return [](ImagePtr img) {
                    return Filters::grayscale(std::move(img));
                };
            }}
        },

        {"--gaussianBlur", 
            {1, [](const std::vector<std::string>& args) {
                int radius = std::stoi(args[0]);
                return [radius](ImagePtr img) {
                    return Filters::gaussianBlur(std::move(img), radius);
                };
            }}
        }
    };
}