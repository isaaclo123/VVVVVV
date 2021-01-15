/*
 * PSP support routines for PhysicsFS.
 *
 * Please see the file LICENSE.txt in the source's root directory.
 *
 *  This file written by Ryan C. Gordon and Ezekiel Bethel.
 */

#define __PHYSICSFS_INTERNAL__
#include "physfs_platforms.h"

#ifdef PHYSFS_PLATFORM_PSP

#include <pspkernel.h>
#include <psptypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <unistd.h>
// #include <ctype.h>
#// include <sys/types.h>
// #include <sys/stat.h>
// #include <pwd.h>
// #include <dirent.h>
#include <errno.h>
// #include <fcntl.h>
#include <time.h>

#include "physfs_internal.h"

#define MAXPATHLEN 1024

static PHYSFS_ErrorCode errcodeFromErrnoError(const int err)
{
    switch (err)
    {
        case 0: return PHYSFS_ERR_OK;
        case EACCES: return PHYSFS_ERR_PERMISSION;
        case EPERM: return PHYSFS_ERR_PERMISSION;
        case EDQUOT: return PHYSFS_ERR_NO_SPACE;
        case EIO: return PHYSFS_ERR_IO;
        case ELOOP: return PHYSFS_ERR_SYMLINK_LOOP;
        case EMLINK: return PHYSFS_ERR_NO_SPACE;
        case ENAMETOOLONG: return PHYSFS_ERR_BAD_FILENAME;
        case ENOENT: return PHYSFS_ERR_NOT_FOUND;
        case ENOSPC: return PHYSFS_ERR_NO_SPACE;
        case ENOTDIR: return PHYSFS_ERR_NOT_FOUND;
        case EISDIR: return PHYSFS_ERR_NOT_A_FILE;
        case EROFS: return PHYSFS_ERR_READ_ONLY;
        case ETXTBSY: return PHYSFS_ERR_BUSY;
        case EBUSY: return PHYSFS_ERR_BUSY;
        case ENOMEM: return PHYSFS_ERR_OUT_OF_MEMORY;
        case ENOTEMPTY: return PHYSFS_ERR_DIR_NOT_EMPTY;
        default: return PHYSFS_ERR_OS_ERROR;
    } /* switch */
} /* errcodeFromErrnoError */


static inline PHYSFS_ErrorCode errcodeFromErrno(void)
{
    return errcodeFromErrnoError(errno);
} /* errcodeFromErrno */

PHYSFS_EnumerateCallbackResult __PHYSFS_platformEnumerate(const char *dirname,
                               PHYSFS_EnumerateCallback callback,
                               const char *origdir, void *callbackdata)
{
    SceUID dir;
    SceIoDirent ent = {};
    PHYSFS_EnumerateCallbackResult retval = PHYSFS_ENUM_OK;

    dir = sceIoDopen(dirname);
    BAIL_IF(dir > 0, errcodeFromErrno(), PHYSFS_ENUM_ERROR);

    while ((retval == PHYSFS_ENUM_OK) && (sceIoDread(dir, &ent) > 0))
    {
        const char *name = ent.d_name;
        if (name[0] == '.')  /* ignore "." and ".." */
        {
            if ((name[1] == '\0') || ((name[1] == '.') && (name[2] == '\0')))
                continue;
        } /* if */

        retval = callback(callbackdata, origdir, name);
        if (retval == PHYSFS_ENUM_ERROR)
            PHYSFS_setErrorCode(PHYSFS_ERR_APP_CALLBACK);
    } /* while */

    sceIoDclose(dir);

    return retval;
} /* __PHYSFS_platformEnumerate */

static inline PHYSFS_sint64 getEpochTime(ScePspDateTime* dt, struct tm* t) {
    t->tm_year = dt->year;
    t->tm_mon = dt->month;           // Month, where 0 = jan
    t->tm_mday = dt->day;          // Day of the month
    t->tm_hour = dt->hour;
    t->tm_min = dt->minute;
    t->tm_sec = dt->microsecond;
    t->tm_isdst = 1;        // Is DST on? 1 = yes, 0 = no, -1 = unknown

    return (PHYSFS_sint64) mktime(t);
}

