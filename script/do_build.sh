#!/bin/bash

set -e

rootdir="$1"
generator="$2"
target="$3"
cmakeopts="$4"
cmakebuildopts="$5"

builddir="${rootdir}/build/${target}"
distdir="${rootdir}/dist/${target}"

if echo "${target}" | grep "^STEP_K" >/dev/null
then
    cmakeopts="${cmakeopts} -DSTEP_K=1"
fi

mkdir -p "${builddir}"
cd "${builddir}"
rm -rf *
cmake ../.. -G "${generator}" ${cmakeopts}
cmake --build . -- ${cmakebuildopts}
mkdir -p "${distdir}"
cp src/STEP_mid.ste "${distdir}"
cd "${distdir}"
strip STEP_mid.ste