# Calculator: direct compilation

An interactive calculator for `+`, `-`, `*`, and `/`.
CMake compiles `src/main.cpp` and `calculator/src/Calculator.cpp` into `cal.exe`.
The separate calculator folder does not create a library target.

## Build and run

From this folder on Linux, with CMake 3.20+ and a C++17 compiler:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./cal-direct-compilation.exe
```

Enter `2`, `+`, and `3` at the prompts to get `Result: 5`.
The `.exe` suffix is part of the target name, even on Linux.

## Difference

This is the direct-compilation step in **1 → 2 → 3 → 4**.
The implementation sources belong to the executable target. Example 2 introduces
a separate static-library target for reusable code.

See the [project comparison and learning order](../README.md).