int __PHYSFS_platformStat(const char *fname, PHYSFS_Stat *st, const int follow)
{
    // TODO
    SceIoStat statbuf = {};
    SceIoDirent dirent = {};
    struct tm t = {};
    int rc;

    // try directory
    int dfd = sceIoDopen(fname);

    if (dfd >= 0) {
        rc = sceIoDread(dfd, &dirent);
        BAIL_IF(rc < 0, PHYSFS_ERR_NOT_FOUND, 0);

        statbuf = dirent.d_stat;
        sceIoDclose(dfd);
    } else { // if fails, try file
        rc = sceIoGetstat(fname, &statbuf);
        BAIL_IF(rc < 0, PHYSFS_ERR_NOT_FOUND, 0);
    }

    if (FIO_S_ISREG(statbuf.st_mode))
    {
        st->filetype = PHYSFS_FILETYPE_REGULAR;
        st->filesize = statbuf.st_size;
    } /* if */

    else if(FIO_S_ISDIR(statbuf.st_mode))
    {
        st->filetype = PHYSFS_FILETYPE_DIRECTORY;
        st->filesize = 0;
    } /* else if */

    else if(FIO_S_ISLNK(statbuf.st_mode))
    {
        st->filetype = PHYSFS_FILETYPE_SYMLINK;
        st->filesize = 0;
    } /* else if */

    else
    {
        st->filetype = PHYSFS_FILETYPE_OTHER;
        st->filesize = statbuf.st_size;
    } /* else */

    st->modtime = getEpochTime(&(statbuf.st_mtime), &t);
    st->createtime = getEpochTime(&(statbuf.st_ctime), &t);
    st->accesstime = getEpochTime(&(statbuf.st_atime), &t);

    st->readonly = ((statbuf.st_mode & PSP_O_RDONLY) == PSP_O_RDONLY);
    return 1;
} /* __PHYSFS_platformStat */

int __PHYSFS_platformInit(void)
{
    return(1);  /* always succeed. */
} /* __PHYSFS_platformInit */


void __PHYSFS_platformDeinit(void) {} /* __PHYSFS_platformDeinit */

// const char *__PHYSFS_platformDirSeparator = "/";

char *__PHYSFS_platformCopyEnvironmentVariable(const char *varname)
{
    const char *envr = getenv(varname);
    char *retval = NULL;

    if (envr != NULL)
    {
        retval = (char *) allocator.Malloc(strlen(envr) + 1);
        if (retval != NULL)
            strcpy(retval, envr);
    } /* if */

    return(retval);
} /* __PHYSFS_platformCopyEnvironmentVariable */


static char *getUserNameByUID(void)
{
    return(NULL);
} /* getUserNameByUID */


static char *getUserDirByUID(void)
{
    return(NULL);
} /* getUserDirByUID */


// char *__PHYSFS_platformGetUserName(void)
// {
//     char *retval = getUserNameByUID();
//     if (retval == NULL)
//         retval = __PHYSFS_platformCopyEnvironmentVariable("USER");
//     return(retval);
// } /* __PHYSFS_platformGetUserName */


char *__PHYSFS_platformCalcUserDir(void)
{
    char *ret = allocator.Malloc(37);
    if (ret != NULL)
        strcpy(ret, "ms0:/PSP/GAME/");
    return(ret);
    // char *retval = __PHYSFS_platformCopyEnvironmentVariable("HOME");

    // /* if the environment variable was set, make sure it's really a dir. */
    // if (retval != NULL)
    // {
    //     struct stat statbuf;
    //     if ((stat(retval, &statbuf) == -1) || (S_ISDIR(statbuf.st_mode) == 0))
    //     {
    //         allocator.Free(retval);
    //         retval = NULL;
    //     } /* if */
    // } /* if */

    // if (retval == NULL)
    //     retval = getUserDirByUID();

    // return(retval);
} /* __PHYSFS_platformGetUserDir */


