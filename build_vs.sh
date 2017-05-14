#!/bin/bash

set -e

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
do_build="${scriptdir}/script/do_build.sh"
generator="Visual Studio 15 2017"
cmakeopts="-DCMAKE_BUILD_TYPE=Release"

MSYSTEM=MINGW32 /bin/bash --login -e "${do_build}" "${scriptdir}" "${generator}" STEP_M "${cmakeopts}" &
MSYSTEM=MINGW32 /bin/bash --login -e "${do_build}" "${scriptdir}" "${generator}" STEP_K_x86 "${cmakeopts}" &
MSYSTEM=MINGW64 /bin/bash --login -e "${do_build}" "${scriptdir}" "${generator} Win64" STEP_K_x64 "${cmakeopts}" &

wait
