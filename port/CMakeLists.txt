# Native port of Tombi! (prototype).  Separate from the matching build: it reads src/ (never writes it), compiles
# the game C for the host and links it with the runtime and Psy-Q replacements in port/.  See docs/PORT.md.
#
#   python3 tools/build_full.py -j 4          # once: the port takes retail addresses from the matching build
#   cmake -S port -B build/port -G Ninja      # (or Unix Makefiles)
#   cmake --build build/port -j 8
#   build/port/tombi --frames 600             # from the repo root: reads game/MAIN0.EXE + game/disc/Tombi.bin
cmake_minimum_required(VERSION 3.16)
project(tombi_port C ASM)

set(ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(GEN ${CMAKE_CURRENT_BINARY_DIR}/gen)
find_package(Python3 REQUIRED COMPONENTS Interpreter)

# 1. pick + filter the game sources (MAIN0 and X000 for now) into ${GEN}/src; re-run on every configure
set(PORT_PROGS MAIN0 X000 CACHE STRING "programs whose C is compiled into the port")
execute_process(COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_port.py sources ${GEN} ${PORT_PROGS}
                RESULT_VARIABLE gen_rc)
if(gen_rc)
  message(FATAL_ERROR "gen_port.py sources failed")
endif()
include(${GEN}/sources.cmake)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
             ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_port.py ${CMAKE_CURRENT_SOURCE_DIR}/fixes.txt ${ROOT}/build/full/main0.full.ld)

# 2. target: i386, freestanding (the game's structs hold 32-bit pointers; the prototype needs no 32-bit libc)
set(PORT_ARCH_FLAGS -m32 -march=i686 -fno-pic -fno-pie -fno-stack-protector -fcf-protection=none
    -fno-asynchronous-unwind-tables -ffreestanding -fno-builtin)
# semantics of the original compiler: unsigned char, wrapping signed arithmetic, no type-based alias analysis
set(PORT_GAME_FLAGS -O1 -w -funsigned-char -fwrapv -fno-strict-aliasing -fno-delete-null-pointer-checks
    -DSKIP_ASM -D_LANGUAGE_C -DMIPSEL)

add_library(game OBJECT ${PORT_GAME_SOURCES})
target_include_directories(game PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN}/inc ${GEN}/inc/tomba)
target_compile_options(game PRIVATE ${PORT_ARCH_FLAGS} ${PORT_GAME_FLAGS})

add_library(game24 OBJECT ${PORT_GAME_SOURCES_T24})
target_include_directories(game24 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include ${GEN}/inc24 ${GEN}/inc24/tomba)
target_compile_options(game24 PRIVATE ${PORT_ARCH_FLAGS} ${PORT_GAME_FLAGS})

file(GLOB PORT_RUNTIME_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/runtime/*.c ${CMAKE_CURRENT_SOURCE_DIR}/shims/*.c)
add_library(portrt OBJECT ${PORT_RUNTIME_SOURCES} ${GEN}/funcmap.c)
target_include_directories(portrt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(portrt PRIVATE ${PORT_ARCH_FLAGS} -O2 -Wall -Wno-unused-function -Wno-builtin-declaration-mismatch)

# 3. resolve symbols against the retail image: aliases, retail data addresses, call thunks
file(GENERATE OUTPUT ${GEN}/objects.txt
     CONTENT "$<JOIN:$<TARGET_OBJECTS:game>,\n>\n$<JOIN:$<TARGET_OBJECTS:game24>,\n>\n$<JOIN:$<TARGET_OBJECTS:portrt>,\n>\n")
add_custom_command(OUTPUT ${GEN}/syms.ld ${GEN}/thunks.S
                   COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_port.py link ${GEN} nm ${GEN}/objects.txt
                   DEPENDS game game24 portrt $<TARGET_OBJECTS:game> $<TARGET_OBJECTS:game24> $<TARGET_OBJECTS:portrt>
                           ${CMAKE_CURRENT_SOURCE_DIR}/tools/gen_port.py
                   COMMENT "Resolving game symbols against the retail image")
set_source_files_properties(${GEN}/thunks.S PROPERTIES GENERATED TRUE COMPILE_OPTIONS "-m32")

add_executable(tombi ${GEN}/thunks.S $<TARGET_OBJECTS:game> $<TARGET_OBJECTS:game24> $<TARGET_OBJECTS:portrt>)
set_target_properties(tombi PROPERTIES LINKER_LANGUAGE C LINK_DEPENDS ${GEN}/syms.ld)
target_link_options(tombi PRIVATE -m32 -nostdlib -static -no-pie -Wl,--build-id=none -Wl,-z,noexecstack
                    -Wl,--allow-multiple-definition ${GEN}/syms.ld)
