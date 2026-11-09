# Maths calculator: static library in a subdirectory

A calculator for arithmetic, square root, and power.
The parent CMake file uses `add_subdirectory(maths)`; the child defines the static
`MathsFunctionsLib` library, which is linked to `CalculatorMathsExe`.

## Build and run

From this folder on Linux, with CMake 3.20+ and a C++17 compiler:

```bash
mkdir build && cd build
cmake ..
cmake --build .
cal-static-library.exe
```

Select `6`, then enter `2` and `3` to get `Result: 8`.
Outputs: `build/CalculatorMathsExe` and `maths/lib/libMathsFunctionsLib.a`.
The current CMake configuration writes the archive into the source tree.

Square root and power are enabled by default. To disable them, configure with:

```bash
mkdir build && cd build
cmake -DENABLE_ADVANCED_OPERATIONS=ON ..
cmake --build .
./cal-static-library.exe
```

## Difference

This is the static-library step in **1 → 2 → 3 → 4**.
Unlike example 1, it builds a separate library target with its own CMake file.
Required library code is linked into the executable; the `.a` is not needed at runtime.
Example 3 keeps the library separate at runtime instead.

See the [project comparison and learning order](../README.md).
