@echo off

SET "PATH=C:\msys64\usr\bin;%PATH%"

@echo on

bash -lc "./test_all.sh"
