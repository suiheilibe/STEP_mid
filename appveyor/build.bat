@echo off

SET "PATH=C:\msys64\usr\bin;%PATH%"

@echo on

bash -lc "cd $APPVEYOR_BUILD_FOLDER; ./build_msys.sh"
