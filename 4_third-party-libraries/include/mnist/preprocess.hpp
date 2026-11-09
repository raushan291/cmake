#pragma once

#include <array>
#include <string>

namespace mnist {

// Converts a path into grayscale, NCHW, float32 data with shape [1, 1, 28, 28].
// Set invert=true for a black digit on white background.
std::array<float, 28 * 28> preprocess_image(const std::string& image_path,
                                          bool invert = false);

}  // namespace mnist
