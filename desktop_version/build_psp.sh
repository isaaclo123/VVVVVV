#!/bin/bash

# PSPDEV=$(psp-config -d)

BUILD=true
LINK=true

if [ "$BUILD" = true ]; then
    rm Makefile
    mkdir -p build
    cd build

    find . -mindepth 1 ! -regex '^./vvvvvv\(/.*\)?' -delete

    # -DCMAKE_VERBOSE_MAKEFILE=1 \
    cmake .. \
        -DSDL_INCLUDE_DIRS="$PSPDEV/psp/include/SDL" \
        -DSDL_LIBRARIES="$PSPDEV/psp/lib/libSDL.a;$PSPDEV/psp/lib/libSDLmain.a;$PSPDEV/psp/lib/libSDL_mixer.a" \
        || exit 1

    cd ../

    cmake --build build
fi


if [ "$LINK" = true ]; then
    echo
    echo "-------REAL LINKER HERE-------"
    echo

    cd build

    set -ex

    psp-gcc -g -G0 -D_GNU_SOURCE=1 -D__POSIX_C_VISIBLE=200809 -L$PSPDEV/psp/sdk/lib -L$PSPDEV/psp/lib -o VVVVVV ./CMakeFiles/VVVVVV.dir/src/*.obj -L./ -llodepng-static -ltinyxml-static -lphysfs-static -lstdc++ -lSDL_mixer -lSDL -lGL -lmikmod -lpspvfpu -lpspvram -lvorbisidec -lpthread-psp -lc  -lpsphprm -lpspdebug -lpspgu -lpspctrl -lpspdisplay -lpspge -lpspsdk -lpsprtc -lpspaudio -lpspnet -lpspnet_inet -lpsppower -lpsputility -lpspuser -lpspkernel -lm -specs=$PSPDEV/psp/sdk/lib/prxspecs -Wl,-T$PSPDEV/psp/sdk/lib/linkfile.prx,-q

    psp-fixup-imports VVVVVV
    psp-prxgen VVVVVV VVVVVV.prx
    mksfo VVVVVV PARAM.SFO
    pack-pbp EBOOT.PBP PARAM.SFO ../VVVVVV.png NULL NULL NULL NULL VVVVVV.prx NULL
fi
