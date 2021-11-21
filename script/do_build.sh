#!/bin/bash

set -euo pipefail

rootdir="$1"
generator="$2"
target="$3"
buildtype="$4"
cmakeopts="$5"
cmakebuildopts="$6"

realcmakeopts="${cmakeopts}"
realcmakebuildopts="${cmakebuildopts}"

cmakebuildconfig=""

if [[ -n "${generator}" ]]
then
    realcmakeopts="${realcmakeopts} -G \"${generator}\""
fi

if echo "${target}" | grep "^STEP_K" >/dev/null
then
    realcmakeopts="${realcmakeopts} -DSTEP_K=1"
fi

if echo "${generator}" | grep "^Visual Studio" >/dev/null
then
    if [[ -n "${buildtype}" ]]
    then
        cmakebuildconfig="--config ${buildtype}"
    else
        cmakebuildconfig="--config Release"
    fi
else
    if [[ -n "${buildtype}" ]]
    then
        realcmakeopts="${realcmakeopts} -DCMAKE_BUILD_TYPE=${buildtype}"
    fi
fi

builddir="${rootdir}/build/${target}"
distdir="${rootdir}/dist/${target}"

mkdir -p "${builddir}"
cd "${builddir}"
eval "cmake ../.. ${realcmakeopts}"
eval "cmake --build . ${cmakebuildconfig} -- ${realcmakebuildopts}"
mkdir -p "${distdir}"
cp src/STEP_mid.ste "${distdir}"
cd "${distdir}"
strip STEP_mid.ste
