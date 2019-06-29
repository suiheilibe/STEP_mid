#!/bin/bash

set -xeu

rootdir="$1"
generator="$2"
target="$3"
buildtype="$4"
cmakeopts="$5"
cmakebuildopts="$6"

realcmakeopts=""

if [[ -n "${generator}" ]]
then
    realcmakeopts="${cmakeopts} -G \"${generator}\""
fi

if [[ -n "${buildtype}" ]]
then
    realcmakeopts="${cmakeopts} -DCMAKE_BUILD_TYPE=${buildtype}"
fi

if echo "${target}" | grep "^STEP_K" >/dev/null
then
    realcmakeopts="${realcmakeopts} -DSTEP_K=1"
fi

builddir="${rootdir}/build/${target}"
distdir="${rootdir}/dist/${target}"

mkdir -p "${builddir}"
cd "${builddir}"
eval "cmake ../.. ${realcmakeopts}"
eval "cmake --build . -- ${cmakebuildopts}"
mkdir -p "${distdir}"
cp src/STEP_mid.ste "${distdir}"
cd "${distdir}"
strip STEP_mid.ste
