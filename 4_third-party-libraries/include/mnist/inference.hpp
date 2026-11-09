#pragma once

#include <array>
#include <memory>
#include <string>

namespace mnist {

struct Prediction {
    int digit = -1;
    float confidence = 0.0f;
    std::array<float, 10> probabilities{};
};

// Public header deliberately does not expose ONNX Runtime headers.
class MnistClassifier {
public:
    explicit MnistClassifier(const std::string& model_path);
    ~MnistClassifier();

    MnistClassifier(const MnistClassifier&) = delete;
    MnistClassifier& operator=(const MnistClassifier&) = delete;

    Prediction predict(const std::array<float, 28 * 28>& pixels) const;

private:
    struct Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace mnist
