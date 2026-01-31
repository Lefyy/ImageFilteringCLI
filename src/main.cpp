#include "Image.h"
#include "filters/Filters.h"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: ImageFilteringCLI input.png output.png\n";
        return 1;
    }

    try {
        Image img(argv[1]);
        img = grayscale(img);
        img.save(argv[2]);

        std::cout << "Saved to " << argv[2] << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
