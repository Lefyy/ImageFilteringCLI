#include "../include/Image.h"
#include "../include/pipeline/Pipeline.h"
#include "../include/pipeline/Registry.h"
#include <iostream>

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

int main(int argc, char** argv) {
    if (argc <= 3) {
        std::cout << "Usage: ImageFilteringCLI input.png output.png\n";
        return 1;
    }

    try {
        Image img(argv[1]);
        std::string out = argv[2];

        auto registry = createRegistry();
        std::vector<Filter> pipeline;

        for (int i = 3; i < argc; ++i) {
            pipeline.push_back(parseFilter(argv, i, registry));
        }

        Image result = runPipeline(img, pipeline);
        result.save(out);

        std::cout << "Saved to " << out << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
