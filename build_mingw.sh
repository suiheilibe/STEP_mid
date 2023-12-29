#!/bin/bash

set -euo pipefail

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
script="${scriptdir}/scripts/build_front.sh"
generator="Ninja"
cmakeopts="-DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchain-mingw32.cmake"
cmakebuildopts="-j4"

exec "${script}" "${scriptdir}" "${generator}" "${cmakeopts}" "${cmakebuildopts}" $@
