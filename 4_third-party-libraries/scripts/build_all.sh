#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
"${ROOT}/scripts/get_sources.sh"
"${ROOT}/scripts/build_third_party.sh"
"${ROOT}/scripts/get_model.sh"
"${ROOT}/scripts/build_project.sh"
