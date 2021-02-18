#!/bin/bash

# PSPDEV=$(psp-config -d)

DELETE=false
BUILD=true
# LINK=true
LINK=true

mkdir -p build
cd build

if [ "$DELETE" = true ]; then
    find . -maxdepth 1 -type f -exec rm -f {} \;
    rm -rf CMakeFiles
    cp ../../third_party/MElib/bin/mediaengine.prx .
fi

if [ "$BUILD" = true ]; then
    # -DCMAKE_VERBOSE_MAKEFILE=1 \
    cmake .. \
        -DSDL_INCLUDE_DIRS="$PSPDEV/psp/include/SDL" \
        -DSDL_LIBRARIES="$PSPDEV/psp/lib/libSDL.a;$PSPDEV/psp/lib/libSDLmain.a;$PSPDEV/psp/lib/libSDL_mixer.a" \
        || exit 1

    cd ../

    cmake --build build
fi

cd build

if [ "$LINK" = true ]; then
    echo
    echo "-------REAL LINKER HERE-------"
    echo

    echo "$(pwd)"

    # cd build

    set -ex

        # ./me_stub.o \

    psp-gcc -g -G0 -D_GNU_SOURCE=1 -D__POSIX_C_VISIBLE=200809 \
        -L$PSPDEV/psp/sdk/lib -L$PSPDEV/psp/lib -L./ \
        -o VVVVVV \
        ./CMakeFiles/VVVVVV.dir/src/*.obj \
        ../../third_party/MElib/lib/libME.a \
        -llodepng-static -ltinyxml-static -lphysfs-static \
        -lstdc++ -lSDL_mixer -lSDL -lGL -lg -lmikmod -lpspvfpu -lpspvram \
        -lvorbisidec -lpthread-psp -lc  -lpsphprm -lpspdebug \
        -lpspgu -lpspctrl -lpspdisplay -lpspge -lpsprtc -lpspsdk \
        -lpspaudio -lpspnet -lpspnet_inet -lpsppower \
        -lpsputility -lpspuser -lpspkernel -lm\
        -specs=$PSPDEV/psp/sdk/lib/prxspecs -Wl,-T$PSPDEV/psp/sdk/lib/linkfile.prx,-q

    psp-fixup-imports VVVVVV
    psp-prxgen VVVVVV VVVVVV.prx
    mksfo VVVVVV PARAM.SFO
    pack-pbp EBOOT.PBP PARAM.SFO ../VVVVVV.png NULL NULL NULL NULL VVVVVV.prx NULL
fi
