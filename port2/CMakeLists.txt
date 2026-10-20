# Native-pointer port of Tombi! (M3, phase 1: compile + link MAIN0 and X000 as ordinary C).  See docs/PORT.md.
#
#   python3 tools/build_full.py -j 4                 # once: the generator takes retail addresses from it
#   cmake -S port2 -B build/port2 -G Ninja && cmake --build build/port2 -j 8
#   build/port2/tombi2                                # phase 1: prints the layout check only
#   cat build/port2/gen/report.txt
cmake_minimum_required(VERSION 3.16)
project(tombi_port2 C)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(GEN ${CMAKE_CURRENT_BINARY_DIR}/gen)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# 1. filtered sources + address resolution (re-run on every configure)
set(PORT2_PROGS MAIN0 X000 CACHE STRING "programs whose C is compiled into port2")
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

add_library(port2rt OBJECT ${CMAKE_CURRENT_SOURCE_DIR}/runtime/main.c ${ROOT}/port/runtime/gte.c)
target_include_directories(port2rt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(port2rt PRIVATE ${PORT2_ARCH_FLAGS} -O2 -Wall)

# 3. resolve the rest against the retail symbol table: data image (from the user's MAIN0.EXE/X000.BIN), aliases,
#    stubs.  All three are generated into the build dir; data.c is game data and never leaves it.
file(GENERATE OUTPUT ${GEN}/objects.txt
     CONTENT "$<JOIN:$<TARGET_OBJECTS:game2>,\n>\n$<JOIN:$<TARGET_OBJECTS:game2_24>,\n>\n$<JOIN:$<TARGET_OBJECTS:port2rt>,\n>\n")
add_custom_command(OUTPUT ${GEN}/syms.ld ${GEN}/stubs.c ${GEN}/data.c ${GEN}/report.txt
                   COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py link ${GEN} ${CMAKE_NM} ${GEN}/objects.txt
                   DEPENDS game2 game2_24 port2rt $<TARGET_OBJECTS:game2> $<TARGET_OBJECTS:game2_24> $<TARGET_OBJECTS:port2rt>
                           ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen2.py
                   COMMENT "Resolving game symbols: data image, aliases, stubs")
add_library(port2gen OBJECT ${GEN}/stubs.c ${GEN}/data.c)
set_source_files_properties(${GEN}/stubs.c ${GEN}/data.c PROPERTIES GENERATED TRUE)
target_include_directories(port2gen PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN})
target_compile_options(port2gen PRIVATE ${PORT2_ARCH_FLAGS} -O0 -w)

add_executable(tombi2 $<TARGET_OBJECTS:game2> $<TARGET_OBJECTS:game2_24> $<TARGET_OBJECTS:port2rt> $<TARGET_OBJECTS:port2gen>)
set_target_properties(tombi2 PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS ${GEN}/syms.ld)
target_link_options(tombi2 PRIVATE -m32 -no-pie -Wl,-Ttext-segment=0x100000 -Wl,-z,noexecstack ${GEN}/syms.ld)
