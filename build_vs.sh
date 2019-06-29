#!/bin/bash

set -xeu

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
script="${scriptdir}/script/build_front.sh"
generator="Visual Studio 15 2017"
cmakeopts=""
cmakebuildopts=""

exec "${script}" "${scriptdir}" "${generator}" "${cmakeopts}" "${cmakebuildopts}" $@