int __PHYSFS_platformExists(const char *fname)
{
    struct SceIoStat statbuf = {};
    BAIL_IF(sceIoGetstat(fname, &statbuf) < 0, errcodeFromErrno(), 0);
    return(1);
} /* __PHYSFS_platformExists */


int __PHYSFS_platformIsSymLink(const char *fname)
{
    struct SceIoStat statbuf = {};
    BAIL_IF(sceIoGetstat(fname, &statbuf) < 0, errcodeFromErrno(), 0);
    return( (FIO_S_ISLNK(statbuf.st_mode)) ? 1 : 0 );
} /* __PHYSFS_platformIsSymlink */


int __PHYSFS_platformIsDirectory(const char *fname)
{
    struct SceIoStat statbuf = {};
    BAIL_IF(sceIoGetstat(fname, &statbuf) < 0, errcodeFromErrno(), 0);
    return( (FIO_S_ISDIR(statbuf.st_mode)) ? 1 : 0 );
} /* __PHYSFS_platformIsDirectory */


char *__PHYSFS_platformCvtToDependent(const char *prepend,
                                      const char *dirName,
                                      const char *append)
{
    int len = ((prepend) ? strlen(prepend) : 0) +
              ((append) ? strlen(append) : 0) +
              strlen(dirName) + 1;
    char *retval = (char *) allocator.Malloc(len);

    BAIL_IF(retval == NULL, errcodeFromErrno(), NULL);

    /* platform-independent notation is Unix-style already.  :)  */

    if (prepend)
        strcpy(retval, prepend);
    else
        retval[0] = '\0';

    strcat(retval, dirName);

    if (append)
        strcat(retval, append);

    return(retval);
} /* __PHYSFS_platformCvtToDependent */



// void __PHYSFS_platformEnumerateFiles(const char *dirname,
//                                      int omitSymLinks,
//                                      PHYSFS_EnumFilesCallback callback,
//                                      const char *origdir,
//                                      void *callbackdata)
// {
//     SceUID dir;
//     struct SceIoDirent ent = {};
//     int bufsize = 0;
//     char *buf = NULL;
//     int dlen = 0;
//
//     if (omitSymLinks)  /* !!! FIXME: this malloc sucks. */
//     {
//         dlen = strlen(dirname);
//         bufsize = dlen + 256;
//         buf = (char *) allocator.Malloc(bufsize);
//         if (buf == NULL)
//             return;
//         strcpy(buf, dirname);
//         if (buf[dlen - 1] != '/')
//         {
//             buf[dlen++] = '/';
//             buf[dlen] = '\0';
//         } /* if */
//     } /* if */
//
//     errno = 0;
//     dir = sceIoDopen(dirname);
//     if (dir < 0)
//     {
//         allocator.Free(buf);
//         return;
//     } /* if */
//
//     while (sceIoDread(dir, &ent))
//     {
//         if (strcmp(ent.d_name, ".") == 0)
//             continue;
//
//         if (strcmp(ent.d_name, "..") == 0)
//             continue;
//
//         if (omitSymLinks)
//         {
//             char *p;
//             int len = strlen(ent.d_name) + dlen + 1;
//             if (len > bufsize)
//             {
//                 p = (char *) allocator.Realloc(buf, len);
//                 if (p == NULL)
//                     continue;
//                 buf = p;
//                 bufsize = len;
//             } /* if */
//
//             strcpy(buf + dlen, ent.d_name);
//             if (__PHYSFS_platformIsSymLink(buf))
//                 continue;
//         } /* if */
//
//         callback(callbackdata, origdir, ent.d_name);
//     } /* while */
//
//     allocator.Free(buf);
//     sceIoDclose(dir);
// } /* __PHYSFS_platformEnumerateFiles */


int __PHYSFS_platformMkDir(const char *path)
{
    int rc;
    errno = 0;
    rc = sceIoMkdir(path, FIO_S_IRWXU);
    BAIL_IF(rc < 0, errcodeFromErrno(), 0);
    return(1);
} /* __PHYSFS_platformMkDir */


