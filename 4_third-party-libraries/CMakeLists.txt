cmake_minimum_required(VERSION 3.20)
project(mnist_cpp_from_source VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_BUILD_RPATH_USE_ORIGIN TRUE)

# Executable in dist/bin uses ../lib; libraries in dist/lib use their own folder.
set(CMAKE_INSTALL_RPATH "$ORIGIN/../lib;$ORIGIN")
include(GNUInstallDirs)

option(MNIST_BUNDLE_RUNTIME "Package locally compiled third-party .so files" ON)

# We *intentionally* never search the system for OpenCV. Only local source build.
set(OPENCV_LOCAL_PREFIX "${CMAKE_SOURCE_DIR}/third_party/install/opencv"
    CACHE PATH "Local OpenCV source-build install prefix")
set(_opencv_package "${OPENCV_LOCAL_PREFIX}/lib/cmake/opencv4")
find_package(OpenCV 4 CONFIG REQUIRED COMPONENTS core imgproc imgcodecs
    PATHS "${_opencv_package}" NO_DEFAULT_PATH)
message(STATUS "Using locally built OpenCV: ${OpenCV_DIR}")

# C/C++ headers and .so from our local ONNX Runtime source build.
set(ONNXRUNTIME_ROOT "${CMAKE_SOURCE_DIR}/third_party/install/onnxruntime"
    CACHE PATH "Local ONNX Runtime source-build install prefix")
find_path(ONNXRUNTIME_INCLUDE_DIR NAMES onnxruntime_cxx_api.h
    PATHS "${ONNXRUNTIME_ROOT}/include" NO_DEFAULT_PATH REQUIRED)
find_library(ONNXRUNTIME_LIBRARY NAMES onnxruntime
    PATHS "${ONNXRUNTIME_ROOT}/lib" NO_DEFAULT_PATH REQUIRED)
add_library(onnxruntime::onnxruntime SHARED IMPORTED GLOBAL)
set_target_properties(onnxruntime::onnxruntime PROPERTIES
    IMPORTED_LOCATION "${ONNXRUNTIME_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${ONNXRUNTIME_INCLUDE_DIR}")
message(STATUS "Using locally built ONNX Runtime: ${ONNXRUNTIME_LIBRARY}")

# Your two reusable application-owned shared libraries.
add_library(mnist_preprocess SHARED src/preprocess.cpp)
target_include_directories(mnist_preprocess PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories(mnist_preprocess PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(mnist_preprocess PRIVATE ${OpenCV_LIBS})

add_library(mnist_inference SHARED src/inference.cpp)
target_include_directories(mnist_inference PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(mnist_inference PRIVATE onnxruntime::onnxruntime)

add_executable(mnist_cli.exe src/main.cpp)
target_link_libraries(mnist_cli.exe PRIVATE mnist_preprocess mnist_inference)

install(TARGETS mnist_cli.exe mnist_preprocess mnist_inference
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(DIRECTORY models/ DESTINATION models FILES_MATCHING PATTERN "*.onnx")

if(MNIST_BUNDLE_RUNTIME)
    # Include versioned .so and symlinks from the locally installed source builds.
    file(GLOB _ort_dsos "${ONNXRUNTIME_ROOT}/lib/libonnxruntime*.so*")
    if(NOT _ort_dsos)
        message(FATAL_ERROR "ONNX Runtime shared libraries missing from ${ONNXRUNTIME_ROOT}/lib")
    endif()
    install(FILES ${_ort_dsos} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    foreach(_component core imgproc imgcodecs)
        file(GLOB _opencv_dsos "${OPENCV_LOCAL_PREFIX}/lib/libopencv_${_component}.so*")
        if(NOT _opencv_dsos)
            message(FATAL_ERROR "OpenCV shared library missing: ${_component}")
        endif()
        install(FILES ${_opencv_dsos} DESTINATION ${CMAKE_INSTALL_LIBDIR})
    endforeach()
endif()

include(CTest)
if(BUILD_TESTING)
    add_executable(preprocess_test tests/preprocess_test.cpp)
    target_link_libraries(preprocess_test PRIVATE mnist_preprocess)
    add_test(NAME preprocess_smoke COMMAND preprocess_test)
endif()
