#!/usr/bin/env bash
# Pin release tags to ensure you do not accidentally build an unstable main branch.
export OPENCV_TAG="${OPENCV_TAG:-4.12.0}"
export ONNXRUNTIME_TAG="${ONNXRUNTIME_TAG:-v1.26.0}"
