# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-26). Do not edit by hand._

## Summary

**Whole game: 98.89 % of the game's code matches byte for byte** (1510324 of 1527340 bytes).

| Part | Code | Matching bytes | Matching |
|---|---:|---:|---|
| MAIN0.EXE: engine, shared by every area | 284708 B | 284708 B | 100.0 % `[########################]` |
| X000.BIN: AREA00 + object code reused by all areas | 312708 B | 312708 B | 100.0 % `[########################]` |
| X001..X019.BIN: code specific to the other areas | 736440 B | 736440 B | 100.0 % `[########################]` |
| SYSTEM/*.BIN: options, sub-screens, game over, info screens | 193484 B | 176468 B | 91.2 % `[######################..]` |
| **Whole game** | **1527340 B** | **1510324 B** | **98.89 %** `[########################]` |

- Bytes of machine code in game functions, counted exactly like the objdiff report on decomp.dev (`tools/units.py` is the common list); Sony's Psy-Q library (102840 B) is not counted: 69200 B of it (67.3 %) also matches, from Psy-Q sources ported from psx_tomba, and is tracked apart.
- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each
  other; those copies match automatically and are not added again (counting every copy separately gives 99.5 %).
- Also tracked for MAIN0 + X000: 95.3 % of the code named by us, 67.7 % typed with the TObj structure.

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 102840 bytes (918 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library | Psy-Q matching |
|---|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 284156 B (800 f) | 98.4 % | 45.5 % | 100.0 % (1152 f) | 102800 B | 69160 B (67.3 %) |
| X000.BIN (AREA00 overlay) | 306628 B (586 f) | 92.4 % | 88.2 % | 100.0 % (529 f) | 40 B | 40 B (100.0 %) |

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
| X001.BIN | AREA01, AREA07 | 331784 B | 604 | 100.0 % (604 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200808 B | 366 | 100.0 % (366 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X003.BIN | AREA03 | 298852 B | 585 | 100.0 % (585 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X004.BIN | AREA04, AREA12 | 281224 B | 571 | 100.0 % (571 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X005.BIN | AREA05 | 181352 B | 330 | 100.0 % (330 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X006.BIN | AREA06 | 211900 B | 398 | 100.0 % (398 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X008.BIN | AREA08 | 175668 B | 309 | 100.0 % (309 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X009.BIN | AREA09 | 258676 B | 508 | 100.0 % (508 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X010.BIN | AREA10 | 277948 B | 537 | 100.0 % (537 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X011.BIN | AREA11 | 190728 B | 356 | 100.0 % (356 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X013.BIN | AREA13 | 185172 B | 332 | 100.0 % (332 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X014.BIN | AREA14 | 238248 B | 496 | 100.0 % (496 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X016.BIN | AREA16 | 186940 B | 361 | 100.0 % (361 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X017.BIN | AREA17 | 190148 B | 353 | 100.0 % (353 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X018.BIN | AREA18 | 192108 B | 355 | 100.0 % (355 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| X019.BIN | AREA19 | 175908 B | 309 | 100.0 % (309 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| O0PTSUB0.BIN | SYSTEM | 8544 B | 27 | 100.0 % (27 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| D0SPSUB.BIN | SYSTEM | 37156 B | 59 | 71.0 % (56 f) | 0 B | 0 B | 0 B | 10780 B (3 f) |
| D0SPSUB2.BIN | SYSTEM | 20004 B | 31 | 85.9 % (29 f) | 0 B | 0 B | 0 B | 2812 B (2 f) |
| D0SPSUB3.BIN | SYSTEM | 25324 B | 46 | 69.6 % (44 f) | 0 B | 4280 B | 0 B | 3424 B (1 f) |
| G0OVER.BIN | SYSTEM | 3152 B | 7 | 100.0 % (7 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| I0NFO.BIN | SYSTEM | 26248 B | 53 | 100.0 % (53 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| I0NFO2.BIN | SYSTEM | 90280 B | 196 | 100.0 % (196 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| I0NFO3.BIN | SYSTEM | 26692 B | 55 | 100.0 % (55 f) | 0 B | 0 B | 0 B | 0 B (0 f) |
| **Total** | | 3814864 B | 7244 | 99.4 % (7237 f) | 0 B | 4280 B | 0 B | 17016 B (6 f) |

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
| 1600 B | `801363e4` | X000.BIN |
| 1272 B | `800ed258` | X000.BIN |
| 1212 B | `800ed750` | X000.BIN |
| 944 B | `8006b0ac` | MAIN0.EXE |
| 812 B | `8011de84` | X000.BIN |
| 796 B | `8011ccd8` | X000.BIN |
| 744 B | `8012ff0c` | X000.BIN |
| 732 B | `80117cbc` | X000.BIN |
| 728 B | `8011ca00` | X000.BIN |
| 672 B | `80122f54` | X000.BIN |
| 656 B | `80122c10` | X000.BIN |
| 640 B | `801180ac` | X000.BIN |
| 636 B | `8006b7ec` | MAIN0.EXE |
| 576 B | `80110cac` | X000.BIN |

## How it is measured

- Unit: bytes of code per function (the size reported by Ghidra).
- *Named*: it is in `notes/names_*.csv` and is not a library function. *Typed*: its first parameter is `TObj *`.
- *Matching*: functions in `src/*.c` marked with `// MATCHING <address> <bytes>`; verified byte for byte
  against the retail executable with `tools/ncheck.py` / `tools/matchcheck.py` (relocations masked).
- The typing figures come from the latest Ghidra export (`notes/functions_*.csv`) and may lag behind.
