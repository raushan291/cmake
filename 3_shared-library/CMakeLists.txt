cmake_minimum_required(VERSION 3.20)

# Set the project name and version
project(PhysicsProject VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard and ensure it's required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
)

# include directories for the library
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(physics)

# Add executable
add_executable(physics-shared-library.exe ${SOURCES})

message("Executable output path: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("Shared library output path: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

# Target link libraries
target_link_libraries(physics-shared-library.exe PhysicsFunctionsLib)
