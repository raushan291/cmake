#include "mnist/inference.hpp"

#include <onnxruntime_cxx_api.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <stdexcept>
#include <string>
#include <vector>

namespace mnist {

struct MnistClassifier::Impl {
    Ort::Env env{ORT_LOGGING_LEVEL_WARNING, "mnist"};
    Ort::SessionOptions options;
    Ort::Session session{nullptr};
    std::string input_name;
    std::string output_name;

    explicit Impl(const std::string& model_path) {
        options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_ALL);
        options.SetIntraOpNumThreads(1);
        session = Ort::Session(env, model_path.c_str(), options);

        if (session.GetInputCount() != 1 || session.GetOutputCount() != 1) {
            throw std::runtime_error("Expected a single-input, single-output MNIST model");
        }

        const auto input_type_info = session.GetInputTypeInfo(0);
        const auto in_info = input_type_info.GetTensorTypeAndShapeInfo();
        const auto in_dims = in_info.GetShape();
        constexpr std::array<int64_t, 4> expected{1, 1, 28, 28};
        if (in_info.GetElementType() != ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT ||
            in_dims.size() != expected.size()) {
            throw std::runtime_error("MNIST input must be float32 [1,1,28,28]");
        }
        for (size_t i = 0; i < expected.size(); ++i) {
            if (in_dims[i] > 0 && in_dims[i] != expected[i]) {
                throw std::runtime_error("Unexpected MNIST input dimensions");
            }
        }

        const auto output_type_info = session.GetOutputTypeInfo(0);
const auto out_info = output_type_info.GetTensorTypeAndShapeInfo();
        if (out_info.GetElementType() != ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT) {
            throw std::runtime_error("MNIST output must have float32 scores");
        }

        Ort::AllocatorWithDefaultOptions allocator;
        auto input_alloc = session.GetInputNameAllocated(0, allocator);
        auto output_alloc = session.GetOutputNameAllocated(0, allocator);
        if (!input_alloc || !output_alloc) {
            throw std::runtime_error("Could not retrieve ONNX model input/output names");
        }
        input_name = input_alloc.get();   // Copy: allocator-owned buffers are short-lived.
        output_name = output_alloc.get();
    }
};

MnistClassifier::MnistClassifier(const std::string& model_path)
    : impl_(std::make_unique<Impl>(model_path)) {}

MnistClassifier::~MnistClassifier() = default;

Prediction MnistClassifier::predict(
    const std::array<float, 28 * 28>& pixels) const {
    constexpr std::array<int64_t, 4> input_shape{1, 1, 28, 28};
    Ort::MemoryInfo cpu_info =
        Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);

    // ORT's non-owning input-tensor API takes a mutable buffer; the Run call
    // completes before the local copy goes out of scope.
    auto input_copy = pixels;
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(
        cpu_info, input_copy.data(), input_copy.size(),
        input_shape.data(), input_shape.size());

    const char* input_names[] = {impl_->input_name.c_str()};
    const char* output_names[] = {impl_->output_name.c_str()};
    const auto outputs = impl_->session.Run(
        Ort::RunOptions{nullptr}, input_names, &input_tensor, 1, output_names, 1);
    if (outputs.size() != 1 || !outputs[0].IsTensor()) {
        throw std::runtime_error("Expected one tensor output from MNIST model");
    }

    const auto& output = outputs[0];
    const auto out_info = output.GetTensorTypeAndShapeInfo();
    if (out_info.GetElementType() != ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT ||
        out_info.GetElementCount() != 10) {
        throw std::runtime_error("Expected exactly 10 float32 MNIST scores");
    }
    const float* logits = output.GetTensorData<float>();

    // Model Zoo MNIST outputs logits. Convert to probabilities with stable softmax.
    const float max_logit = *std::max_element(logits, logits + 10);
    std::array<float, 10> probabilities{};
    double total = 0.0;
    for (size_t i = 0; i < probabilities.size(); ++i) {
        probabilities[i] = std::exp(logits[i] - max_logit);
        total += probabilities[i];
    }
    if (!(total > 0.0) || !std::isfinite(total)) {
        throw std::runtime_error("Invalid model output: softmax normalization failed");
    }
    for (float& probability : probabilities) {
        probability = static_cast<float>(probability / total);
    }

    const auto winning = std::max_element(probabilities.begin(), probabilities.end());
    Prediction result;
    result.digit = static_cast<int>(winning - probabilities.begin());
    result.confidence = *winning;
    result.probabilities = probabilities;
    return result;
}

}  // namespace mnist
