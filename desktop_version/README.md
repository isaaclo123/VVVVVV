How to Build
------------

* Install psptoolchain as usual
* install my psplibraries fork with small compiler flag changes
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

Including data.zip
------------
You'll need the data.zip file from VVVVVV to actually run the game! It's
available to download separately for free in the
[Make and Play](https://thelettervsixtim.es/makeandplay/)
edition of the game. Put this file next to your executable and the game should
run.

This is intended for personal use only - our license doesn't allow you to
actually distribute this data.zip file with your own forks without getting
permission from us first. See [LICENSE.md](../LICENSE.md) for more details. (If
you've got a project in mind that requires distributing this
file, [get in touch](http://distractionware.com/email/)!)

A Word About Compiler Quirks
----------------------------

_(Note: This section only applies to version 2.2 of the source code, which is
the initial commit of this repository. Since then, much hard work has been put
in to fix many undefined behaviors. If you're compiling the latest version of
the source code, ignore this section.)_

This engine is _super_ fussy about optimization levels and runtime checks. In
particular, the Windows version _absolutely positively must_ be compiled in
Debug mode, with /RTC enabled. If you build in Release mode, or have /RTC
disabled, the game behaves dramatically different in ways that were never fully
documented (bizarre softlocks, out-of-bounds issues that don't show up in tools
like Valgrind, stuff like that). There are lots of things about this old code
that could be cleaned up, polished, rewritten, and so on, but this is the one
that will probably bite you the hardest when setting up your own build,
regardless of platform.

We hope you'll enjoy messing with the source anyway!

Love, flibit
