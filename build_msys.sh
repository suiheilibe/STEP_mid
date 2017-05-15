#!/bin/bash

set -e

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
script="${scriptdir}/script/build_front.sh"
generator="MSYS Makefiles"
cmakeopts=""
cmakebuildopts="-j4"

MSYSTEM=MINGW32 /bin/bash --login "${script}" -R "${scriptdir}" -G "${generator}" -t STEP_M -c "${cmakeopts}" -b "${cmakebuildopts}" $@ &
MSYSTEM=MINGW32 /bin/bash --login "${script}" -R "${scriptdir}" -G "${generator}" -t STEP_K_x86 -c "${cmakeopts}" -b "${cmakebuildopts}" $@ &
MSYSTEM=MINGW64 /bin/bash --login "${script}" -R "${scriptdir}" -G "${generator}" -t STEP_K_x64 -c "${cmakeopts}" -b "${cmakebuildopts}" $@ &

wait
