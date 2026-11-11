# Native-pointer port of Tombi! (M3: the game C compiled as ordinary C, the GTE/GPU/SPU/CD/pad emulated in-process,
# SDL2 window).  See docs/PORT.md "M3".
#
#   python3 tools/build_full.py -j 4                 # once: the generator takes retail addresses from it
#   cmake -S port2 -B build/port2 -G Ninja && cmake --build build/port2 -j 8
#   python3 tools/extract_data.py game/disc/Tombi.bin data   # once: the game files the exe reads (docs/PORT.md "Running without the disc image")
#   build/port2/tombi2                                # SDL2 window, paced at 50 Hz, OpenGL renderer (--renderer soft: software); reads data/ (--data DIR, or --disc IMAGE)
#   build/port2/tombi2 --headless --frames 1500 --dump-frames out --pad-script 1000:start
#   cat build/port2/gen/report.txt
# Android (armeabi-v7a libtombi.so + libmain.so, built by Gradle from port2/android: docs/PORT.md "4. Android",
#   .github/workflows/android-apk.yml); needs -DPORT2_SDL2_SOURCE=SDL2-2.30.x source tree.
# Windows (cross-compiled with mingw-w64, docs/PORT.md "M3 phase 3"):
#   cmake -S port2 -B build/port2-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=port2/cmake/mingw-i686.cmake \
#         -DPORT_SDL2_MINGW=/path/to/SDL2-2.30.x/i686-w64-mingw32 && cmake --build build/port2-win -j 8
#   -> build/port2-win/tombi2.exe (needs SDL2.dll from the same SDL2 folder next to it)
cmake_minimum_required(VERSION 3.16)
project(tombi_port2 C ASM)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(GEN ${CMAKE_CURRENT_BINARY_DIR}/gen)

