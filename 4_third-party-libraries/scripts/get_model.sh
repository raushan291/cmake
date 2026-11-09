#!/usr/bin/env bash
set -euo pipefail

project_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
mkdir -p "${project_dir}/models"
destination="${project_dir}/models/mnist-8.onnx"
url="https://huggingface.co/onnxmodelzoo/mnist-8/resolve/main/mnist-8.onnx"
# SHA-256 published for the onnxmodelzoo/mnist-8 model revision.
expected_sha="2f06e72de813a8635c9bc0397ac447a601bdbfa7df4bebc278723b958831c9bf"

if [[ -f "${destination}" ]]; then
    if echo "${expected_sha}  ${destination}" | sha256sum --check --status; then
        echo "Already downloaded and verified: ${destination}"
        exit 0
    fi
    echo "Existing model checksum mismatched. Re-downloading." >&2
fi

tmp="$(mktemp)"
trap 'rm -f "${tmp}"' EXIT
curl --fail --location --retry 3 --show-error --output "${tmp}" "${url}"
echo "${expected_sha}  ${tmp}" | sha256sum --check --status || {
    echo "ERROR: MNIST model hash check failed. Nothing was installed." >&2
    exit 1
}
mv "${tmp}" "${destination}"
echo "Saved: ${destination}"
