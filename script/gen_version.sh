#!/bin/bash

set -euo pipefail

printf "#define VERSION \"%s\"\r\n" "$(git describe --tags)"
