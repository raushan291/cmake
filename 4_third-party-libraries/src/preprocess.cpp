#include "mnist/preprocess.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <stdexcept>

namespace mnist {

std::array<float, 28 * 28> preprocess_image(const std::string& image_path,
                                          bool invert) {
    const cv::Mat gray = cv::imread(image_path, cv::IMREAD_GRAYSCALE);
    if (gray.empty()) {
        throw std::runtime_error("Cannot read image: " + image_path);
    }

    cv::Mat resized;
    cv::resize(gray, resized, cv::Size(28, 28), 0, 0, cv::INTER_AREA);
    if (invert) {
        cv::bitwise_not(resized, resized);
    }

    cv::Mat normalized;
    resized.convertTo(normalized, CV_32FC1, 1.0 / 255.0);

    // One channel and one image: flattened H*W is already NCHW [1,1,28,28].
    std::array<float, 28 * 28> pixels{};
    if (normalized.isContinuous()) {
        std::copy_n(normalized.ptr<float>(0), pixels.size(), pixels.begin());
    } else {
        for (int y = 0; y < normalized.rows; ++y) {
            std::copy_n(normalized.ptr<float>(y), 28, pixels.begin() + y * 28);
        }
    }
    return pixels;
}

}  // namespace mnist
