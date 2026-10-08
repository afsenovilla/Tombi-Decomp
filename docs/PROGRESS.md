# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-08). Do not edit by hand._

## Summary

**Whole game: 46.3 % of the game's code matches byte for byte** (589572 of 1272064 bytes).

| Part | Code | Matching bytes | Matching |
|---|---:|---:|---|
| MAIN0.EXE: engine, shared by every area | 228328 B | 198220 B | 86.8 % `[#####################...]` |
| X000.BIN: AREA00 + object code reused by all areas | 297956 B | 294564 B | 98.9 % `[########################]` |
| X001..X019.BIN: code specific to the other areas | 745780 B | 96788 B | 13.0 % `[###.....................]` |
| **Whole game** | **1272064 B** | **589572 B** | **46.3 %** `[###########.............]` |

- Bytes of machine code in game functions; Sony's Psy-Q library (98992 B) is not counted.
- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each
  other; those copies match automatically and are not added again (counting every copy separately gives 82.7 %).
- Also tracked for MAIN0 + X000: 6.5 % of the code named by us, 75.9 % typed with the TObj structure.

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 98992 bytes (901 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library |
|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 228328 B (516 f) | 14.5 % | 56.6 % | 86.8 % (750 f) | 98952 B |
| X000.BIN (AREA00 overlay) | 297956 B (528 f) | 0.3 % | 90.7 % | 98.9 % (519 f) | 40 B |

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
| X001.BIN | AREA01, AREA07 | 330276 B | 603 | 56.3 % (355 f) | 1100 B | 412 B | 92 B | 142876 B (240 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200656 B | 366 | 89.5 % (326 f) | 1100 B | 288 B | 0 B | 19588 B (36 f) |
| X003.BIN | AREA03 | 301504 B | 589 | 63.5 % (380 f) | 1100 B | 716 B | 108 B | 108220 B (196 f) |
| X004.BIN | AREA04, AREA12 | 282416 B | 572 | 70.5 % (398 f) | 1100 B | 1884 B | 144 B | 80112 B (156 f) |
| X005.BIN | AREA05 | 180984 B | 330 | 97.2 % (313 f) | 1100 B | 164 B | 0 B | 3760 B (13 f) |
| X006.BIN | AREA06 | 211508 B | 397 | 85.9 % (325 f) | 1100 B | 12 B | 0 B | 28792 B (69 f) |
| X008.BIN | AREA08 | 175296 B | 308 | 99.1 % (301 f) | 1100 B | 12 B | 0 B | 492 B (4 f) |
| X009.BIN | AREA09 | 258724 B | 507 | 70.6 % (341 f) | 1100 B | 680 B | 104 B | 74264 B (154 f) |
| X010.BIN | AREA10 | 275380 B | 535 | 66.9 % (370 f) | 1100 B | 3196 B | 36 B | 86836 B (150 f) |
| X011.BIN | AREA11 | 190544 B | 355 | 92.3 % (315 f) | 1100 B | 164 B | 24 B | 13404 B (35 f) |
| X013.BIN | AREA13 | 182492 B | 330 | 96.1 % (310 f) | 1100 B | 12 B | 0 B | 5984 B (17 f) |
| X014.BIN | AREA14 | 236716 B | 490 | 76.6 % (357 f) | 1100 B | 376 B | 40 B | 53832 B (127 f) |
| X016.BIN | AREA16 | 186632 B | 359 | 94.7 % (327 f) | 1100 B | 456 B | 24 B | 8328 B (25 f) |
| X017.BIN | AREA17 | 189344 B | 350 | 94.6 % (325 f) | 1100 B | 12 B | 16 B | 9012 B (21 f) |
| X018.BIN | AREA18 | 193352 B | 356 | 92.2 % (323 f) | 1100 B | 820 B | 0 B | 13172 B (26 f) |
| X019.BIN | AREA19 | 175124 B | 306 | 99.2 % (301 f) | 1100 B | 12 B | 0 B | 320 B (2 f) |
| **Total** | | 3570948 B | 6753 | 81.1 % (5367 f) | 17600 B | 9216 B | 588 B | 648992 B (1271 f) |

The *Matching* column counts every copy, including the shared code matched through twins; the whole-game
figure in the summary counts each function once.

## What is NOT counted (the real denominator is larger)

- `MAIN1..8.EXE`: they share ~98 % of the code (`.text`) with MAIN0; they are treated as variants and not added.
- `SCES_013.31` (loader, 651 KB): almost all Psy-Q library; not analyzed.
- AREA07 and AREA12 reuse X001/X004, AREA15 has no code; X1nn..X8nn are other languages.
- Functions called only by an overlay that Ghidra does not recognize, and overlays not yet identified
  (172 MAIN0 targets at `0x800E8000+` do not fall in X000).
- Functions outside the code (`479` bytes of Ghidra false positives in RAM with no contents).

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
| 3080 B | `80024ea0` | MAIN0.EXE |
| 2888 B | `8003afb4` | MAIN0.EXE |
| 2672 B | `8010f400` | X000.BIN |
| 2632 B | `800f2b98` | X000.BIN |

## How it is measured

- Unit: bytes of code per function (the size reported by Ghidra).
- *Named*: it is in `notes/names_*.csv` and is not a library function. *Typed*: its first parameter is `TObj *`.
- *Matching*: functions in `src/*.c` marked with `// MATCHING <address> <bytes>`; verified byte for byte
  against the retail executable with `tools/ncheck.py` / `tools/matchcheck.py` (relocations masked).
- The typing figures come from the latest Ghidra export (`notes/functions_*.csv`) and may lag behind.
