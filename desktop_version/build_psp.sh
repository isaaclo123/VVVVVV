#!/bin/bash

# PSPDEV=$(psp-config -d)

rm -r build
rm cmake_install.cmake

mkdir -p build

cd build

# -DCMAKE_VERBOSE_MAKEFILE=1 \
cmake .. \
    -DCMAKE_TOOLCHAIN_FILE="psp.cmake" \
    -DSDL2_INCLUDE_DIRS="$PSPDEV/psp/include/SDL2" \
    -DSDL2_LIBRARIES="$PSPDEV/psp/lib/libSDL.a;$PSPDEV/psp/lib/libSDL2main.a;$PSPDEV/psp/lib/libSDL2_mixer.a" &&

cd ../

cmake --build build
