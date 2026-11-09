# CMake examples

Four independent C++17 projects. Follow **1 → 2 → 3 → 4**; each link has build and usage details.

| Project | Approach | Main difference |
| --- | --- | --- |
| [1: Direct Compilation](1_direct-compilation/README.md) | Direct compilation | Sources compile into one executable; no separate library. |
| [2: Static Library](2_static-library/README.md) | Static library | Required code from a `.a` library is linked into the executable. |
| [3: Shared Library](3_shared-library/README.md) | Shared/dynamic library | A separate `.so` library is needed at runtime. |
| [4: Third Party Libraries](4_third-party-libraries/README.md) | Third-party libraries | Uses locally built OpenCV and ONNX Runtime shared libraries. |

Third-party means code from another project; it can be static or shared.

## Build

Build each project separately by following its README.

On Linux, examples 1–3 require a C++17 compiler and CMake 3.20+. Example 4 has additional dependencies and setup steps described in its README.