static void *doOpen(const char *filename, int flags)
{
    const int appending = (flags & PSP_O_APPEND);
    int fd;
    int *retval;
    errno = 0;

    /* O_APPEND doesn't actually behave as we'd like. */
    flags &= ~PSP_O_APPEND;

    fd = sceIoOpen(filename, flags, FIO_S_IRUSR | FIO_S_IWUSR);
    BAIL_IF(fd < 0, errcodeFromErrno(), NULL);

    if (appending)
    {
        if (sceIoLseek(fd, 0, SEEK_END) < 0)
        {
            sceIoClose(fd);
            BAIL(errcodeFromErrno(), NULL);
        } /* if */
    } /* if */

    retval = (int *) allocator.Malloc(sizeof (int));
    if (retval == NULL)
    {
        sceIoClose(fd);
        BAIL(PHYSFS_ERR_OUT_OF_MEMORY, NULL);
    } /* if */

    printf("in doOpen, fd: %i\n", fd);
    int cur = sceIoLseek(fd, 0, SEEK_CUR);
    printf("in doOpen, cur: %i\n", cur);
    int len = sceIoLseek(fd, 0, SEEK_END);
    printf("in doOpen len: %i\n", len);

    sceIoLseek(fd, cur, SEEK_SET);

    *retval = fd;
    return((void *) retval);
} /* doOpen */


void *__PHYSFS_platformOpenRead(const char *filename)
{
    return(doOpen(filename, PSP_O_RDONLY));
} /* __PHYSFS_platformOpenRead */


void *__PHYSFS_platformOpenWrite(const char *filename)
{
    return(doOpen(filename, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC));
} /* __PHYSFS_platformOpenWrite */


void *__PHYSFS_platformOpenAppend(const char *filename)
{
    return(doOpen(filename, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_APPEND));
} /* __PHYSFS_platformOpenAppend */


PHYSFS_sint64 __PHYSFS_platformRead(void *opaque, void *buffer,
                                    PHYSFS_uint64 len)
{
    const int fd = *((int *) opaque);
    int rc = 0;

    if (!__PHYSFS_ui64FitsAddressSpace(len))
        BAIL(PHYSFS_ERR_INVALID_ARGUMENT, -1);

    rc = sceIoRead(fd, buffer, (size_t) len);
    BAIL_IF(rc == -1, errcodeFromErrno(), -1);
    assert(rc >= 0);
    assert(rc <= len);
    return (PHYSFS_sint64) rc;
} /* __PHYSFS_platformRead */


PHYSFS_sint64 __PHYSFS_platformWrite(void *opaque, const void *buffer,
                                     PHYSFS_uint64 len)
{
    const int fd = *((int *) opaque);
    int rc = 0;

    if (!__PHYSFS_ui64FitsAddressSpace(len))
        BAIL(PHYSFS_ERR_INVALID_ARGUMENT, -1);

    rc = sceIoWrite(fd, (void *) buffer, (size_t) len);

    BAIL_IF(rc == -1, errcodeFromErrno(), rc);
    assert(rc >= 0);
    assert(rc <= len);
    return (PHYSFS_sint64) rc;
} /* __PHYSFS_platformWrite */


int __PHYSFS_platformSeek(void *opaque, PHYSFS_uint64 pos)
{
    int fd = *((int *) opaque);

    #ifdef PHYSFS_HAVE_LLSEEK
      unsigned long offset_high = ((pos >> 32) & 0xFFFFFFFF);
      unsigned long offset_low = (pos & 0xFFFFFFFF);
      loff_t retoffset;
      int rc = llseek(fd, offset_high, offset_low, &retoffset, SEEK_SET);
      BAIL_IF(rc == -1, errcodeFromErrno(), 0);
    #else
      BAIL_IF(sceIoLseek(fd, (int) pos, SEEK_SET) == -1, errcodeFromErrno(), 0);
    #endif

    return(1);
} /* __PHYSFS_platformSeek */


