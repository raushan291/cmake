#include "mnist/inference.hpp"
#include "mnist/preprocess.hpp"

#include <iomanip>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 4 || (argc == 4 && std::string(argv[3]) != "--invert")) {
        std::cerr << "Usage: " << argv[0]
                  << " <model.onnx> [image.png] [--invert]\n"
                  << "Omit the image path to enter it interactively.\n"
                  << "Default: white digit on a black background.\n";
        return 2;
    }

    const std::string model_path = argv[1];
    std::string image_path;
    if (argc >= 3) {
        image_path = argv[2];
    } else {
        std::cout << "Enter image path: ";
        std::getline(std::cin, image_path);
    }
    if (image_path.empty()) {
        std::cerr << "ERROR: image path cannot be empty\n";
        return 2;
    }
    const bool invert = argc == 4;

    try {
        const auto input = mnist::preprocess_image(image_path, invert);
        const mnist::MnistClassifier classifier(model_path);
        const mnist::Prediction p = classifier.predict(input);

        std::cout << "Predicted digit: " << p.digit << '\n';
        std::cout << "Confidence: " << std::fixed << std::setprecision(2)
                  << (p.confidence * 100.0f) << "%\n";
        std::cout << "Probabilities:\n";
        for (size_t i = 0; i < p.probabilities.size(); ++i) {
            std::cout << "  " << i << ": " << std::setprecision(4)
                      << p.probabilities[i] << '\n';
        }
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << '\n';
        return 1;
    }
}
