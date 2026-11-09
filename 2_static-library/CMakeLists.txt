cmake_minimum_required(VERSION 3.20)

# Set the project name and version
project(CalculatorMaths VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard and ensure it's required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the subdirectories
add_subdirectory(maths)

# Add the executable target for the main application
add_executable(cal-static-library.exe src/main.cpp)

# Link the calculator library with the main executable
target_link_libraries(cal-static-library.exe PRIVATE MathsFunctionsLib)