PHYSFS_sint64 __PHYSFS_platformTell(void *opaque)
{
    int fd = *((int *) opaque);
    PHYSFS_sint64 retval;

    #ifdef PHYSFS_HAVE_LLSEEK
      loff_t retoffset;
      int rc = llseek(fd, 0, &retoffset, SEEK_CUR);
      BAIL_IF(rc == -1, errno, -1);
      retval = (PHYSFS_sint64) retoffset;
    #else
      retval = (PHYSFS_sint64) sceIoLseek(fd, 0, SEEK_CUR);

      BAIL_IF(retval == -1, errno, -1);
    #endif

    return(retval);
} /* __PHYSFS_platformTell */


PHYSFS_sint64 __PHYSFS_platformFileLength(void *opaque)
{
    int fd = *((int *) opaque);
    int cur = sceIoLseek(fd, 0, SEEK_CUR);

    // printf("in fileLength, fd: %i\n", fd);
    // printf("in fileLength, cur: %i\n", cur);

    int len = sceIoLseek(fd, 0, SEEK_END);

    BAIL_IF(len < 0, errno, -1);

    printf("in fileLength, len: %i\n", len);

    sceIoLseek(fd, cur, SEEK_SET);
    return (PHYSFS_sint64)len;
} /* __PHYSFS_platformFileLength */


int __PHYSFS_platformEOF(void *opaque)
{
    PHYSFS_sint64 pos = __PHYSFS_platformTell(opaque);
    PHYSFS_sint64 len = __PHYSFS_platformFileLength(opaque);
    return((pos < 0) || (len < 0) || (pos >= len));
} /* __PHYSFS_platformEOF */


int __PHYSFS_platformFlush(void *opaque)
{
    BAIL_IF(sceIoSync("ms0:", 0) < -1, PHYSFS_ERR_IO, 0);
    return(1);
} /* __PHYSFS_platformFlush */


void __PHYSFS_platformClose(void *opaque)
{
    int fd = *((int *) opaque);
    BAIL_IF(sceIoClose(fd) == -1, errno, 0);
    allocator.Free(opaque);
} /* __PHYSFS_platformClose */


int __PHYSFS_platformDelete(const char *path)
{
    BAIL_IF(sceIoRemove(path) == -1, errno, 0);
    return(1);
} /* __PHYSFS_platformDelete */


PHYSFS_sint64 __PHYSFS_platformGetLastModTime(const char *fname)
{
    return (PHYSFS_sint64)-1;
} /* __PHYSFS_platformGetLastModTime */

#ifdef PHYSFS_NO_THREAD_SUPPORT
void *__PHYSFS_platformGetThreadID(void) { return((void *) 0x0001); }
void *__PHYSFS_platformCreateMutex(void) { return((void *) 0x0001); }
void __PHYSFS_platformDestroyMutex(void *mutex) {}
int __PHYSFS_platformGrabMutex(void *mutex) { return(1); }
void __PHYSFS_platformReleaseMutex(void *mutex) {}
#endif

#ifdef PHYSFS_NO_CDROM_SUPPORT

/* Stub version for platforms without CD-ROM support. */
void __PHYSFS_platformDetectAvailableCDs(PHYSFS_StringCallback cb, void *data)
{
} /* __PHYSFS_platformDetectAvailableCDs */

#endif

char *__PHYSFS_platformRealPath(const char *path)
{
    char *ret = allocator.Malloc(strlen(path)+1);
    if (ret != NULL)
        strcpy(ret, path);
    return(ret);
} /* __PHYSFS_platformRealPath */

char *__PHYSFS_platformCalcBaseDir(const char *argv0)
{
    char *ret = allocator.Malloc(37);
    if (ret != NULL)
        strcpy(ret, "ms0:/PSP/GAME/");
    return(ret);
}

char *__PHYSFS_platformCalcPrefDir(const char *org, const char *app)
{
    char *ret = allocator.Malloc(37);
    if (ret != NULL)
        strcpy(ret, "ms0:/PSP/GAME/");
    return(ret);
}

int __PHYSFS_platformSetDefaultAllocator(PHYSFS_Allocator *a)
{
    return(0);  /* just use malloc() and friends. */
} /* __PHYSFS_platformSetDefaultAllocator */

#endif  /* PHYSFS_PLATFORM_PSP */

/* end of psp.c ... */

