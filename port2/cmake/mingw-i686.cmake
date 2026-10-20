# Cross-compiling the native-pointer port (port2) for Windows (32-bit) with mingw-w64 (apt: gcc-mingw-w64-i686);
# docs/PORT.md "M3 phase 3":
#   cmake -S port2 -B build/port2-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=port2/cmake/mingw-i686.cmake \
#         -DPORT_SDL2_MINGW=/path/to/SDL2-2.30.x/i686-w64-mingw32
#   cmake --build build/port2-win -j 8        # -> build/port2-win/tombi2.exe (+ SDL2.dll from the SDL2 folder)
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(MINGW_PREFIX "/usr/bin/i686-w64-mingw32-" CACHE STRING "prefix of the mingw-w64 i686 tools")
set(CMAKE_C_COMPILER ${MINGW_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${MINGW_PREFIX}gcc)
set(CMAKE_NM ${MINGW_PREFIX}nm)
set(CMAKE_RC_COMPILER ${MINGW_PREFIX}windres)
