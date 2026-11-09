cmake_minimum_required(VERSION 3.20)

project(MathsFunctions VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include direcrtory for the library
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Specify the source files for the static library
set(SOURCES 
    src/MathFunctions.cpp
)

# Add an option to enable or disable advanced math functions
option(ENABLE_ADVANCED_OPERATIONS "Enable advanced math functions" ON)

# Create static liberary target
add_library(MathsFunctionsLib STATIC src/MathsFunctions.cpp)

# Conditally add defenitions based on the option
if(ENABLE_ADVANCED_OPERATIONS)
    target_compile_definitions(MathsFunctionsLib PUBLIC ENABLE_ADVANCED_OPERATIONS)
endif()

# Specify the output directory for the static library and header files
set_target_properties(MathsFunctionsLib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# Install header in the include directory (not needed for building the library, but useful for users of the library)
install(FILES include/MathsFunctions.h DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(MathsFunctionsLib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
