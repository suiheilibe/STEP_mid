#!/bin/bash

set -euo pipefail

rootdir="$1"
script="${rootdir}/script/do_build.sh"
generator="$2"
cmakeopts="$3"
cmakebuildopts="$4"

targets="STEP_M STEP_K_x86 STEP_K_x64"
msystem_STEP_M="MINGW32"
msystem_STEP_K_x86="MINGW32"
msystem_STEP_K_x64="MINGW64"

for target in ${targets}
do
    eval 'msystem=${msystem_'${target}'}'
    MSYSTEM="${msystem}" /bin/bash -l "${script}" "${rootdir}" "${generator}" "${target}" "" "${cmakeopts}" "${cmakebuildopts}" $@ &
    eval "pids_${target}=$!"
done

ret=0
for target in ${targets}
do
    eval wait '$pids_'${target}
    if [ $? -ne 0 ]
    then
        ret=1
    fi
done

exit ${ret}
