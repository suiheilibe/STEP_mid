#!/bin/bash

set -euo pipefail

printf "#define VERSION \"%s\"\n" "$(git describe --tags)"
