# Cross-compiling the port for Windows (32-bit) with mingw-w64 (apt: gcc-mingw-w64-i686); docs/PORT.md "Windows build":
#   cmake -S port -B build/port-win -G Ninja -DCMAKE_TOOLCHAIN_FILE=port/cmake/mingw-i686.cmake \
#         -DPORT_SDL2_MINGW=/path/to/SDL2-2.30.x/i686-w64-mingw32
#   cmake --build build/port-win -j 8        # -> build/port-win/tombi.exe, tombi-view.exe
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR i686)
set(MINGW_PREFIX "/usr/bin/i686-w64-mingw32-" CACHE STRING "prefix of the mingw-w64 i686 tools")
set(CMAKE_C_COMPILER ${MINGW_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${MINGW_PREFIX}gcc)
set(CMAKE_NM ${MINGW_PREFIX}nm)
set(CMAKE_RC_COMPILER ${MINGW_PREFIX}windres)
