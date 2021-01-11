#!/bin/bash

# PSPDEV=$(psp-config -d)

make clean

cmake . \
    -DCMAKE_VERBOSE_MAKEFILE=1 \
    -DSDL2_INCLUDE_DIRS="$PSPDEV/psp/include/SDL2" \
    -DSDL2_LIBRARIES="$PSPDEV/psp/lib/libSDL.a;$PSPDEV/psp/lib/libSDL2main.a;$PSPDEV/psp/lib/libSDL2_mixer.a" &&

make
