cmake_minimum_required(VERSION 3.20)

# Set the project name and version
project(Calculator VERSION 1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directories
include_directories(${PROJECT_SOURCE_DIR}/calculator/include)

# Add the source files
set(SOURCES 
    src/main.cpp 
    calculator/src/Calculator.cpp
)

# Add an executable
add_executable(cal-direct-compilation.exe ${SOURCES})
