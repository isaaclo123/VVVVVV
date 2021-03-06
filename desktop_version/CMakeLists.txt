# CMake File for VVVVVV
# Written by Ethan "flibitijibibo" Lee

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/")
SET(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/PSP.cmake")

# Set helper variables:
SET(PSPDEV $ENV{PSPDEV})
SET(PSPSDK ${PSPDEV}/psp/sdk)
SET(PSPBIN ${PSPDEV}/bin)
SET(PSPCMAKE ${PSPDEV}/psp/share/cmake)

# link_directories(
#     ${PSPSDK}/lib
#     ${PSPDEV}/lib
#     ${PSPDEV}/psp/lib
# )

# Set SDL variables

# add_definitions(-DSDL_INCLUDE_DIRS=${PSPDEV}/psp/include/SDL)
# add_definitions(-DSDL_LIBRARIES=${PSPDEV}/psp/lib/libSDL.a;${PSPDEV}/psp/lib/libSDLmain.a;${PSPDEV}/psp/lib/libSDL_mixer.a



# CMake Options
OPTION(ENABLE_WARNINGS "Enable compilation warnings" ON)
OPTION(ENABLE_WERROR "Treat compilation warnings as errors" OFF)

SET(CUSTOM_LEVEL_SUPPORT ENABLED CACHE STRING "Optionally disable playing and/or editing of custom levels")
SET_PROPERTY(CACHE CUSTOM_LEVEL_SUPPORT PROPERTY STRINGS ENABLED NO_EDITOR DISABLED)

SET(STEAM OFF CACHE BOOL "Use the Steam API")
SET(GOG OFF CACHE BOOL "Use the GOG API")

SET(OFFICIAL_BUILD OFF CACHE BOOL "Compile an official build of the game")

SET(MAKEANDPLAY OFF CACHE BOOL "Compile a version of the game without the main campaign (provided for convenience; consider modifying MakeAndPlay.h instead")

IF(OFFICIAL_BUILD AND NOT MAKEANDPLAY)
        SET(STEAM ON)
        SET(GOG ON)
ENDIF()

IF(MAKEANDPLAY)
        ADD_DEFINITIONS(-DMAKEANDPLAY)
ENDIF()

# Set standard to C++98/C++03
SET(CMAKE_CXX_STANDARD 98)
SET(CMAKE_CXX_EXTENSIONS OFF) # prevent mixing stdlib implementations (dangerous!)

# Architecture Flags
IF(APPLE)
        # Wow, Apple is a huge jerk these days huh?
        SET(OSX_10_9_SDK_PATH /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk)
        IF(NOT CMAKE_OSX_SYSROOT)
                IF(IS_DIRECTORY ${OSX_10_9_SDK_PATH})
                        SET(CMAKE_OSX_SYSROOT ${OSX_10_9_SDK_PATH})
                ELSE()
                        MESSAGE(WARNING "CMAKE_OSX_SYSROOT not set and macOS 10.9 SDK not found! Using default one.")
                ENDIF()
        ENDIF()
        SET(CMAKE_OSX_DEPLOYMENT_TARGET 10.9)
        LINK_DIRECTORIES(/usr/local/lib)
        ADD_COMPILE_OPTIONS(-Werror=partial-availability)
ENDIF()

PROJECT(VVVVVV)


# -g for debug

ADD_COMPILE_OPTIONS(-g)

ADD_COMPILE_OPTIONS("-D__POSIX_C_VISIBLE=200809")

ADD_DEFINITIONS(-D_GNU_SOURCE=1 -DVVV_COMPILEMUSIC)
# ADD_DEFINITIONS(-Dmain=SDL_main)

IF(APPLE)
        MESSAGE(STATUS "Using macOS SDK at ${CMAKE_OSX_SYSROOT}")
ENDIF()

# Flags
ADD_DEFINITIONS(-DPHYSFS_SUPPORTS_DEFAULT=0 -DPHYSFS_SUPPORTS_ZIP=1)

# RPATH
IF(NOT WIN32)
        IF(APPLE)
                SET(BIN_LIBROOT "osx")
                SET(BIN_RPATH "@executable_path/osx")
        ELSEIF(CMAKE_SIZEOF_VOID_P MATCHES "8")
                SET(BIN_LIBROOT "lib64")
                SET(BIN_RPATH "\$ORIGIN/lib64")
        ELSE()
                SET(BIN_LIBROOT "lib")
                SET(BIN_RPATH "\$ORIGIN/lib")
        ENDIF()
        SET(CMAKE_SKIP_BUILD_RPATH TRUE)
        SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
        SET(CMAKE_INSTALL_RPATH ${BIN_RPATH})
        SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
ENDIF()

# Include Directories
INCLUDE_DIRECTORIES(
        src
        # ../third_party/snprintf
        ../third_party/MElib/include
        ../third_party/MElib/include/driver # ../third_party/MElib
        # ../third_party/MElib/src/driver
        ../third_party/tinyxml
        ../third_party/physfs
        ../third_party/lodepng
        ../third_party/utfcpp/source
)

# Source Lists
# SET(MELIB_SRC
#     ../third_party/MElib/src/MediaEngine.S
#     ../third_party/MElib/src/me.c
#     ../third_party/MElib/src/melib.c
#     ../third_party/MElib/src/driver/MediaEngine.S
#     ../third_party/MElib/src/driver/me.c
#     ../third_party/MElib/src/driver/me_stub.S
#     )

SET(VVV_SRC
    # ../third_party/MElib/src/me.c
    # ../third_party/MElib/src/melib.c
    # ../third_party/MElib/src/MediaEngine.S

        src/BlockV.cpp
        src/editor.cpp
        src/Ent.cpp
        src/Entity.cpp
        src/FileSystemUtils.cpp
        src/Finalclass.cpp
        src/Game.cpp
        src/Graphics.cpp
        src/GraphicsResources_psp.cpp
        src/GraphicsUtil_psp.cpp
        src/Input_psp.cpp
        src/KeyPoll_psp.cpp
        src/Labclass.cpp
        src/Logic.cpp
        src/main.cpp
        src/Map.cpp
        src/Music.cpp
        # src/Music.cpp
        src/Otherlevel.cpp
        src/preloader.cpp
        src/Render.cpp
        src/Screen_psp.cpp
        src/Script.cpp
        src/Scripts.cpp
        src/SoundSystem.cpp
        src/Spacestation2.cpp
        src/TerminalScripts.cpp
        src/Textbox.cpp
        src/Tower.cpp
        src/UtilityClass.cpp
        src/WarpClass.cpp
)
IF(NOT CUSTOM_LEVEL_SUPPORT STREQUAL "DISABLED")
        LIST(APPEND VVV_SRC src/editor.cpp)
ENDIF()
IF(STEAM)
        LIST(APPEND VVV_SRC src/SteamNetwork.c)
        ADD_DEFINITIONS(-DSTEAM_NETWORK)
ENDIF()
IF(GOG)
        LIST(APPEND VVV_SRC src/GOGNetwork.c)
        ADD_DEFINITIONS(-DGOG_NETWORK)
ENDIF()

SET(XML_SRC
        ../third_party/tinyxml/tinystr.cpp
        ../third_party/tinyxml/tinyxml.cpp
        ../third_party/tinyxml/tinyxmlerror.cpp
        ../third_party/tinyxml/tinyxmlparser.cpp
        ../third_party/tinyxml/snprintf.cpp
)

SET(PFS_SRC
    ../third_party/physfs/physfs.c
    ../third_party/physfs/physfs_archiver_dir.c
    ../third_party/physfs/physfs_archiver_unpacked.c
    ../third_party/physfs/physfs_archiver_zip.c
    ../third_party/physfs/physfs_byteorder.c
    ../third_party/physfs/physfs_unicode.c
    ../third_party/physfs/physfs_platform_psp.c
    ../third_party/physfs/physfs_platform_posix.c
    ../third_party/physfs/physfs_platform_unix.c
    ../third_party/physfs/physfs_platform_windows.c
    ../third_party/physfs/physfs_platform_haiku.cpp
)
IF(APPLE)
        # Are you noticing a pattern with this Apple crap yet?
        SET(PFS_SRC ${PFS_SRC} ../third_party/physfs/physfs_platform_apple.m)
ENDIF()
SET(PNG_SRC ../third_party/lodepng/lodepng.c)

# Executable information
IF(WIN32)
        ADD_EXECUTABLE(VVVVVV WIN32 ${VVV_SRC})
ELSE()
        ADD_EXECUTABLE(VVVVVV ${VVV_SRC})
ENDIF()

# ADD_EXECUTABLE(MediaEngine ${MEDIAENGINE_SRC})

IF(NOT OFFICIAL_BUILD)
        # Add interim commit hash and its date to the build

        # FIND_PACKAGE sets GIT_FOUND and GIT_EXECUTABLE
        FIND_PACKAGE(Git)

        IF(GIT_FOUND)
                # These filenames have to be qualified, because when we run
                # the CMake script, its work dir gets set to the build folder
                SET(VERSION_INPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/Version.h.in)
                SET(VERSION_OUTPUT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/Version.h.out)

                ADD_CUSTOM_TARGET(
                        GenerateVersion ALL
                        # This BYPRODUCTS line is required for this to be ran every time
                        BYPRODUCTS ${VERSION_OUTPUT_FILE}
                        COMMAND ${CMAKE_COMMAND}
                        # These args have to be passed through, otherwise the script can't see them
                        # Also, these args have to come BEFORE `-P`! (Otherwise it fails with an unclear error)
                        -DGIT_EXECUTABLE=${GIT_EXECUTABLE}
                        -DINPUT_FILE=${VERSION_INPUT_FILE}
                        -DOUTPUT_FILE=${VERSION_OUTPUT_FILE}
                        -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/version.cmake
                )

                ADD_DEPENDENCIES(VVVVVV GenerateVersion)

                # This lets Version.h know that Version.h.out exists
                ADD_DEFINITIONS(-DVERSION_H_OUT_EXISTS)
        ENDIF()
ENDIF()

# Build options
IF(ENABLE_WARNINGS)
        # The weird syntax is due to CMake generator expressions.
        # Saves quite a few lines and boilerplate at the price of readability.
        TARGET_COMPILE_OPTIONS(VVVVVV PRIVATE
                $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:
                        -Wall $<$<BOOL:${ENABLE_WERROR}>:-Werror>>
                $<$<CXX_COMPILER_ID:MSVC>:
                        /W4 $<$<BOOL:${ENABLE_WERROR}>:/WX>>)
ENDIF()

IF(CUSTOM_LEVEL_SUPPORT STREQUAL "NO_EDITOR")
        ADD_DEFINITIONS(-DNO_EDITOR)
ELSEIF(CUSTOM_LEVEL_SUPPORT STREQUAL "DISABLED")
        ADD_DEFINITIONS(-DNO_CUSTOM_LEVELS -DNO_EDITOR)
ENDIF()

IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        SET(SUPPORTS_IMPLICIT_FALLTHROUGH 1)
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        IF(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
                SET(SUPPORTS_IMPLICIT_FALLTHROUGH 1)
        ELSE()
                SET(SUPPORTS_IMPLICIT_FALLTHROUGH 0)
        ENDIF()
ELSE()
        SET(SUPPORTS_IMPLICIT_FALLTHROUGH 0)
ENDIF()


IF(SUPPORTS_IMPLICIT_FALLTHROUGH)
        TARGET_COMPILE_OPTIONS(VVVVVV PRIVATE -Werror=implicit-fallthrough)
ENDIF()

SET_PROPERTY(TARGET VVVVVV PROPERTY CXX_STANDARD 98)
SET_PROPERTY(TARGET VVVVVV PROPERTY CXX_EXTENSIONS FALSE)

ADD_LIBRARY(tinyxml-static STATIC ${XML_SRC})
target_precompile_headers(tinyxml-static
    PRIVATE ../third_party/tinyxml/config.h
    PRIVATE ../third_party/tinyxml/tinyxml.h
)

ADD_LIBRARY(physfs-static STATIC ${PFS_SRC})
ADD_LIBRARY(lodepng-static STATIC ${PNG_SRC})

# Static Dependencies
TARGET_LINK_LIBRARIES(
    VVVVVV
    ../third_party/MElib/lib/libME.a
    physfs-static
    tinyxml-static
    lodepng-static
    )

# SDL Dependency (Detection pulled from FAudio)
if (DEFINED SDL_INCLUDE_DIRS AND DEFINED SDL_LIBRARIES)
        message(STATUS "Using pre-defined SDL variables SDL_INCLUDE_DIRS and SDL_LIBRARIES")
        target_include_directories(VVVVVV SYSTEM PUBLIC "$<BUILD_INTERFACE:${SDL_INCLUDE_DIRS}>")
        # target_link_libraries(VVVVVV ${SDL_LIBRARIES})
        # link_directories(AFTER VVVVVV ${SDL_INCLUDE_DIRS})
else()
        # Only try to autodetect if both SDL variables aren't explicitly set
        find_package(SDL CONFIG)
        if (TARGET SDL::SDL)
                message(STATUS "Using TARGET SDL::SDL")
                target_link_libraries(VVVVVV SDL::SDL SDL_mixer)
        elseif (TARGET SDL)
                message(STATUS "Using TARGET SDL")
                target_link_libraries(VVVVVV SDL SDL_mixer)
        else()
                message(STATUS "No TARGET SDL::SDL, or SDL, using variables")
                find_path(SDL_MIXER_INCLUDE_DIRS NAMES SDL_mixer.h PATH_SUFFIXES SDL)
                target_include_directories(VVVVVV SYSTEM PUBLIC "$<BUILD_INTERFACE:${SDL_INCLUDE_DIRS}>" ${SDL_MIXER_INCLUDE_DIRS})
                target_link_libraries(VVVVVV ${SDL_LIBRARIES} SDL_mixer)
        endif()
endif()

# Yes, more Apple Crap
IF(APPLE)
        FIND_LIBRARY(FOUNDATION NAMES Foundation)
        FIND_LIBRARY(IOKIT NAMES IOKit)
        TARGET_LINK_LIBRARIES(VVVVVV objc ${IOKIT} ${FOUNDATION})
ENDIF()
# But hey, also some Haiku crap
IF(HAIKU)
        FIND_LIBRARY(BE_LIBRARY be)
        FIND_LIBRARY(ROOT_LIBRARY root)
        TARGET_LINK_LIBRARIES(VVVVVV ${BE_LIBRARY} ${ROOT_LIBRARY})
ENDIF()
