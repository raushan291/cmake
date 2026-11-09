# CMake examples

Four independent C++17 projects. Follow **1 → 2 → 3 → 4**; each link has build and usage details.

| Project | Approach | Main difference |
| --- | --- | --- |
| [1: Calculator](1_calculator/README.md) | Direct compilation | Sources compile into one executable; no separate library. |
| [2: Maths calculator](2_calculator-maths/README.md) | Static library | Required code from a `.a` library is linked into the executable. |
| [3: Physics](3_physics/README.md) | Shared/dynamic library | A separate `.so` library is needed at runtime. |
| [4: MNIST](4_third-party-libs/README.md) | Third-party libraries | Uses locally built OpenCV and ONNX Runtime shared libraries. |

Third-party means code from another project; it can be static or shared.

## Build

On Linux, examples 1–3 need a C++17 compiler and CMake 3.20+.
From this repository root, build the calculator:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./cal.exe
```

Build each project separately. For example 4, follow its [setup instructions](4_third-party-libs/README.md#build-and-run);
it needs CMake 3.28+, extra tools, and network access to build dependencies.
