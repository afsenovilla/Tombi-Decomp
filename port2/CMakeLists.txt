# Native-pointer port of Tombi! (M3: the game C compiled as ordinary C, the GTE/GPU/SPU/CD/pad emulated in-process,
# SDL2 window).  See docs/PORT.md "M3".
#
#   python3 tools/build_full.py -j 4                 # once: the generator takes retail addresses from it
#   cmake -S port2 -B build/port2 -G Ninja && cmake --build build/port2 -j 8
#   build/port2/tombi2                                # SDL2 window, paced at 50 Hz
#   build/port2/tombi2 --headless --frames 1500 --dump-frames out --pad-script 1000:start
#   cat build/port2/gen/report.txt
# Windows (cross-compiled with mingw-w64, docs/PORT.md "M3 phase 3"):
#   cmake -S port2 -B build/port2-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=port2/cmake/mingw-i686.cmake \
#         -DPORT_SDL2_MINGW=/path/to/SDL2-2.30.x/i686-w64-mingw32 && cmake --build build/port2-win -j 8
#   -> build/port2-win/tombi2.exe (needs SDL2.dll from the same SDL2 folder next to it)
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
if(WIN32)
  list(APPEND PORT2_ARCH_FLAGS -mno-ms-bitfields)     # gcc's bitfield layout, as on Linux
  # C99 printf (%lld) from mingw's own stdio for the runtime only: the game's printf must stay the shim of libc.c
  set(PORT2_RT_FLAGS -D__USE_MINGW_ANSI_STDIO=1)
endif()
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
# build id (git short sha, "+" when the tree is modified): the first log line says which checkout the binary is from
add_custom_command(OUTPUT ${GEN}/build_id.stamp
                   COMMAND ${CMAKE_COMMAND} -DSRC=${ROOT} -DOUT=${GEN}/build_id.c -P ${ROOT}/port/cmake/build_id.cmake
                   COMMAND ${CMAKE_COMMAND} -E touch ${GEN}/build_id.stamp
                   BYPRODUCTS ${GEN}/build_id.c COMMENT "build id" VERBATIM)
add_custom_target(port2_build_id ALL DEPENDS ${GEN}/build_id.stamp)
execute_process(COMMAND ${CMAKE_COMMAND} -DSRC=${ROOT} -DOUT=${GEN}/build_id.c -P ${ROOT}/port/cmake/build_id.cmake)
add_library(port2rt OBJECT ${PORT2_RT} ${GEN}/funcmap.c ${GEN}/build_id.c)
add_dependencies(port2rt port2_build_id)
target_include_directories(port2rt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(port2rt PRIVATE ${PORT2_ARCH_FLAGS} ${PORT2_RT_FLAGS} -O2 -Wall -Wno-unused-function)
if(WIN32)
  # -DPORT_SDL2_MINGW=DIR names the i686-w64-mingw32 folder of SDL2-devel-2.30.x-mingw.tar.gz (never vendored:
  # .github/workflows/port2-exe.yml downloads it); tombi2.exe needs its bin/SDL2.dll next to it
  set(PORT_SDL2_MINGW "" CACHE PATH "SDL2 mingw development folder (.../i686-w64-mingw32)")
  if(NOT EXISTS ${PORT_SDL2_MINGW}/include/SDL2/SDL.h)
    message(FATAL_ERROR "-DPORT_SDL2_MINGW must name the i686-w64-mingw32 folder of the SDL2 mingw development tarball")
  endif()
  target_include_directories(port2rt PRIVATE ${PORT_SDL2_MINGW}/include)
  set(PORT2_SDL2 -L${PORT_SDL2_MINGW}/lib -lSDL2 -lwinmm)
else()
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
# (-fno-zero-initialized-in-bss: the zero regions of the image, port_scratch and port_io, stay in .data right after
#  port_ram, below the runtime's large .bss buffers, so the whole image sits below 16 MB on Windows too)
target_compile_options(port2gen PRIVATE ${PORT2_ARCH_FLAGS} -O0 -w -fno-zero-initialized-in-bss)

add_executable(tombi2 $<TARGET_OBJECTS:game2> $<TARGET_OBJECTS:game2_24> $<TARGET_OBJECTS:port2rt> $<TARGET_OBJECTS:port2gen>)
set_target_properties(tombi2 PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS ${GEN}/syms.ld)
if(WIN32)
  # a console program (the launcher captures stderr; no SDL2main: main() is the plain C one); based at 4 MB with
  # ASLR off so the data image stays below 16 MB (the 24-bit OT links); 16 MB main stack; no libgcc DLL
  target_link_options(tombi2 PRIVATE -m32 -mconsole -static-libgcc -Wl,--image-base,0x400000 -Wl,--disable-dynamicbase
                      -Wl,--stack,16777216 ${GEN}/syms.ld)
  target_link_libraries(tombi2 PRIVATE ${PORT2_SDL2})
else()
target_link_options(tombi2 PRIVATE -m32 -no-pie -Wl,-Ttext-segment=0x100000 -Wl,-z,noexecstack ${GEN}/syms.ld)
target_link_libraries(tombi2 PRIVATE ${PORT2_SDL2} m pthread)
endif()
