#ifndef _INCL_PHYSFS_PLATFORMS
#define _INCL_PHYSFS_PLATFORMS

#ifndef __PHYSICSFS_INTERNAL__
#error Do not include this header from your applications.
#endif

/*
 * These only define the platforms to determine which files in the platforms
 *  directory should be compiled. For example, technically BeOS can be called
 *  a "unix" system, but since it doesn't use unix.c, we don't define
 *  PHYSFS_PLATFORM_UNIX on that system.
 */

// #define PHYSFS_HAVE_LLSEEK 0
#define PHYSFS_NO_THREAD_SUPPORT 1
#define PHYSFS_NO_CDROM_SUPPORT 1
#define PHYSFS_PLATFORM_PSP 1
// #define PHYSFS_PLATFORM_UNIX 1
// #define PHYSFS_PLATFORM_POSIX 1

#endif  /* include-once blocker. */

