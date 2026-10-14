# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-14). Do not edit by hand._

## Summary

**Whole game: 100.00 % of the game's code matches byte for byte** (1327848 of 1327888 bytes).

| Part | Code | Matching bytes | Matching |
|---|---:|---:|---|
| MAIN0.EXE: engine, shared by every area | 279180 B | 279180 B | 100.0 % `[########################]` |
| X000.BIN: AREA00 + object code reused by all areas | 312228 B | 312228 B | 100.0 % `[########################]` |
| X001..X019.BIN: code specific to the other areas | 736480 B | 736440 B | 100.0 % `[########################]` |
| **Whole game** | **1327888 B** | **1327848 B** | **100.00 %** `[########################]` |

- Bytes of machine code in game functions, counted exactly like the objdiff report on decomp.dev (`tools/units.py` is the common list); Sony's Psy-Q library (99096 B) is not counted: 67188 B of it (67.8 %) also matches, from Psy-Q sources ported from psx_tomba, and is tracked apart.
- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each
  other; those copies match automatically and are not added again (counting every copy separately gives 99.8 %).
- Also tracked for MAIN0 + X000: 15.7 % of the code named by us, 76.0 % typed with the TObj structure.

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 99096 bytes (902 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library | Psy-Q matching |
|---|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 228224 B (515 f) | 35.7 % | 56.7 % | 100.0 % (1141 f) | 99056 B | 67148 B (67.8 %) |
| X000.BIN (AREA00 overlay) | 297744 B (526 f) | 0.3 % | 90.8 % | 100.0 % (523 f) | 40 B | 40 B (100.0 %) |

## Area overlays (X001..X019)

Raw code blobs loaded at `0x800E8028` like X000 ([notes/overlays.md](../notes/overlays.md)). Boundaries come from
`tools/areas.py bounds` (`notes/functions_x0nn.csv`); about 173 KB of object code is shared by all of them (and X000)
at the same addresses, so most of it is matched by copying the X000/MAIN0 twin (`tools/areas.py twins`, sources
in `src/x0nn/`). Unmatched functions are listed in [notes/todo_areas.csv](../notes/todo_areas.csv) by status:
*x000* = identical to an X000 function not matched yet, *dup* = identical to a function of an earlier overlay,
*piece* = identical to a Ghidra piece of a larger matched X000 range (needs its own C),
*new* = no twin anywhere (the real new work).

| Overlay | Areas | Code | Functions | Matching | Pending: x000 | dup | piece | new |
|---|---|---|---|---|---|---|---|---|
| X001.BIN | AREA01, AREA07 | 329308 B | 600 | 99.9 % (599 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200304 B | 364 | 99.8 % (362 f) | 0 B | 0 B | 400 B | 40 B (1 f) |
| X003.BIN | AREA03 | 297232 B | 582 | 99.9 % (581 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X004.BIN | AREA04, AREA12 | 280208 B | 567 | 99.9 % (566 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X005.BIN | AREA05 | 180916 B | 328 | 99.8 % (327 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X006.BIN | AREA06 | 210924 B | 395 | 99.8 % (394 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X008.BIN | AREA08 | 175004 B | 306 | 99.8 % (305 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X009.BIN | AREA09 | 257776 B | 504 | 99.8 % (503 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X010.BIN | AREA10 | 274380 B | 531 | 99.4 % (529 f) | 0 B | 1212 B | 400 B | 0 B (0 f) |
| X011.BIN | AREA11 | 190292 B | 354 | 99.8 % (353 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X013.BIN | AREA13 | 182232 B | 327 | 99.8 % (326 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X014.BIN | AREA14 | 236716 B | 490 | 99.8 % (489 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X016.BIN | AREA16 | 185988 B | 357 | 99.8 % (356 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X017.BIN | AREA17 | 188428 B | 348 | 99.8 % (347 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X018.BIN | AREA18 | 191628 B | 352 | 99.8 % (351 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| X019.BIN | AREA19 | 175124 B | 306 | 99.8 % (305 f) | 0 B | 0 B | 400 B | 0 B (0 f) |
| **Total** | | 3556460 B | 6711 | 99.8 % (6693 f) | 0 B | 1212 B | 6400 B | 40 B (1 f) |

The *Matching* column counts every copy, including the shared code matched through twins; the whole-game
figure in the summary counts each function once.

## What is NOT counted (the real denominator is larger)

- `MAIN1..8.EXE`: they share ~98 % of the code (`.text`) with MAIN0; they are treated as variants and not added.
- `SCES_013.31` (loader, 651 KB): almost all Psy-Q library; not analyzed.
- AREA07 and AREA12 reuse X001/X004, AREA15 has no code; X1nn..X8nn are other languages.
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
| 6188 B | `800317c0` | MAIN0.EXE |
| 5740 B | `800f5078` | X000.BIN |
| 5708 B | `8003566c` | MAIN0.EXE |
| 5056 B | `80033b44` | MAIN0.EXE |
| 4360 B | `80051088` | MAIN0.EXE |
| 4088 B | `80100788` | X000.BIN |
| 3908 B | `800fc414` | X000.BIN |
| 3248 B | `80134e5c` | X000.BIN |
| 3152 B | `80133474` | X000.BIN |
| 3132 B | `80029078` | MAIN0.EXE |
| 3124 B | `80130bb0` | X000.BIN |
| 2672 B | `8010f400` | X000.BIN |
| 2632 B | `800f2b98` | X000.BIN |
| 2524 B | `800f705c` | X000.BIN |
| 2488 B | `800f44b0` | X000.BIN |

## How it is measured

- Unit: bytes of code per function (the size reported by Ghidra).
- *Named*: it is in `notes/names_*.csv` and is not a library function. *Typed*: its first parameter is `TObj *`.
- *Matching*: functions in `src/*.c` marked with `// MATCHING <address> <bytes>`; verified byte for byte
  against the retail executable with `tools/ncheck.py` / `tools/matchcheck.py` (relocations masked).
- The typing figures come from the latest Ghidra export (`notes/functions_*.csv`) and may lag behind.
