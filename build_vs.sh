#!/bin/bash

set -euo pipefail

scriptdir="$( cd -P "$(dirname "$0")" && pwd -P )"
script="${scriptdir}/scripts/build_front.sh"
generator="Visual Studio 16 2019"
cmakeopts=""
cmakebuildopts=""

${scriptdir}/script/gen_version.sh > "${scriptdir}/src/version.h"

exec "${script}" "${scriptdir}" "${generator}" "${cmakeopts}" "${cmakebuildopts}" $@
