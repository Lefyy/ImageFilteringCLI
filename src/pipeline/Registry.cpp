#include "../../include/pipeline/Registry.h"
#include "../../include/filters/Filters.h"

std::unordered_map<std::string, FilterSpec> createRegistry() {
    return {
        {"--invert",
            {0, [](auto) {
                return [](const Image& img) {
                    return Filters::invert(img);
                };
            }}
        },

        {"--grayscale", 
            {0, [](auto) {
                return [](const Image& img) {
                    return Filters::grayscale(img);
                };
            }}
        },

        {"--gaussianBlur", 
            {1, [](const std::vector<std::string>& args) {
                int radius = std::stoi(args[0]);
                return [radius](const Image& img) {
                    return Filters::gaussianBlur(img, radius);
                };
            }}
        }
    };
}