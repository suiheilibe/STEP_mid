@echo off

SET "PATH=C:\msys64\usr\bin;%PATH%"

@echo on

bash -lc "pacman -Syu --noconfirm"
bash -lc "pacman -S --needed --noconfirm mingw-w64-i686-cmake mingw-w64-x86_64-cmake"
bash -lc "pacman -Sc --noconfirm"
