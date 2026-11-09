#include "mnist/preprocess.hpp"

#include <cassert>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <string>

int main() {
    const std::string path = "preprocess_test_image.pgm";
    {
        std::ofstream out(path, std::ios::binary);
        assert(out.good());
        out << "P5\n28 28\n255\n";
        for (int i = 0; i < 28 * 28; ++i) {
            const unsigned char pixel = (i == 0) ? 255 : 0;
            out.write(reinterpret_cast<const char*>(&pixel), 1);
        }
    }
    const auto input = mnist::preprocess_image(path);
    const auto inverted = mnist::preprocess_image(path, true);
    assert(input.size() == 28 * 28);
    assert(std::abs(input[0] - 1.0f) < 0.0001f);
    assert(std::abs(input[1] - 0.0f) < 0.0001f);
    assert(std::abs(inverted[0] - 0.0f) < 0.0001f);
    assert(std::abs(inverted[1] - 1.0f) < 0.0001f);
    std::remove(path.c_str());
    return 0;
}
