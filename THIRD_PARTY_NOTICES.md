# Third-party notices

## psx_tomba — https://github.com/hansbonini/psx_tomba
Decompilation of the NTSC-U release of the same game (*Tomba!*, `SCUS_942.36`), MIT licensed (per its README).
Used here:
- `include/tomba/` — headers (`common.h`, `game.h`, macros) copied unmodified. Its PSY-Q SDK headers
  (`include/tomba/psyq/`, Sony copyright) are NOT stored here; `tools/setup_native.sh` fetches them locally.
- `src/*.c` files whose header says `Portado de psx_tomba` — functions ported from its C sources and
  re-verified byte-for-byte against the PAL executable.
- `notes/names_main0.csv` entries whose comment mentions `psx_tomba` — function names, mapped onto the PAL
  executable either by matching compiled bytes or by address alignment.

## Toolchain (not redistributed, downloaded by the setup scripts)
- decompals/old-gcc (`gcc-2.7.2-psx`), mkst/maspsx, GNU binutils.
