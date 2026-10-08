# Tombi! (Tomba!) — PlayStation decompilation

A work-in-progress **matching decompilation** of *Tombi!* (known as *Tomba!* in North America and
*Ore! Tomba* in Japan) for the original PlayStation. The goal is C source code that compiles, with the
game's original toolchain, into byte-identical machine code.

This repository targets the **European Spanish release** and works function by function: every
function in `src/` is verified to produce exactly the same instructions as the retail executable.

> This repository contains no game data, executables, disc images or Sony SDK files.
> You need your own legally obtained copy of the game and of the toolchain.

## Progress

![Progress](docs/progress.svg)

Function maps (one block per function, coloured by status):

![MAIN0](docs/map_main0.svg)

![X000](docs/map_x000.svg)

Full breakdown in [docs/PROGRESS.md](docs/PROGRESS.md), regenerated with `python3 tools/progress.py`.
Progress is measured in bytes of **game code**; the Sony PSY-Q library code is reported separately.

## Target

| File | Description | SHA-1 |
|------|-------------|-------|
| `SCES_013.31` | Boot executable (PAL, Spanish) | `f6c383514e5d8367d2eed06400315cf6dd57ef6b` |
| `EXE/MAIN0.EXE` | Game core, loaded at `0x80010000` | `bec8dc5f4116f5e497f2337eef8c523ff6dee8fc` |
| `AREA00/X000.BIN` | Area overlay, loaded at `0x800E8028` | `ce168475f667f2ca8842eabad2aec829e301c5c5` |

`MAIN0.EXE` corresponds to `SCUS_942.36` of the NTSC-U release (same functions, shifted addresses).
See [notes/disc_layout.md](notes/disc_layout.md), [notes/memory_map_main0.md](notes/memory_map_main0.md)
and [notes/overlays.md](notes/overlays.md).

## How matching works

Each function lives in its own file, `src/<Name>.c`, with a small header:

```c
// FUNC 8001fddc 48 MAIN0        address, size, program
// MATCHING 8001fddc 48          added once the bytes match
```

Two checkers compile the file and compare the result with the retail binary, masking relocations:

| Tool | Toolchain | Use |
|------|-----------|-----|
| `tools/ncheck.py` | native `gcc-2.7.2-psx` cc1 + [maspsx](https://github.com/mkst/maspsx) + GNU `as` | fast iteration (seconds) |
| `tools/matchcheck.py` | original PSY-Q 4.3 `CC1PSX.EXE` (Wine) + `CPPPSX` (DOSBox) + `ASPSX 2.86` | reference check |

The game was built with the **PSY-Q 4.3 C compiler (GCC 2.7.2.SN.1)**; see
[notes/compiler.md](notes/compiler.md). Practical recipes for reproducing GCC 2.7.2 output are collected
in [notes/matching_guide.md](notes/matching_guide.md).

## Getting started

Requirements: Linux, Python 3, `binutils-mipsel-linux-gnu`. Optional: Wine + DOSBox and the PSY-Q SDK
binaries for the reference checker; Ghidra 12.1.4 + JDK 21 for analysis.

```bash
# 1. Put your own game files in game/ (ignored by git)
#    game/SCES_013.31, game/MAIN0.EXE, game/AREA00/X000.BIN

# 2. Fast native toolchain (old-gcc + maspsx + binutils)
tools/setup_native.sh

# 3. Check one function, or all of them
python3 tools/ncheck.py src/MulCos.c
python3 tools/ncheck.py src/*.c

# 4. Inspect a function: disassembly + Ghidra pseudo-C
python3 tools/fn.py 8001fddc
```

Reference checker: copy `CC1PSX.EXE` (PSY-Q 4.3) to `/opt/psyq/cc43/`, the DOS `CPPPSX.EXE` to
`/opt/psyq/new/` and `ASPSX.EXE` 2.86 to `/opt/psyq/46/BIN/`, install `wine` and `dosbox`, then run
`python3 tools/matchcheck.py src/<Name>.c`.

## Progress report (objdiff / decomp.dev)

`tools/build_report.py` produces the [objdiff](https://github.com/encounter/objdiff) report that
[decomp.dev](https://decomp.dev) displays: [splat](https://github.com/ethteck/splat) splits the retail
binaries into one assembly file per function (`config/main0.yaml`, `config/x000.yaml`), each function is
assembled as the *target* object, each `src/*.c` is compiled as the *base* object, and `objdiff-cli`
compares them (`build/report.json`). The GitHub Actions workflow `.github/workflows/report.yml` runs it on
every push; it needs a `GAME_FILES_URL` secret pointing to a zip with your own `MAIN0.EXE` and
`AREA00/X000.BIN`.

## Repository layout

```
src/            matching C, one function per file
src/wip/        unfinished attempts (not counted as progress)
include/        shared headers (tobj.h: game object struct)
include/tomba/  headers from psx_tomba (see THIRD_PARTY_NOTICES.md)
notes/          memory maps, structures, function lists and names, guides
docs/           generated progress report and charts
ghidra/scripts/ Ghidra scripts (headless pipeline, struct and name import)
config/         splat configurations for MAIN0.EXE and X000.BIN
.github/        CI workflow that publishes the objdiff report
tools/          checkers, progress report, analysis helpers, setup scripts
game/           your own game files (ignored by git)
```

## Tools

| Tool | Purpose |
|------|---------|
| `tools/ncheck.py` | Fast native compile-and-compare (`--score`, `--asm`, `--mark`) |
| `tools/matchcheck.py` | Reference compile-and-compare with the original PSY-Q binaries |
| `tools/fn.py` | Disassembly (capstone) and Ghidra pseudo-C of one function |
| `tools/progress.py` | Regenerates `docs/PROGRESS.md` and the SVG charts |
| `tools/classify.py` | Separates complete functions from Ghidra fragments (`notes/todo_match3.csv`) |
| `tools/fixbounds.py` | Detects functions Ghidra split incorrectly |
| `tools/permute.py` | Small random permuter (CPU only) for near-misses |
| `tools/ghidra_pipeline.sh` | Headless Ghidra: import, apply names and structs, export pseudo-C |
| `tools/setup_ghidra.sh`, `tools/setup_native.sh` | Toolchain installers |
| `tools/psxexe_info.py` | Prints the PS-X EXE header |

## Related projects

- [psx_tomba](https://github.com/hansbonini/psx_tomba) — decompilation of the NTSC-U release
  (`SCUS_942.36`) with a full splat/objdiff build. Its headers, names and some functions are reused here
  and re-verified against the PAL executable; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## License

Source code and documentation are released under the [MIT License](LICENSE).
Third-party material is listed in [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).

## Legal

This project contains only original source code written to reproduce the behaviour of the game, plus
documentation. *Tombi!* is © Whoopee Camp / Sony Computer Entertainment. No copyrighted game data or
SDK files are distributed.
