# tools/psxemu

A PlayStation for running Tombi!'s own MIPS code in Python: a block-compiling R3000A interpreter (`cpu.py`), a GTE
(`gte.py`), a software GPU rasterizer (`gpu.py`) and HLE of the BIOS and of the Psy-Q libraries keyed by function
address (`machine.py`). Sound, MDEC and real CD timing are not emulated (libsnd/libspu are stubs, FMV tasks end at
once).

## Builds

`Machine(build=NAME)` (default `pal`) picks a profile of `builds.py`:

| build | executable (default path) | boots to |
|---|---|---|
| `pal` | `game/MAIN0.EXE` (SCES-01331) | title, gameplay, debug area select (as before profiles) |
| `usa` | `game_usa/fs/SCUS_942.36` | gameplay (AREA00 opening dialogue) |
| `jp` | `game_jp/SLPS_011.44` | gameplay (AREA00 opening dialogue) |
| `beta` | `game_beta980203/fs/SLPS_011.44` | gameplay (AREA00 opening dialogue) |
| `jpdemo` | `game_jp_demo/fs/SLPM_801.65` | gameplay (AREA00, 5:00 demo timer) |
| `prepre9` | `game_prepre9/fs/TOMBA/TOMBA.EXE` | gameplay (Dwarf Village, 3:00 timer); AREA01 via `pre9_rock.py` |
| `prepre8` | `game_prepre8/fs/TOMBA/TOMBA.EXE` | gameplay (early title, AREA00, 5:00 timer) |
| `dengeki` | `game_dengeki_d5/fs/TOMBA/TOMBA.EXE` | gameplay (AREA00) |
| `hyper` | `game_hyper12/fs/TOMBA/TOMBA.EXE` | gameplay (AREA00, 5:00 timer) |

The discs come from `bash tools/disc_fetch.sh NAME`; `Machine(build=..., exe=PATH, disc=PATH)` overrides the paths.
Only `pal` uses the hand-named symbol tables (`notes/functions_main0.csv`, `notes/names_main0.csv`). For every other
build, `libsig.py` finds the library entry points (and the few game functions the HLE needs: `exitTask`,
`StartTask`, `updateMainObjects`, the FMV player task...) by masked byte signature of the matched PAL functions and
writes them as an address table, `syms/<build>.csv` (names and addresses only, nothing of the game). The FMV player
task is found by its `StartTask` call next to `MOVIE_PLAY_STATE` (0x1F8001CC) = 1; PrePre 8, older, keeps that state at
0x800CCCDC, so its profile names the task (0x800228B8) by hand:

    python3 tools/psxemu/libsig.py prepre9 [--exe PATH] [--all]

The demos load files with `CdReadyCallback` + `CdlReadN` instead of `CdRead`; profiles with `cd_irq` deliver the
sectors from the VBlank (8 per VBlank) through the game's own callback. Old libetc `PadRead` (PrePre 9) reads the BIOS
`PAD_init` buffer; libpad builds use `PadInitDirect`'s buffer, or the one `joypadInit` passes (USA). libpad functions
the tables miss (the beta's PadGetState) call through a pointer the HLE'd init never sets and are stubbed by pattern.
PAL is unchanged: `capture.area00` gives the same frame and instruction counts and the same RAM and VRAM as before.

## Tools

- `capture.py`: PAL title / area00 / world map / every room through the debug select.
- `pre9_rock.py`: PrePre 9, Tombi examines the "2" graffiti rock: `frames`, `video` (MP4 + GIF) and `trace` (call
  tree of the rock handler, every store it makes, and a differential of RAM writes against the same input without the
  button presses).
- Debug hooks: `cpu.trace = fn(cpu, pc)` at every block start; `cpu.set_store_hook(fn(cpu, pc, addr, value, size))`
  before every store (blocks are recompiled with the hook, slower); `Machine.save_state` / `load_state`.

Timing: one game frame per 60 Hz (NTSC) or 50 Hz (PAL) field when the game keeps up (`0x1F8001EA` = 1), so frame
dumps play back at that rate.
