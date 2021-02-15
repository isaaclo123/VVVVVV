## How to Build

* Install psptoolchain as usual
* install my psplibraries fork with small SDL compiler flag changes
    * 
      [https://github.com/isaaclo123/psplibraries](https://github.com/isaaclo123/psplibraries)

To generate the projects on Linux:

```bash
# in the VVVVVV/desktop_version/ directory

./build_psp.sh
```

This will generate a EBOOT.PBP inside the build folder

## How to Install on PSP

1. Create a folder VVVVVV
2. Extract the contents of data,zip into the VVVVVV folder
3. Find a VVVVVV music unpacker (like vvvvvv_music_unpacker) on github to unpack the 
   vvvvvvmusic.vvv binary.
   * This should create a folder called data/music inside the VVVVVV folder
   * Afterward, feel free to delete the vvvvvvmusic.vvv binary if you wish
4. Place the EBOOT.PBP inside the VVVVVV folder
5. Place the VVVVVV folder into /PSP/GAME
6. Enjoy!

## About

This port is based off of the code from 
[https://github.com/gusarba/VVVVVVDC](https://github.com/gusarba/VVVVVVDC). The port is a bit messy 
and has a known memory leak regarding music loading. The port also only compiles for PSP, no other 
working target. Certain backgrounds are removed in the Tower stage for slightly better performance, 
and background scrolling has been disabled for performance too. Certain levels like the Tower are 
still slower than normal, but playable.
