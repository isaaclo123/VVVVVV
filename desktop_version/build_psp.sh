#!/bin/bash

# PSPDEV=$(psp-config -d)

# jrm -rf build/*
# jrm Makefile
# j
# jmkdir -p build
# j
# jcd build
# j
# j# -DCMAKE_VERBOSE_MAKEFILE=1 \
# jcmake .. \
# j    -DSDL_INCLUDE_DIRS="$PSPDEV/psp/include/SDL" \
# j    -DSDL_LIBRARIES="$PSPDEV/psp/lib/libSDL.a;$PSPDEV/psp/lib/libSDLmain.a;$PSPDEV/psp/lib/libSDL_mixer.a" \
# j    || exit 1
# j
# jcd ../
# j
# jcmake --build build


echo
echo "-------REAL LINKER HERE-------"
echo

cd build

set -ex

psp-gcc -g -G0 -D_GNU_SOURCE=1 -D__POSIX_C_VISIBLE=200809 -L$PSPDEV/psp/sdk/lib -L$PSPDEV/psp/lib -o VVVVVV ./CMakeFiles/VVVVVV.dir/src/*.obj -L./ -llodepng-static -ltinyxml-static -lphysfs-static -lstdc++ -lSDL_mixer -lSDL -lGL -lmikmod -lpspvfpu -lpspvram -lvorbisidec -lpthread-psp -lc  -lpsphprm -lpspdebug -lpspgu -lpspctrl -lpspdisplay -lpspge -lpspsdk -lpsprtc -lpspaudio -lpspnet -lpspnet_inet -lpsppower -lpsputility -lpspuser -lpspkernel -lm -specs=$PSPDEV/psp/sdk/lib/prxspecs -Wl,-T$PSPDEV/psp/sdk/lib/linkfile.prx,-q

psp-fixup-imports VVVVVV
psp-prxgen VVVVVV VVVVVV.prx
