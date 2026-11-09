# Physics: shared/dynamic library

Calculates force, kinetic energy, and potential energy.
The parent CMake file uses `add_subdirectory(physics)`; the child defines
`PhysicsFunctionsLib` with `add_library(... SHARED ...)`. `PhysicsExe` links to it.

## Build and run

From this folder on Linux, with CMake 3.20+ and a C++17 compiler:

```bash
mkdir build && cd build
cmake ..
cmake --build .
./physics-shared-library.exe
```

Select `1`, enter mass `2` and acceleration `3` to get `Result: 6 N`.
Outputs: `build/PhysicsExe` and `build/libPhysicsFunctionsLib.so`.
CMake supplies a runtime search path for running from the build directory.

## Difference

This is the shared-library step in **1 → 2 → 3 → 4**.
Unlike example 2, the library stays separate and must be available at runtime.
If distributing the executable, include the `.so` and configure a runtime search path.
Example 4 adds third-party dependencies and runtime packaging.

See the [project comparison and learning order](../README.md).
