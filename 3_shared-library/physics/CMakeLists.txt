cmake_minimum_required(VERSION 3.20)

Project(PhyscisProject VERSION 1.0 LANGUAGES CXX)

# Set the output directory for all binaries (executables and libraries) to be same as build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Add shared library for physics functions
add_library(PhysicsFunctionsLib SHARED src/PhysicsFunctions.cpp)

# Define and export symbols for platforms (need for proper symbol export)
target_compile_definitions(PhysicsFunctionsLib PRIVATE PHYSICS_FUNCTIONS_EXPORTS)

# Include directory for header files
target_include_directories(PhysicsFunctionsLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install library and header files to specified directories (optional, but useful for users of the library)
install(TARGETS PhysicsFunctionsLib
    RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin
    LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
    ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib
)

install(FILES include/PhysicsFunctions.h DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/include")
