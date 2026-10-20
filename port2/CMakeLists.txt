# Native-pointer port of Tombi! (M3: the game C compiled as ordinary C, the GTE/GPU/SPU/CD/pad emulated in-process,
# SDL2 window).  See docs/PORT.md "M3".
#
#   python3 tools/build_full.py -j 4                 # once: the generator takes retail addresses from it
#   cmake -S port2 -B build/port2 -G Ninja && cmake --build build/port2 -j 8
#   build/port2/tombi2                                # SDL2 window, paced at 50 Hz
#   build/port2/tombi2 --headless --frames 1500 --dump-frames out --pad-script 1000:start
#   cat build/port2/gen/report.txt
cmake_minimum_required(VERSION 3.16)
project(tombi_port2 C ASM)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(GEN ${CMAKE_CURRENT_BINARY_DIR}/gen)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# 1. filtered sources + address resolution (re-run on every configure)
set(PORT2_PROGS MAIN0 X000 X001 X002 X003 X004 X005 X006 X008 X009 X010 X011 X013 X014 X016 X017 X018 X019
    O0PTSUB0 D0SPSUB D0SPSUB2 D0SPSUB3 G0OVER I0NFO I0NFO2 I0NFO3 CACHE STRING "programs whose C is compiled into port2")
execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py sources ${GEN} ${PORT2_PROGS}
                RESULT_VARIABLE gen_rc)
if(gen_rc)
  message(FATAL_ERROR "gen2.py sources failed")
endif()
include(${GEN}/sources.cmake)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
             ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py ${ROOT}/port/tools/gen_port.py ${ROOT}/port/fixes.txt
             ${ROOT}/build/full/main0.full.ld)

# 2. phase 1 is a 32-bit build with the normal libc (the game's structs hold 32-bit pointers); the image is linked
#    below 16 MB so the 24-bit ordering-table links are exact (port_prelude.h).  No freestanding tricks.
set(PORT2_ARCH_FLAGS -m32 -march=i686 -fno-pic -fno-pie -fno-omit-frame-pointer)
set(PORT2_GAME_FLAGS -O1 -w -funsigned-char -fwrapv -fno-strict-aliasing -fno-delete-null-pointer-checks
    -fno-builtin -DSKIP_ASM -D_LANGUAGE_C -DMIPSEL)

add_library(game2 OBJECT ${PORT_GAME_SOURCES})
target_include_directories(game2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN} ${GEN}/inc ${GEN}/inc/tomba)
target_compile_options(game2 PRIVATE ${PORT2_ARCH_FLAGS} ${PORT2_GAME_FLAGS})

add_library(game2_24 OBJECT ${PORT_GAME_SOURCES_T24})
target_include_directories(game2_24 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN} ${GEN}/inc24 ${GEN}/inc24/tomba)
target_compile_options(game2_24 PRIVATE ${PORT2_ARCH_FLAGS} ${PORT2_GAME_FLAGS})

# the runtime, the shims and the tables gen_port wrote at configure time (funcmap.c: retail address -> native
# function, the overlay set, the areas and their signature words)
file(GLOB PORT2_RT ${CMAKE_CURRENT_SOURCE_DIR}/runtime/*.c ${CMAKE_CURRENT_SOURCE_DIR}/shims/*.c)
add_library(port2rt OBJECT ${PORT2_RT} ${GEN}/funcmap.c)
target_include_directories(port2rt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(port2rt PRIVATE ${PORT2_ARCH_FLAGS} -O2 -Wall -Wno-unused-function)
# SDL2 for the 32-bit build: the runtime library is enough (the headers are architecture independent)
find_library(PORT2_SDL2 NAMES SDL2-2.0 libSDL2-2.0.so.0 PATHS /usr/lib/i386-linux-gnu /usr/lib32 NO_DEFAULT_PATH)
if(NOT PORT2_SDL2)
  find_library(PORT2_SDL2 NAMES SDL2-2.0 SDL2)
endif()
if(NOT PORT2_SDL2)
  message(FATAL_ERROR "32-bit SDL2 not found (libsdl2-2.0-0:i386)")
endif()
# Debian keeps SDL_config.h per architecture; a 32-bit build on a 64-bit host takes the host's copy with the
# pointer size corrected (the rest of it is feature flags of the same library build)
find_file(PORT2_SDL2_CONFIG NAMES SDL2/_real_SDL_config.h SDL2/SDL_config.h
          PATHS /usr/include/i386-linux-gnu /usr/include/x86_64-linux-gnu /usr/include NO_DEFAULT_PATH)
if(PORT2_SDL2_CONFIG)
  file(READ ${PORT2_SDL2_CONFIG} sdlcfg)
  string(REPLACE "#define SIZEOF_VOIDP 8" "#define SIZEOF_VOIDP 4" sdlcfg "${sdlcfg}")
  file(WRITE ${GEN}/sdlinc/SDL2/_real_SDL_config.h "${sdlcfg}")
  get_filename_component(sdlcfgdir ${PORT2_SDL2_CONFIG} DIRECTORY)
  foreach(h SDL_platform.h begin_code.h close_code.h)
    if(EXISTS ${sdlcfgdir}/${h})          # (symlinks into /usr/include/SDL2: copy the content)
      file(READ ${sdlcfgdir}/${h} hdr)
      file(WRITE ${GEN}/sdlinc/SDL2/${h} "${hdr}")
    endif()
  endforeach()
  target_include_directories(port2rt PRIVATE ${GEN}/sdlinc ${GEN}/sdlinc/SDL2)
endif()

# 3. resolve the rest against the retail symbol table: data image (from the user's MAIN0.EXE/X000.BIN), aliases,
#    stubs.  All three are generated into the build dir; data.c is game data and never leaves it.
file(GENERATE OUTPUT ${GEN}/objects.txt
     CONTENT "$<JOIN:$<TARGET_OBJECTS:game2>,\n>\n$<JOIN:$<TARGET_OBJECTS:game2_24>,\n>\n$<JOIN:$<TARGET_OBJECTS:port2rt>,\n>\n")
add_custom_command(OUTPUT ${GEN}/syms.ld ${GEN}/stubs.c ${GEN}/data.c ${GEN}/thunks.S ${GEN}/addrmap.c ${GEN}/report.txt
                   COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py link ${GEN} ${CMAKE_NM} ${GEN}/objects.txt
                   DEPENDS game2 game2_24 port2rt $<TARGET_OBJECTS:game2> $<TARGET_OBJECTS:game2_24> $<TARGET_OBJECTS:port2rt>
                           ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py
                   COMMENT "Resolving game symbols: data image, aliases, dispatch thunks, stubs")
add_library(port2gen OBJECT ${GEN}/stubs.c ${GEN}/data.c ${GEN}/thunks.S ${GEN}/addrmap.c)
set_source_files_properties(${GEN}/stubs.c ${GEN}/data.c ${GEN}/thunks.S ${GEN}/addrmap.c PROPERTIES GENERATED TRUE)
target_include_directories(port2gen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN})
target_compile_options(port2gen PRIVATE ${PORT2_ARCH_FLAGS} -O0 -w)

add_executable(tombi2 $<TARGET_OBJECTS:game2> $<TARGET_OBJECTS:game2_24> $<TARGET_OBJECTS:port2rt> $<TARGET_OBJECTS:port2gen>)
set_target_properties(tombi2 PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS ${GEN}/syms.ld)
target_link_options(tombi2 PRIVATE -m32 -no-pie -Wl,-Ttext-segment=0x100000 -Wl,-z,noexecstack ${GEN}/syms.ld)
target_link_libraries(tombi2 PRIVATE ${PORT2_SDL2} m)
