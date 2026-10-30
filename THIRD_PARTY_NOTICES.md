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

## Other PSY-Q library decompilations (ports in `src/`, header line `Ported from <project> ...`)
The functions below are copies of Sony's PSY-Q library objects as the PAL executable links them, re-verified
byte-for-byte here (`tools/ncheck.py`). Like the psx_tomba ports they are decompilations of Sony Computer
Entertainment's library code; the MIT grants below cover their authors' work in them, not Sony's code. Each file's
third header line names its project, the source file it comes from and the copyright line; the C was adapted to
our headers, PAL addresses and names (the `port:` comments mark every change to the body). The names in
`notes/names_main0.csv` whose comment names one of these projects were confirmed by those bytes.

### sotn-decomp — https://github.com/Xeeynamo/sotn-decomp
The repository as a whole is AGPL-3.0; the files of its PSY-Q library decompilation, `src/main/psxsdk/`, each carry
the line `SPDX-License-Identifier: MIT`, and only those files are used here (the same reading psyz and
eternal-silver-sssc-decomp apply). sotn-decomp gives no copyright line of its own for them; psyz's `decomp/LICENSE`
names the same contributors.
- Used: `cb_read`, `memmove`, `sprintf`, `ratan2`, `rcos`, `rsin`, `setIntr` (prototype form), `startIntrDMA`, `_card_clear`,
  `_spu_setInTransfer`, `vmNoiseOn` (with its static helpers), `_SsVmPitchBend`, `_SsVmSeKeyOn`, `_SsVmSeKeyOff`,
  `_SsVmGetSeqVol`/`LVol`/`RVol` (their `SpuVm*` names mapped to libsnd's `_SsVm*`).
- MIT License. Copyright (c) github.com/xeeynamo/sotn-decomp contributors.
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
  to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above
  copyright notice and this permission notice shall be included in all copies or substantial portions of the
  Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

### eternal-silver-sssc-decomp — https://github.com/bashalarmistdecomp/eternal-silver-sssc-decomp
Matching decompilation of *Lunar: Silver Star Story Complete* (SLUS_006.28), MIT licensed (`LICENSE`, (c) 2026
Bash Alarmist); its `NOTICE.md` states that the C under `decomp/<serial>/src/` derives from the game's program and
Sony's PSY-Q 4.5 libraries and that the licence grants nothing in them. Its own files credit sotn-decomp, psyz and
psy-q-decomp for parts they match (their notices are in its `LICENSES/`).
- Used: `_SsSetNrpnVabAttr0` .. `_SsSetNrpnVabAttr16`, `_SsSndPlay`, `_SsVmDamperOn`, `SsStart`, `_spu_init`,
  `_spu_FsetPCR`, `trapIntr`, `setIntr`, `startIntrVSync`, `init_ring_status`, `memcpy`, `bcopy`, `strcpy`,
  `strncpy`, `strlen`. The `IntrEnv`/`IntrCallbacks` and `SsAdsr` typedefs in those files are its.
- MIT License. Copyright (c) 2026 Bash Alarmist.
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
  to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above
  copyright notice and this permission notice shall be included in all copies or substantial portions of the
  Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

### dcb_decomp — https://github.com/ReGame-Labs/dcb_decomp
Matching decompilation of *Digimon Digital Card Battle* (SLUS_013.28), MIT licensed (`LICENSE`, (c) 2026 ReGame
Labs); its `src/main/psyq/` holds the PSY-Q 4.7 library objects the game links, decompiled one file per object.
- Used: `csqrt` (with its two static helpers), `memchr`, `printf`, `rand`, `memset`, `bzero`, `strcat`, `toupper`,
  `_SsVmSelectToneAndVag`.
- MIT License. Copyright (c) 2026 ReGame Labs.
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
  documentation files (the "Software"), to deal in the Software without restriction, including without limitation
  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
  to permit persons to whom the Software is furnished to do so, subject to the following conditions: The above
  copyright notice and this permission notice shall be included in all copies or substantial portions of the
  Software. THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
  LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
  SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
  DEALINGS IN THE SOFTWARE.

### psy-q-decomp — https://github.com/sozud/psy-q-decomp
MIT licensed matching decompilation of the PSY-Q 3.5 libraries; its `src/etc/intr.c` and `src/spu/` were compared
with ours during the port (the same bytes as the files above); no file of it is used on its own.

## TombaEdit — https://github.com/SiergiejW/TombaEdit
MIT licensed (Python/PyQt6 asset viewer for Tomba!/Tombi!). Used here as a reference, not vendored:
- `docs/FORMATS.md` — summary of the WMD, SPR, WPP, GRPX, CLM, WHM, WFM, WSS, APD and archive layouts, taken from its `docs/formats.md` and `docs/collision.md`.
- `tools/objinit.py` — the idea of resolving an object's texture page offset, CLUT and SPR bank by running its init code on an object
  filled from the placement record, and the PAL routine addresses it stubs (from `formats/level/runtime.py`, `game/builds.py`); the code is ours.

## Toolchain (not redistributed, downloaded by the setup scripts)
- decompals/old-gcc (`gcc-2.7.2-psx`), mkst/maspsx, GNU binutils.

## mkpsxiso / dumpsxiso — https://github.com/Lameguy64/mkpsxiso
GPL-2.0. Not stored in the repo: `.github/workflows/text-exe.yml` downloads the v2.30 Windows release and bundles
`mkpsxiso.exe` and `dumpsxiso.exe`, unmodified, inside `tombi-text.exe` (release asset) (used by its "Generar ISO"). Source and license at the link above.

## SDL2 — https://github.com/libsdl-org/SDL
zlib licence. Not stored in the repo: `.github/workflows/port2-exe.yml` downloads the `SDL2-devel-2.30.x-mingw` release
and links `port2/runtime/sdl.c` (tombi2.exe) against it; `SDL2.dll`, unmodified, is bundled inside `tombi-port.exe` (release asset)
(the native port's launcher, docs/PORT.md) next to our `tombi.exe` / `tombi-view.exe`. Its licence text travels with
the artifact as `SDL2-LICENSE.txt`. No game data is bundled: the player supplies their own disc image.
