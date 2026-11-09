#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "${ROOT}/scripts/versions.sh"
mkdir -p "${ROOT}/third_party"

clone_pinned() {
    local name="$1" url="$2" tag="$3"
    local dst="${ROOT}/third_party/${name}"
    if [[ -d "${dst}/.git" ]]; then
        echo "Using existing ${name} repository: ${dst}"
        echo "  commit: $(git -C "${dst}" rev-parse HEAD)"
        return
    fi
    if [[ -e "$dst" ]]; then
        echo "ERROR: ${dst} exists but is not a Git checkout; inspect or remove it." >&2
        exit 1
    fi
    echo "Cloning ${url} (tag ${tag}) -> ${dst}"
    git clone --depth 1 --branch "${tag}" --recurse-submodules --shallow-submodules "${url}" "${dst}"
    echo "  commit: $(git -C "${dst}" rev-parse HEAD)"
}

clone_pinned opencv "https://github.com/opencv/opencv.git" "${OPENCV_TAG}"
clone_pinned onnxruntime "https://github.com/microsoft/onnxruntime.git" "${ONNXRUNTIME_TAG}"
