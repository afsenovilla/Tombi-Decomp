# Third-party notices

## psx_tomba — https://github.com/hansbonini/psx_tomba
Decompilation of the NTSC-U release of the same game (*Tomba!*, `SCUS_942.36`), MIT licensed (per its README).
Used here:
- `include/tomba/` — headers (`common.h`, `game.h`, macros) copied unmodified (refreshed from psx_tomba commit #21,
  "Add function matches, PsyQ matches, apply enums and rename functions"), plus its library-internal headers
  `libsnd_i.h` and `libspu_internal.h` (from `src/scus_942.36/psyq/libsnd/` and `libspu/`). Its PSY-Q SDK headers
  (`include/tomba/psyq/`, Sony copyright) are NOT stored here; `tools/setup_native.sh` fetches them locally.
- `src/*.c` files whose header says `Portado de psx_tomba` — functions ported from its C sources and
  re-verified byte-for-byte against the PAL executable.
- `notes/names_main0.csv` entries whose comment mentions `psx_tomba` — function names, mapped onto the PAL
  executable either by matching compiled bytes or by address alignment.

## TombaEdit — https://github.com/SiergiejW/TombaEdit
MIT licensed (Python/PyQt6 asset viewer for Tomba!/Tombi!). Used here as a reference, not vendored:
- `docs/FORMATS.md` — summary of the WMD, SPR, WPP, GRPX, CLM, WHM, WFM, WSS, APD and archive layouts, taken from its `docs/formats.md` and `docs/collision.md`.
- `tools/objinit.py` — the idea of resolving an object's texture page offset, CLUT and SPR bank by running its init code on an object
  filled from the placement record, and the PAL routine addresses it stubs (from `formats/level/runtime.py`, `game/builds.py`); the code is ours.

## Toolchain (not redistributed, downloaded by the setup scripts)
- decompals/old-gcc (`gcc-2.7.2-psx`), mkst/maspsx, GNU binutils.

## mkpsxiso / dumpsxiso — https://github.com/Lameguy64/mkpsxiso
GPL-2.0. Not stored in the repo: `.github/workflows/text-exe.yml` downloads the v2.30 Windows release and bundles
`mkpsxiso.exe` and `dumpsxiso.exe`, unmodified, inside `dist/windows/tombi-text.exe` (used by its "Generar ISO"). Source and license at the link above.

## SDL2 — https://github.com/libsdl-org/SDL
zlib licence. Not stored in the repo: `.github/workflows/port2-exe.yml` downloads the `SDL2-devel-2.30.x-mingw` release
and links `port2/runtime/sdl.c` (tombi2.exe) against it; `SDL2.dll`, unmodified, is bundled inside `dist/windows/tombi-port.exe`
(the native port's launcher, docs/PORT.md) next to our `tombi.exe` / `tombi-view.exe`. Its licence text travels with
the artifact as `SDL2-LICENSE.txt`. No game data is bundled: the player supplies their own disc image.
