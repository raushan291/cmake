#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
JOBS="${JOBS:-2}"  # ONNX Runtime can require considerable RAM.
[[ "$JOBS" =~ ^[1-9][0-9]*$ ]] || { echo 'JOBS must be a positive integer' >&2; exit 1; }
OPENCV_SRC="${ROOT}/third_party/opencv"
ORT_SRC="${ROOT}/third_party/onnxruntime"
OPENCV_BUILD="${ROOT}/third_party/build/opencv"
ORT_BUILD="${ROOT}/third_party/build/onnxruntime"
OPENCV_INSTALL="${ROOT}/third_party/install/opencv"
ORT_INSTALL="${ROOT}/third_party/install/onnxruntime"

[[ -f "${OPENCV_SRC}/CMakeLists.txt" && -f "${ORT_SRC}/build.sh" ]] || {
    echo 'Missing Git sources. Run ./scripts/get_sources.sh first.' >&2
    exit 1
}

mkdir -p "${OPENCV_BUILD}" "${ORT_BUILD}" "${OPENCV_INSTALL}" "${ORT_INSTALL}/include" "${ORT_INSTALL}/lib"

echo '============================================================'
echo 'Building OpenCV shared libraries from local GitHub sources'
echo '============================================================'
cmake -S "${OPENCV_SRC}" -B "${OPENCV_BUILD}" -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="${OPENCV_INSTALL}" \
    -DCMAKE_INSTALL_LIBDIR=lib \
    '-DCMAKE_INSTALL_RPATH=$ORIGIN' \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_LIST=core,imgproc,imgcodecs \
    -DWITH_EIGEN=OFF \
    -DBUILD_TESTS=OFF \
    -DBUILD_PERF_TESTS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_DOCS=OFF \
    -DBUILD_opencv_apps=OFF \
    -DBUILD_opencv_python2=OFF \
    -DBUILD_opencv_python3=OFF \
    -DBUILD_JAVA=OFF \
    -DWITH_IPP=OFF \
    -DWITH_OPENCL=OFF \
    -DWITH_FFMPEG=OFF \
    -DWITH_GSTREAMER=OFF \
    -DWITH_GTK=OFF \
    -DWITH_QT=OFF \
    -DWITH_OPENEXR=OFF
cmake --build "${OPENCV_BUILD}" --parallel "${JOBS}"
cmake --install "${OPENCV_BUILD}"

# ONNX Runtime's supported build.sh drives CMake internally. Build CPU only.
echo '============================================================'
echo 'Building ONNX Runtime shared library from local GitHub sources'
echo '============================================================'
(
    cd "${ORT_SRC}"
    ./build.sh --config Release --update --build --build_shared_lib \
        --parallel "${JOBS}" --skip_tests --build_dir "${ORT_BUILD}" \
        --cmake_generator "Unix Makefiles" --compile_no_warning_as_error
)

# Find the actual ORT CMake output; the Release directory depends on build script version.
ORT_SO="$(find "${ORT_BUILD}" -name 'libonnxruntime.so' -print -quit)"
if [[ -z "${ORT_SO}" ]]; then
    echo "ERROR: Cannot locate libonnxruntime.so under ${ORT_BUILD}" >&2
    exit 1
fi
ORT_LIB_DIR="$(dirname "${ORT_SO}")"
shopt -s nullglob
ort_libs=( "${ORT_LIB_DIR}"/libonnxruntime*.so* )
if (( ${#ort_libs[@]} == 0 )); then
    echo "ERROR: ONNX Runtime .so files not found in ${ORT_LIB_DIR}" >&2
    exit 1
fi
cp -a "${ort_libs[@]}" "${ORT_INSTALL}/lib/"
# Flatten the public C++ header directory into our local SDK's include path.
ORT_HEADER_DIR="${ORT_SRC}/include/onnxruntime/core/session"
if [[ ! -f "${ORT_HEADER_DIR}/onnxruntime_cxx_api.h" ]]; then
    echo "ERROR: Missing ONNX Runtime C++ header in ${ORT_HEADER_DIR}" >&2
    exit 1
fi
cp -a "${ORT_HEADER_DIR}/." "${ORT_INSTALL}/include/"

printf '\nThird-party builds complete:\n'
find "${OPENCV_INSTALL}/lib" -maxdepth 1 -name 'libopencv*.so' -print
find "${ORT_INSTALL}/lib" -maxdepth 1 -name 'libonnxruntime*.so' -print
