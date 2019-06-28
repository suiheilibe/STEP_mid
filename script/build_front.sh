#!/bin/bash

set -xeu

rootdir=""
generator=""
target=""
cmakeopts=""
cmakebuildopts=""
buildtype="Release"

while getopts dR:G:t:c:b: OPT
do
    case $OPT in
        R)  rootdir=$OPTARG
            ;;
        G)  generator=$OPTARG
            ;;
        t)  target=$OPTARG
            ;;
        c)  cmakeopts=$OPTARG
            ;;
        b)  cmakebuildopts=$OPTARG
            ;;
        d)  buildtype="Debug"
            ;;
    esac
done

shift $((OPTIND - 1))

cmakeopts="${cmakeopts} -DCMAKE_BUILD_TYPE=${buildtype}"

if echo "${target}" | grep "^STEP_K" >/dev/null
then
    cmakeopts="${cmakeopts} -DSTEP_K=1"
fi

exec "${rootdir}/script/do_build.sh" "${rootdir}" "${generator}" "${target}" "${cmakeopts}" "${cmakebuildopts}"