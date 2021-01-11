#!/bin/bash

# PSPDEV=$(psp-config -d)

PWD=$(pwd)

make clean
rm -r CMakeFiles
rm -r CMakeCache.txt
rm cmake_isntall.cmake

mkdir -p build

cd build

cmake .. \
    -DCMAKE_VERBOSE_MAKEFILE=1 \
    -DCMAKE_MODULE_PATH="$PWD" \
    -DCMAKE_TOOLCHAIN_FILE="psp.cmake" \
    -DSDL2_INCLUDE_DIRS="$PSPDEV/psp/include/SDL2" \
    -DSDL2_LIBRARIES="$PSPDEV/psp/lib/libSDL.a;$PSPDEV/psp/lib/libSDL2main.a;$PSPDEV/psp/lib/libSDL2_mixer.a" &&

make ../Makefile
