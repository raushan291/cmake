#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
JOBS="${JOBS:-2}"

# Create and enter build directory
mkdir -p "${ROOT}/build"
cd "${ROOT}/build"

# Configure
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compile
cmake --build . -j"${JOBS}"

# Run tests
ctest --output-on-failure

# Install executable and shared libraries
cmake --install . --prefix "${ROOT}/dist"

echo "Build completed successfully!"
echo "Run: ./dist/bin/mnist_cli.exe ./dist/models/mnist-8.onnx ./examples/7.png"
