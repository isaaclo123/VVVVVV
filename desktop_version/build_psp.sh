#!/bin/bash

# PSPDEV=$(psp-config -d)

rm -rf build/*
rm Makefile

mkdir -p build

cd build

# -DCMAKE_VERBOSE_MAKEFILE=1 \
cmake .. \
    -DSDL_INCLUDE_DIRS="$PSPDEV/psp/include/SDL" \
    -DSDL_LIBRARIES="$PSPDEV/psp/lib/libSDL.a;$PSPDEV/psp/lib/libSDLmain.a;$PSPDEV/psp/lib/libSDL_mixer.a" \
    || exit 1

cd ../

cmake --build build


echo
echo "-------REAL LINKER HERE-------"
echo

cd build

set -ex

psp-gcc -g -G0 -D_GNU_SOURCE -D_POSIX_C_SOURCE=200809L -L$PSPDEV/psp/sdk/lib -o VVVVVV ./CMakeFiles/VVVVVV.dir/src/*.obj -L./ -llodepng-static -ltinyxml-static -lphysfs-static -lstdc++ -lSDL_mixer -lSDL -lSDLmain -lGL -lmikmod -lpspvfpu -lpspvram -lvorbisidec -lpthread-psp -lc  -lpsphprm -lpspdebug -lpspgu -lpspctrl -lpspdisplay -lpspge -lpspsdk -lpsprtc -lpspaudio -lpspirkeyb  -lpspnet -lpspnet_inet -lpsppower -lpsputility -lpspuser -lpspkernel -lm -specs=$PSPDEV/psp/sdk/lib/prxspecs -Wl,-T$PSPDEV/psp/sdk/lib/linkfile.prx,-q

psp-fixup-imports VVVVVV
psp-prxgen VVVVVV VVVVVV.prx
