#!/bin/bash

set -e

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
do_build="${scriptdir}/script/do_build.sh"
generator="MSYS Makefiles"
cmakeopts="-DCMAKE_BUILD_TYPE=Release"
cmakebuildopts="-j4"

MSYSTEM=MINGW32 /bin/bash --login -e "${do_build}" "${scriptdir}" "${generator}" STEP_M "${cmakeopts}" "${cmakebuildopts}" &
MSYSTEM=MINGW32 /bin/bash --login -e "${do_build}" "${scriptdir}" "${generator}" STEP_K_x86 "${cmakeopts}" "${cmakebuildopts}" &
MSYSTEM=MINGW64 /bin/bash --login -e "${do_build}" "${scriptdir}" "${generator}" STEP_K_x64 "${cmakeopts}" "${cmakebuildopts}" &

wait
