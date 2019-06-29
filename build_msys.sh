#!/bin/bash

set -xeu

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
script="${scriptdir}/script/build_front.sh"
generator="MSYS Makefiles"
cmakeopts=""
cmakebuildopts="-j4"

exec "${script}" "${scriptdir}" "${generator}" "${cmakeopts}" "${cmakebuildopts}" $@
