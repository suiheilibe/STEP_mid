#!/bin/bash

set -euo pipefail

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"

MSYSTEM=MINGW32 /bin/bash -lc "cd ${scriptdir}/build/STEP_M && ctest" &
pids_0=$!

MSYSTEM=MINGW32 /bin/bash -lc "cd ${scriptdir}/build/STEP_K_x86 && ctest" &
pids_1=$!

MSYSTEM=MINGW64 /bin/bash -lc "cd ${scriptdir}/build/STEP_K_x64 && ctest" &
pids_2=$!

ret=0
for i in $(seq 0 2)
do
    eval wait '$pids_'$i
    if [ $? -ne 0 ]
    then
        ret=1
    fi
done

exit ${ret}
