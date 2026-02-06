#include "../include/cli/FilterParser.h"
#include "../include/Image.h"
#include "../include/pipeline/Pipeline.h"
#include "../include/pipeline/Registry.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc <= 3) {
        std::cout << "Usage: ImageFilteringCLI input.png output.png\n";
        return 1;
    }

    try {
        auto img = std::make_unique<Image>(argv[1]);

        auto registry = createRegistry();
        std::vector<Filter> pipeline;

        for (int i = 3; i < argc; ++i) {
            pipeline.push_back(parseFilter(argv, i, registry));
        }

        auto result = runPipeline(std::move(img), pipeline);
        result->save(argv[2]);

        std::cout << "Saved to " << argv[2] << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
