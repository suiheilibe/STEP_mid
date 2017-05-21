#!/bin/bash

set -e

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
script="${scriptdir}/script/build_front.sh"
generator="MSYS Makefiles"
cmakeopts=""
cmakebuildopts="-j4"

MSYSTEM=MINGW32 /bin/bash -l "${script}" -R "${scriptdir}" -G "${generator}" -t STEP_M -c "${cmakeopts}" -b "${cmakebuildopts}" $@ &
pids_0=$!

MSYSTEM=MINGW32 /bin/bash -l "${script}" -R "${scriptdir}" -G "${generator}" -t STEP_K_x86 -c "${cmakeopts}" -b "${cmakebuildopts}" $@ &
pids_1=$!

MSYSTEM=MINGW64 /bin/bash -l "${script}" -R "${scriptdir}" -G "${generator}" -t STEP_K_x64 -c "${cmakeopts}" -b "${cmakebuildopts}" $@ &
pids_2=$!

for i in $(seq 0 2)
do
    eval wait '$pids_'$i
    if [ $? -ne 0 ]
    then
        exit 1
    fi
done
