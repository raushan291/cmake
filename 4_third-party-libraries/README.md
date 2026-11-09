# MNIST: third-party shared libraries

Recognizes handwritten digits using OpenCV for image preprocessing and ONNX Runtime
for inference. Scripts clone and build both dependencies locally from source.
Default tags: OpenCV `4.12.0` and ONNX Runtime `v1.26.0` (see `scripts/versions.sh`).

## Build and run

Run from this folder on Ubuntu Linux. Use a C++17 compiler and CMake 3.28+ for
the dependency builds (the application itself declares a 3.20 minimum).

```bash
sudo apt update
sudo apt install -y build-essential git cmake ninja-build python3 python3-venv python3-pip curl ca-certificates pkg-config libjpeg-dev libpng-dev zlib1g-dev
JOBS=2 ./scripts/build_all.sh
./dist/bin/mnist_cli.exe ./dist/models/mnist-8.onnx ./examples/7.png
```

The first build needs network access and can take significant time and memory.
`build_all.sh` downloads sources and the model, builds dependencies and the app,
runs tests, and installs to `dist/`.

To run the steps separately:

```bash
./scripts/get_sources.sh
JOBS=2 ./scripts/build_third_party.sh
./scripts/get_model.sh
JOBS=2 ./scripts/build_project.sh
```

After dependencies and the model are ready, manual app build/test/install is:

```bash
mkdir build && cd build
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
JOBS="${JOBS:-2}"
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j"${JOBS}"
ctest --output-on-failure
cmake --install . --prefix "${ROOT}/dist"

../dist/bin/mnist_cli.exe ../dist/models/mnist-8.onnx ../examples/7.png
```

Use your own image path instead of `./examples/7.png`; add `--invert` for a dark
digit on a light background. Predictions depend on the image's similarity to MNIST.

## What gets linked

- `mnist_cli.exe` links to application-owned `libmnist_preprocess.so` and `libmnist_inference.so`.
- Preprocessing links to OpenCV; inference links to ONNX Runtime.
- Dependency sources live in `third_party/`; local headers and libraries are under `third_party/install/`.
- CMake finds local OpenCV with `find_package()` and exposes ONNX Runtime as an imported shared-library target. Configuring the app alone does not build these dependencies.
- `dist/` contains `bin/`, `lib/`, `include/`, and `models/`. Third-party shared libraries are bundled by default (`MNIST_BUNDLE_RUNTIME=ON`).

The installed app uses `$ORIGIN`-relative paths to find bundled shared libraries.
System dependencies such as glibc and libstdc++ are still required; retain third-party license notices when distributing.

## Difference

This is the third-party-library step in **1 → 2 → 3 → 4**.
Example 3 builds one application-owned shared library. This example also integrates
libraries from other projects and packages their runtime files.
**Third-party** describes the code's origin, not its linking type: these dependencies are shared.

See the [project comparison and learning order](../README.md).