# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-15). Do not edit by hand._

## Summary

**Whole game: 87.96 % of the game's code matches byte for byte** (1338136 of 1521276 bytes).

| Part | Code | Matching bytes | Matching |
|---|---:|---:|---|
| MAIN0.EXE: engine, shared by every area | 279192 B | 279192 B | 100.0 % `[########################]` |
| X000.BIN: AREA00 + object code reused by all areas | 312228 B | 312228 B | 100.0 % `[########################]` |
| X001..X019.BIN: code specific to the other areas | 736440 B | 736440 B | 100.0 % `[########################]` |
| SYSTEM/*.BIN: options, sub-screens, game over, info screens | 193416 B | 10276 B | 5.3 % `[#.......................]` |
| **Whole game** | **1521276 B** | **1338136 B** | **87.96 %** `[#####################...]` |

- Bytes of machine code in game functions, counted exactly like the objdiff report on decomp.dev (`tools/units.py` is the common list); Sony's Psy-Q library (99084 B) is not counted: 67176 B of it (67.8 %) also matches, from Psy-Q sources ported from psx_tomba, and is tracked apart.
- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each
  other; those copies match automatically and are not added again (counting every copy separately gives 94.6 %).
- Also tracked for MAIN0 + X000: 93.1 % of the code named by us, 76.0 % typed with the TObj structure.

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 99084 bytes (901 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library | Psy-Q matching |
|---|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 228236 B (516 f) | 98.9 % | 56.6 % | 100.0 % (1141 f) | 99044 B | 67136 B (67.8 %) |
| X000.BIN (AREA00 overlay) | 297744 B (526 f) | 88.7 % | 90.8 % | 100.0 % (523 f) | 40 B | 40 B (100.0 %) |

## Area overlays (X001..X019) and SYSTEM overlays

Raw code blobs loaded at `0x800E8028` like X000 ([notes/overlays.md](../notes/overlays.md)). Boundaries come from
`tools/areas.py bounds` (`notes/functions_x0nn.csv`); about 173 KB of object code is shared by all of them (and X000)
at the same addresses, so most of it is matched by copying the X000/MAIN0 twin (`tools/areas.py twins`, sources
in `src/x0nn/`). Unmatched functions are listed in [notes/todo_areas.csv](../notes/todo_areas.csv) by status:
*x000* = identical to an X000 function not matched yet, *dup* = identical to a function of an earlier overlay,
*piece* = identical to a Ghidra piece of a larger matched X000 range (needs its own C),
*new* = no twin anywhere (the real new work). The SYSTEM overlays (`tools/sysovl.py`: O0PTSUB0, D0SPSUB*, G0OVER,
I0NFO*, also at `0x800E8028`, sources in `src/<name>/`) are linked from source by `tools/build_full.py`.

| Overlay | Areas | Code | Functions | Matching | Pending: x000 | dup | piece | new |
|---|---|---|---|---|---|---|---|---|
| X001.BIN | AREA01, AREA07 | 331348 B | 602 | 99.9 % (601 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200372 B | 364 | 99.8 % (363 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X003.BIN | AREA03 | 298416 B | 583 | 99.9 % (582 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X004.BIN | AREA04, AREA12 | 280788 B | 569 | 99.9 % (568 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X005.BIN | AREA05 | 180916 B | 328 | 99.8 % (327 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X006.BIN | AREA06 | 211464 B | 396 | 99.8 % (395 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X008.BIN | AREA08 | 175232 B | 307 | 99.8 % (306 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X009.BIN | AREA09 | 258240 B | 506 | 99.8 % (505 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X010.BIN | AREA10 | 277512 B | 535 | 99.4 % (533 f) | 0 B | 1212 B | 400 B | 0 B (0 f) |
| X011.BIN | AREA11 | 190292 B | 354 | 99.8 % (353 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X013.BIN | AREA13 | 184736 B | 330 | 99.8 % (329 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X014.BIN | AREA14 | 237812 B | 494 | 99.8 % (493 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X016.BIN | AREA16 | 186504 B | 359 | 99.8 % (358 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X017.BIN | AREA17 | 189712 B | 351 | 99.8 % (350 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X018.BIN | AREA18 | 191672 B | 353 | 99.8 % (352 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X019.BIN | AREA19 | 175472 B | 307 | 99.8 % (306 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| O0PTSUB0.BIN | SYSTEM | 8544 B | 27 | 77.4 % (24 f) | 0 B | 0 B | 0 B | 1932 B (3 f) |
| D0SPSUB.BIN | SYSTEM | 37156 B | 59 | 1.4 % (12 f) | 0 B | 984 B | 0 B | 35660 B (43 f) |
| D0SPSUB2.BIN | SYSTEM | 20004 B | 31 | 0.0 % (0 f) | 0 B | 220 B | 0 B | 19784 B (29 f) |
| D0SPSUB3.BIN | SYSTEM | 25324 B | 46 | 0.0 % (0 f) | 0 B | 19404 B | 0 B | 5920 B (10 f) |
| G0OVER.BIN | SYSTEM | 3152 B | 7 | 100.0 % (7 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| I0NFO.BIN | SYSTEM | 26248 B | 53 | 0.1 % (1 f) | 0 B | 368 B | 20 B | 25828 B (49 f) |
| I0NFO2.BIN | SYSTEM | 90280 B | 196 | 0.1 % (2 f) | 0 B | 14692 B | 48 B | 75460 B (154 f) |
| I0NFO3.BIN | SYSTEM | 26692 B | 55 | 0.0 % (0 f) | 0 B | 8096 B | 40 B | 18556 B (39 f) |
| **Total** | | 3807888 B | 7212 | 93.8 % (6767 f) | 0 B | 44976 B | 6508 B | 183140 B (327 f) |

The *Matching* column counts every copy, including the shared code matched through twins; the whole-game
figure in the summary counts each function once.

## What is NOT counted (the real denominator is larger)

- `MAIN1..8.EXE`: they share ~98 % of the code (`.text`) with MAIN0; they are treated as variants and not added.
- `SCES_013.31` (loader, 651 KB): almost all Psy-Q library; not analyzed.
- AREA07 and AREA12 reuse X001/X004, AREA15 has no code; X1nn..X8nn are other languages, and so are the
  SYSTEM overlays of the variants 1..8 (D1SPSUB ... I8NFO3; only the variant 0 of MAIN0 is counted).
- Functions called only by an overlay that Ghidra does not recognize, and overlays not yet identified
  (172 MAIN0 targets at `0x800E8000+` do not fall in X000).
- Functions outside the code (`479` bytes of Ghidra false positives in RAM with no contents).

## Plan

1. Match every game function byte for byte (this report).
2. All area overlays (done: 16 overlays wired in, see above).
3. Remove the matching debt (`do{}while(0)`, `register asm`, volatile pins, `// FLAGS`, asm fences).
4. Readable names and types (psx_tomba names, structs instead of raw offsets); then split the sources into
   segment files (one per original .c) and link with the splat build.
5. Visibility: objdiff report on decomp.dev (done).
6. Assets: a decompilation is complete only when the whole disc can be rebuilt. Inventory the files of the
   disc (`tools/isoext.py` extracts it), document every format (area `X0nn.BIN` data, graphics, sound banks,
   STR movies, texts, `SCES_013.31` and `MAIN1..8`), write unpack/repack tools and a `make` target that
   produces a bootable image from sources. Starts when the code passes ~95 %.

## Largest unnamed functions (next targets)

| Size | Address | Program |
|---|---|---|
| 2328 B | `8005aa74` | MAIN0.EXE |
| 1936 B | `801377b8` | X000.BIN |
| 1676 B | `80136ce0` | X000.BIN |
| 1600 B | `801363e4` | X000.BIN |
| 1272 B | `800ed258` | X000.BIN |
| 1212 B | `800ed750` | X000.BIN |
| 1036 B | `8011e30c` | X000.BIN |
| 864 B | `80137458` | X000.BIN |
| 796 B | `8011ccd8` | X000.BIN |
| 772 B | `80123344` | X000.BIN |
| 768 B | `8012d9fc` | X000.BIN |
| 744 B | `8012ff0c` | X000.BIN |
| 732 B | `80117cbc` | X000.BIN |
| 728 B | `8011ca00` | X000.BIN |
| 672 B | `80122f54` | X000.BIN |

## How it is measured

- Unit: bytes of code per function (the size reported by Ghidra).
- *Named*: it is in `notes/names_*.csv` and is not a library function. *Typed*: its first parameter is `TObj *`.
- *Matching*: functions in `src/*.c` marked with `// MATCHING <address> <bytes>`; verified byte for byte
  against the retail executable with `tools/ncheck.py` / `tools/matchcheck.py` (relocations masked).
- The typing figures come from the latest Ghidra export (`notes/functions_*.csv`) and may lag behind.
