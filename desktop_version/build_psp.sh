#!/bin/bash

# PSPDEV=$(psp-config -d)

rm -r build
rm cmake_install.cmake

mkdir -p build

cd build

# -DCMAKE_VERBOSE_MAKEFILE=1 \
cmake .. \
    -DSDL2_INCLUDE_DIRS="$PSPDEV/psp/include/SDL2" \
    -DSDL2_LIBRARIES="$PSPDEV/psp/lib/libSDL2.a;$PSPDEV/psp/lib/libSDL2main.a;$PSPDEV/psp/lib/libSDL2_mixer.a" \
    || exit 1

cd ../

cmake --build build || exit 1

cd build

# psp-gcc -G0 -L$PSPDEV/psp/sdk/lib -o VVVVVV ./build/CMakeFiles/VVVVVV.dir/src/*.obj -L./build/ -llodepng-static -ltinyxml2-static -lphysfs-static -lstdc++ -lSDL2_mixer -lSDL2 -lSDL2main -lGL -lpspvfpu -lpspvram -lvorbisidec -lpthread-psp -lc  -lpsphprm -lpspdebug -lpspgu -lpspctrl -lpspdisplay -lpspge -lpspsdk -lpsprtc -lpspaudio -lpspirkeyb  -lpspnet -lpspnet_inet -lpsppower -lpsputility -lpspuser -lpspkernel -lm -specs=$PSPDEV/psp/sdk/lib/prxspecs -Wl,-T$PSPDEV/psp/sdk/lib/linkfile.prx,-q

# psp-fixup-imports VVVVVV
# psp-prxgen VVVVVV VVVVVV.prx