# Android arm64-v8a (docs/PORT.md "64-bit builds: wasm2c"): the game's structs hold 32-bit pointers, so the 64-bit build
# is the wasm2c one: the same generated game C + runtime compiled to wasm32 and translated back to portable C by
# port2/wasm/build.py --until w2c (run before Gradle; -DPORT2_W2C_DIR, default build/wasm/w2c), compiled here with the
# native host (port2/wasm/host*.c: WASI subset, coroutines, GL imports) and runtime/sdl.c.  Same libraries as the
# armeabi-v7a build: libtombi.so (entry port_main), libmain.so (loader.c), libSDL2.so, libtombidisc.so.
if(ANDROID AND (ANDROID_ABI STREQUAL "arm64-v8a" OR CMAKE_ANDROID_ARCH_ABI STREQUAL "arm64-v8a"))
  set(PORT2_W2C_DIR ${ROOT}/build/wasm/w2c CACHE PATH "wasm2c output of port2/wasm/build.py --until w2c")
  if(NOT EXISTS ${PORT2_W2C_DIR}/tombi.h)
    message(FATAL_ERROR "arm64-v8a needs the wasm2c build first: python3 port2/wasm/build.py --until w2c (-DPORT2_W2C_DIR=${PORT2_W2C_DIR})")
  endif()
  set(PORT2_SDL2_SOURCE "" CACHE PATH "SDL2 source tree (Android)")
  if(NOT EXISTS ${PORT2_SDL2_SOURCE}/include/SDL.h)
    message(FATAL_ERROR "-DPORT2_SDL2_SOURCE must name an SDL2 source tree (SDL2-2.30.x)")
  endif()
  set(SDL_STATIC OFF CACHE BOOL "" FORCE)
  set(SDL_SHARED ON CACHE BOOL "" FORCE)
  set(SDL_TEST OFF CACHE BOOL "" FORCE)
  add_subdirectory(${PORT2_SDL2_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/sdl2 EXCLUDE_FROM_ALL)
  file(GLOB sdlhdrs ${PORT2_SDL2_SOURCE}/include/*.h)
  file(COPY ${sdlhdrs} DESTINATION ${GEN}/sdlinc/SDL2)
  file(GLOB PORT2_W2C_SRCS ${PORT2_W2C_DIR}/*.c)
  set(W ${CMAKE_CURRENT_SOURCE_DIR}/wasm)
  add_library(tombi2 SHARED ${PORT2_W2C_SRCS} ${W}/host.c ${W}/host_sdl.c ${W}/host_gl.c ${CMAKE_CURRENT_SOURCE_DIR}/runtime/sdl.c)
  set_target_properties(tombi2 PROPERTIES OUTPUT_NAME tombi LINKER_LANGUAGE C)
  target_include_directories(tombi2 PRIVATE ${PORT2_W2C_DIR} ${W} ${GEN}/sdlinc ${CMAKE_CURRENT_SOURCE_DIR}/include)
  target_compile_options(tombi2 PRIVATE -O2 -fPIC -fno-strict-aliasing -fwrapv -ffp-contract=off -w)
  target_compile_definitions(tombi2 PRIVATE WASM_RT_NONCONFORMING_UNCHECKED_STACK_EXHAUSTION=1 WASM_RT_SKIP_SIGNAL_RECOVERY=1
                             WASM_RT_TRAP_HANDLER=port_w2c_trap)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/runtime/sdl.c PROPERTIES COMPILE_OPTIONS "-include;${W}/sdl_bridge.h")
  target_link_options(tombi2 PRIVATE -Wl,-z,noexecstack)
  target_link_libraries(tombi2 PRIVATE SDL2 log dl m)
  find_program(PORT2_OBJCOPY NAMES llvm-objcopy HINTS ${ANDROID_TOOLCHAIN_ROOT}/bin ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin)
  if(PORT2_OBJCOPY)
    add_custom_command(TARGET tombi2 POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tombi2> ${CMAKE_CURRENT_BINARY_DIR}/libtombi.debug.so
                       COMMAND ${PORT2_OBJCOPY} --strip-debug $<TARGET_FILE:tombi2>
                       COMMENT "libtombi.so: symbol table kept, DWARF moved to libtombi.debug.so" VERBATIM)
  endif()
  add_library(main SHARED ${CMAKE_CURRENT_SOURCE_DIR}/android/loader.c)
  target_include_directories(main PRIVATE ${GEN}/sdlinc)
  target_link_libraries(main PRIVATE SDL2 log dl)
  add_dependencies(main tombi2)
  set(CHDR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libchdr)
  add_library(port2chd OBJECT ${CHDR_DIR}/unity.c ${CHDR_DIR}/src/libchdr_codec_avhuff.c)
  target_include_directories(port2chd PRIVATE ${CHDR_DIR}/include)
  target_compile_definitions(port2chd PRIVATE ZSTD_DISABLE_ASM)
  target_compile_options(port2chd PRIVATE -fPIC -O2 -w)
  add_library(tombidisc SHARED ${CMAKE_CURRENT_SOURCE_DIR}/android/disccheck.c ${CMAKE_CURRENT_SOURCE_DIR}/shims/cd_chd.c
              ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lzma-7z/lzma7z.c
              $<TARGET_OBJECTS:port2chd>)
  target_include_directories(tombidisc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CHDR_DIR}/include)
  target_compile_definitions(tombidisc PRIVATE PORT_HAVE_CHD)
  target_compile_options(tombidisc PRIVATE -O2 -fPIC)
  return()
endif()
find_package(Python3 REQUIRED COMPONENTS Interpreter)
if(ANDROID)
  # gen2.py for Android: ARM dispatch thunks, K&R declarations clang refuses, and the C library is bionic (the NDK
  # sysroot's libc.so stub), not i386 glibc
  if(NOT ANDROID_PLATFORM_LEVEL)
    string(REGEX REPLACE "^android-" "" ANDROID_PLATFORM_LEVEL "${ANDROID_PLATFORM}")
  endif()
  set(PORT2_GEN_ENV ${CMAKE_COMMAND} -E env PORT2_ARCH=arm
      PORT2_LIBC=${CMAKE_SYSROOT}/usr/lib/arm-linux-androideabi/${ANDROID_PLATFORM_LEVEL}/libc.so)
endif()

# the game version (docs/PORT_VERSIONS_PLAN.md U1, port/tools/portver.py): empty = PAL SCES-01331 (the default build),
# usa = SCUS-94236 (needs python3 tools/build_full.py --version usa first, and the USA disc's files in game_usa/fs at
# build time), jp = SLPS-01144 (build_full.py --version jp, files in game_jp); one binary per version:
# cmake -S port2 -B build/port2-scus94236 -DPORT_VERSION=usa
set(PORT_VERSION "" CACHE STRING "game version the port is generated for: empty (PAL SCES-01331), usa (SCUS-94236) or jp (SLPS-01144)")
if(PORT_VERSION)
  if(PORT2_GEN_ENV)
    list(APPEND PORT2_GEN_ENV PORT_VERSION=${PORT_VERSION})
  else()
    set(PORT2_GEN_ENV ${CMAKE_COMMAND} -E env PORT_VERSION=${PORT_VERSION})
  endif()
  string(TOLOWER "${PORT_VERSION}" _pv)
  if(_pv STREQUAL "usa" OR _pv STREQUAL "scus94236")
    set(PORT2_FULL_DIR ${ROOT}/build/scus94236/full)
    set(PORT2_FULL_EXE scus94236)
  elseif(_pv STREQUAL "jp" OR _pv STREQUAL "slps01144")
    set(PORT2_FULL_DIR ${ROOT}/build/slps01144/full)
    set(PORT2_FULL_EXE slps01144)
  else()
    message(FATAL_ERROR "PORT_VERSION=${PORT_VERSION}: not supported (empty for PAL, usa or jp)")
  endif()
else()
  set(PORT2_FULL_DIR ${ROOT}/build/full)
  set(PORT2_FULL_EXE main0)
endif()

# 1. filtered sources + address resolution (re-run on every configure)
set(PORT2_PROGS MAIN0 X000 X001 X002 X003 X004 X005 X006 X008 X009 X010 X011 X013 X014 X016 X017 X018 X019
    O0PTSUB0 D0SPSUB D0SPSUB2 D0SPSUB3 G0OVER I0NFO I0NFO2 I0NFO3 CACHE STRING "programs whose C is compiled into port2")
execute_process(COMMAND ${PORT2_GEN_ENV} ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py sources ${GEN} ${PORT2_PROGS}
                RESULT_VARIABLE gen_rc)
if(gen_rc)
  message(FATAL_ERROR "gen2.py sources failed")
endif()
include(${GEN}/sources.cmake)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
             ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py ${ROOT}/port/tools/gen_port.py ${ROOT}/port/tools/portver.py ${ROOT}/port/fixes.txt
             ${PORT2_FULL_DIR}/${PORT2_FULL_EXE}.full.ld)

# 2. phase 1 is a 32-bit build with the normal libc (the game's structs hold 32-bit pointers); the image is linked
#    below 16 MB so the 24-bit ordering-table links are exact (port_prelude.h).  No freestanding tricks.
#    Android (armeabi-v7a, docs/PORT.md "4. Android"): the same 32-bit pointer model on ARM, built as libtombi.so for
#    the SDL2 android-project; A32 code (-marm) so the null-page handler (runtime/lowmem_arm.inc) decodes one
#    instruction set; position independent (a shared library), loaded below 16 MB by port2/android/loader.c.
if(ANDROID)
  if(NOT CMAKE_ANDROID_ARCH_ABI STREQUAL "armeabi-v7a" AND NOT ANDROID_ABI STREQUAL "armeabi-v7a")
    message(FATAL_ERROR "port2's native build needs a 32-bit target (armeabi-v7a); arm64-v8a is the wasm2c build above")
  endif()
  set(PORT2_ARCH_FLAGS -marm -fPIC -fno-omit-frame-pointer)
else()
set(PORT2_ARCH_FLAGS -m32 -march=i686 -fno-pic -fno-pie -fno-omit-frame-pointer)
endif()
if(WIN32)
  list(APPEND PORT2_ARCH_FLAGS -mno-ms-bitfields)     # gcc's bitfield layout, as on Linux
  # C99 printf (%lld) from mingw's own stdio for the runtime only: the game's printf must stay the shim of libc.c
  set(PORT2_RT_FLAGS -D__USE_MINGW_ANSI_STDIO=1)
endif()
set(PORT2_GAME_FLAGS -O1 -w -funsigned-char -fwrapv -fno-strict-aliasing -fno-delete-null-pointer-checks
    -fno-builtin -DSKIP_ASM -D_LANGUAGE_C -DMIPSEL)
if(ANDROID)    # (the NDK's clang makes these errors by default; gcc warns, and -w silences them on the PC builds)
  list(APPEND PORT2_GAME_FLAGS -Wno-error=implicit-function-declaration -Wno-error=int-conversion
       -Wno-error=incompatible-function-pointer-types -Wno-error=implicit-int -Wno-error=return-type)
endif()

# (another version: its macro, VERSION_<SERIAL>, picks the version's #if branches of the shared C; sources.cmake)
foreach(d ${PORT_VERSION_DEFS})
  list(APPEND PORT2_GAME_FLAGS -D${d})
endforeach()
add_library(game2 OBJECT ${PORT_GAME_SOURCES})
target_include_directories(game2 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN} ${GEN}/inc ${GEN}/inc/tomba)
target_compile_options(game2 PRIVATE ${PORT2_ARCH_FLAGS} ${PORT2_GAME_FLAGS})
set(PORT2_GAME_OBJS $<TARGET_OBJECTS:game2> $<TARGET_OBJECTS:game2_24>)
set(PORT2_GAME_TARGETS game2 game2_24)

add_library(game2_24 OBJECT ${PORT_GAME_SOURCES_T24})
target_include_directories(game2_24 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN} ${GEN}/inc24 ${GEN}/inc24/tomba)
target_compile_options(game2_24 PRIVATE ${PORT2_ARCH_FLAGS} ${PORT2_GAME_FLAGS})
# the psx_tomba header set (`// HEADERS tomba26`: the USA ports); a PAL build has no such file
if(PORT_GAME_SOURCES_T26)
  add_library(game2_26 OBJECT ${PORT_GAME_SOURCES_T26})
  target_include_directories(game2_26 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN} ${GEN}/inc26 ${GEN}/inc26/tomba)
  target_compile_options(game2_26 PRIVATE ${PORT2_ARCH_FLAGS} ${PORT2_GAME_FLAGS})
  list(APPEND PORT2_GAME_OBJS $<TARGET_OBJECTS:game2_26>)
  list(APPEND PORT2_GAME_TARGETS game2_26)
endif()

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
# libchdr (third_party/libchdr, BSD-3, with the zstd / LZMA / miniz / dr_flac copies it ships): .chd disc images
set(CHDR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libchdr)
add_library(port2chd OBJECT ${CHDR_DIR}/unity.c ${CHDR_DIR}/src/libchdr_codec_avhuff.c)
target_include_directories(port2chd PRIVATE ${CHDR_DIR}/include)
target_compile_definitions(port2chd PRIVATE ZSTD_DISABLE_ASM)
target_compile_options(port2chd PRIVATE ${PORT2_ARCH_FLAGS} -O2 -w)
add_library(port2rt OBJECT ${PORT2_RT} ${GEN}/funcmap.c ${GEN}/build_id.c)
add_dependencies(port2rt port2_build_id)
target_include_directories(port2rt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(port2rt PRIVATE ${GEN}/version)      # port_version.h (gen2.py: the version's layout)
target_include_directories(port2rt PRIVATE ${CHDR_DIR}/include)
target_compile_definitions(port2rt PRIVATE PORT_HAVE_CHD)
target_compile_options(port2rt PRIVATE ${PORT2_ARCH_FLAGS} ${PORT2_RT_FLAGS} -O2 -Wall -Wno-unused-function -ffp-contract=off)
# the runtime's float math (the software rasteriser's attribute gradients, shims/gpu.c) rounds the same on every build:
# IEEE single / double, no fused multiply-adds; on 32-bit x86 SSE2 instead of the x87's 80-bit intermediates (which
# put a few texels per frame elsewhere than the ARM and the 64-bit wasm2c builds do)
if(NOT ANDROID)
  target_compile_options(port2rt PRIVATE -msse2 -mfpmath=sse)
endif()
# the Spanish redraws generator must round like Pillow / Python (byte-identical to tools/gfx_es): no fused multiply-adds,
# and on 32-bit x86 SSE2 doubles instead of the x87's 80-bit intermediates
if(ANDROID)
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/shims/gfx_es.c PROPERTIES COMPILE_OPTIONS "-ffp-contract=off;-fno-math-errno")
else()
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/shims/gfx_es.c PROPERTIES COMPILE_OPTIONS "-ffp-contract=off;-fno-math-errno;-msse2;-mfpmath=sse")
endif()
if(ANDROID)
  # -DPORT2_SDL2_SOURCE=DIR: an SDL2 2.30.x source tree (the workflow .github/workflows/android-apk.yml downloads the
  # release tarball; never vendored).  Built here as libSDL2.so; the headers are copied to gen/sdlinc/SDL2 so the
  # runtime's <SDL2/SDL.h> includes work unchanged.
  set(PORT2_SDL2_SOURCE "" CACHE PATH "SDL2 source tree (Android)")
  if(NOT EXISTS ${PORT2_SDL2_SOURCE}/include/SDL.h)
    message(FATAL_ERROR "-DPORT2_SDL2_SOURCE must name an SDL2 source tree (SDL2-2.30.x)")
  endif()
  if(NOT TARGET SDL2)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    add_subdirectory(${PORT2_SDL2_SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/sdl2 EXCLUDE_FROM_ALL)
  endif()
  file(GLOB sdlhdrs ${PORT2_SDL2_SOURCE}/include/*.h)
  file(COPY ${sdlhdrs} DESTINATION ${GEN}/sdlinc/SDL2)
  target_include_directories(port2rt PRIVATE ${GEN}/sdlinc)
  set(PORT2_SDL2 SDL2 log android dl m)
elseif(WIN32)
  # -DPORT_SDL2_MINGW=DIR names the i686-w64-mingw32 folder of SDL2-devel-2.30.x-mingw.tar.gz (never vendored:
  # .github/workflows/port2-exe.yml downloads it); tombi2.exe needs its bin/SDL2.dll next to it
  set(PORT_SDL2_MINGW "" CACHE PATH "SDL2 mingw development folder (.../i686-w64-mingw32)")
  if(NOT EXISTS ${PORT_SDL2_MINGW}/include/SDL2/SDL.h)
    message(FATAL_ERROR "-DPORT_SDL2_MINGW must name the i686-w64-mingw32 folder of the SDL2 mingw development tarball")
  endif()
  target_include_directories(port2rt PRIVATE ${PORT_SDL2_MINGW}/include)
  # (opengl32: the GL renderer of shims/gpu_gl.c loads every entry point through SDL_GL_GetProcAddress; no GLEW)
  set(PORT2_SDL2 -L${PORT_SDL2_MINGW}/lib -lSDL2 -lwinmm -lopengl32)
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
     CONTENT "$<JOIN:${PORT2_GAME_OBJS},\n>\n$<JOIN:$<TARGET_OBJECTS:port2rt>,\n>\n$<JOIN:$<TARGET_OBJECTS:port2chd>,\n>\n")
add_custom_command(OUTPUT ${GEN}/syms.ld ${GEN}/stubs.c ${GEN}/data.c ${GEN}/thunks.S ${GEN}/addrmap.c ${GEN}/report.txt
                   COMMAND ${PORT2_GEN_ENV} ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py link ${GEN} ${CMAKE_NM} ${GEN}/objects.txt
                   DEPENDS ${PORT2_GAME_TARGETS} port2rt port2chd $<TARGET_OBJECTS:port2chd> ${PORT2_GAME_OBJS} $<TARGET_OBJECTS:port2rt>
                           ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py
                   COMMENT "Resolving game symbols: data image, aliases, dispatch thunks, stubs")
add_library(port2gen OBJECT ${GEN}/stubs.c ${GEN}/data.c ${GEN}/thunks.S ${GEN}/addrmap.c)
set_source_files_properties(${GEN}/stubs.c ${GEN}/data.c ${GEN}/thunks.S ${GEN}/addrmap.c PROPERTIES GENERATED TRUE)
target_include_directories(port2gen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN} ${GEN}/version)
# (-fno-zero-initialized-in-bss: the zero regions of the image, port_scratch and port_io, stay in .data right after
#  port_ram, below the runtime's large .bss buffers, so the whole image sits below 16 MB on Windows too)
target_compile_options(port2gen PRIVATE ${PORT2_ARCH_FLAGS} -O0 -w -fno-zero-initialized-in-bss)

if(ANDROID)
  # libtombi.so (the game; entry port_main) and libmain.so (SDL_main: reserves the low range, loads libtombi.so into
  # it).  -Bsymbolic: the game's own definitions (its Psy-Q `write`/`open` stubs, the printf shim) win inside the
  # library, as they do in the PC executables, over bionic's that the dynamic linker would otherwise pick first.
  add_library(tombi2 SHARED ${PORT2_GAME_OBJS} $<TARGET_OBJECTS:port2rt> $<TARGET_OBJECTS:port2chd> $<TARGET_OBJECTS:port2gen>)
  set_target_properties(tombi2 PROPERTIES OUTPUT_NAME tombi LINKER_LANGUAGE C LINK_DEPENDS ${GEN}/syms.ld)
  target_link_options(tombi2 PRIVATE -Wl,-Bsymbolic -Wl,-z,noexecstack ${GEN}/syms.ld)
  target_link_libraries(tombi2 PRIVATE ${PORT2_SDL2})
  # crash reports with real names: the APK keeps libtombi.so's symbol table (app/build.gradle keepDebugSymbols; the
  # loader's crash handler reads it) but not the DWARF, which goes with the full copy, libtombi.debug.so in this
  # build dir (the workflow uploads it with the APK, for llvm-addr2line / llvm-symbolizer on device addresses)
  find_program(PORT2_OBJCOPY NAMES llvm-objcopy HINTS ${ANDROID_TOOLCHAIN_ROOT}/bin ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/bin)
  if(PORT2_OBJCOPY)
    add_custom_command(TARGET tombi2 POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:tombi2> ${CMAKE_CURRENT_BINARY_DIR}/libtombi.debug.so
                       COMMAND ${PORT2_OBJCOPY} --strip-debug $<TARGET_FILE:tombi2>
                       COMMENT "libtombi.so: symbol table kept, DWARF moved to libtombi.debug.so" VERBATIM)
  endif()
  add_library(main SHARED ${CMAKE_CURRENT_SOURCE_DIR}/android/loader.c)
  target_include_directories(main PRIVATE ${GEN}/sdlinc)
  target_link_libraries(main PRIVATE SDL2 log dl)
  add_dependencies(main tombi2)
  # libtombidisc.so: the launcher's disc check (MAIN0.EXE sha1 of a .bin/.iso/.chd), loaded in the launcher process
  add_library(tombidisc SHARED ${CMAKE_CURRENT_SOURCE_DIR}/android/disccheck.c ${CMAKE_CURRENT_SOURCE_DIR}/shims/cd_chd.c
              ${CMAKE_CURRENT_SOURCE_DIR}/third_party/lzma-7z/lzma7z.c
              $<TARGET_OBJECTS:port2chd>)
  target_include_directories(tombidisc PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${CHDR_DIR}/include)
  target_compile_definitions(tombidisc PRIVATE PORT_HAVE_CHD)
  target_compile_options(tombidisc PRIVATE -O2 -fPIC)
  return()
endif()
add_executable(tombi2 ${PORT2_GAME_OBJS} $<TARGET_OBJECTS:port2rt> $<TARGET_OBJECTS:port2chd> $<TARGET_OBJECTS:port2gen>)
set_target_properties(tombi2 PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS ${GEN}/syms.ld)
if(WIN32)
  enable_language(RC)                               # the exe's icon (tools/icons/port.ico)
  target_sources(tombi2 PRIVATE tombi2.rc)
  # a console program (the launcher captures stderr; no SDL2main: main() is the plain C one); based at 4 MB with
  # ASLR off so the data image stays below 16 MB (the 24-bit OT links); 16 MB main stack; no libgcc DLL
  target_link_options(tombi2 PRIVATE -m32 -mconsole -static-libgcc -Wl,--image-base,0x400000 -Wl,--disable-dynamicbase
                      -Wl,--stack,16777216 ${GEN}/syms.ld)
  target_link_libraries(tombi2 PRIVATE ${PORT2_SDL2})
else()
target_link_options(tombi2 PRIVATE -m32 -no-pie -Wl,-Ttext-segment=0x100000 -Wl,-z,noexecstack ${GEN}/syms.ld)
target_link_libraries(tombi2 PRIVATE ${PORT2_SDL2} m pthread)
endif()
