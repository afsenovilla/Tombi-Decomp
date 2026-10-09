# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-09). Do not edit by hand._

## Summary

**Whole game: 86.6 % of the game's code matches byte for byte** (1101444 of 1272064 bytes).

| Part | Code | Matching bytes | Matching |
|---|---:|---:|---|
| MAIN0.EXE: engine, shared by every area | 228328 B | 198220 B | 86.8 % `[#####################...]` |
| X000.BIN: AREA00 + object code reused by all areas | 297956 B | 294564 B | 98.9 % `[########################]` |
| X001..X019.BIN: code specific to the other areas | 745780 B | 608660 B | 81.6 % `[####################....]` |
| **Whole game** | **1272064 B** | **1101444 B** | **86.6 %** `[#####################...]` |

- Bytes of machine code in game functions; Sony's Psy-Q library (98992 B) is not counted.
- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each
  other; those copies match automatically and are not added again (counting every copy separately gives 95.4 %).
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
| X001.BIN | AREA01, AREA07 | 330276 B | 603 | 90.1 % (565 f) | 1100 B | 0 B | 0 B | 31664 B (36 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200656 B | 366 | 98.0 % (357 f) | 1100 B | 12 B | 0 B | 2920 B (6 f) |
| X003.BIN | AREA03 | 301504 B | 589 | 92.7 % (554 f) | 1100 B | 12 B | 44 B | 20932 B (30 f) |
| X004.BIN | AREA04, AREA12 | 282416 B | 572 | 93.9 % (544 f) | 1100 B | 536 B | 0 B | 15672 B (24 f) |
| X005.BIN | AREA05 | 180984 B | 330 | 99.3 % (324 f) | 1100 B | 12 B | 0 B | 68 B (3 f) |
| X006.BIN | AREA06 | 211508 B | 397 | 93.1 % (357 f) | 1100 B | 12 B | 0 B | 13576 B (37 f) |
| X008.BIN | AREA08 | 175296 B | 308 | 99.2 % (303 f) | 1100 B | 12 B | 0 B | 292 B (2 f) |
| X009.BIN | AREA09 | 258724 B | 507 | 94.8 % (483 f) | 1100 B | 12 B | 0 B | 12308 B (21 f) |
| X010.BIN | AREA10 | 275380 B | 535 | 91.0 % (494 f) | 1100 B | 1396 B | 0 B | 22404 B (36 f) |
| X011.BIN | AREA11 | 190544 B | 355 | 98.8 % (346 f) | 1100 B | 12 B | 0 B | 1240 B (6 f) |
| X013.BIN | AREA13 | 182492 B | 330 | 99.0 % (322 f) | 1100 B | 12 B | 0 B | 792 B (5 f) |
| X014.BIN | AREA14 | 236716 B | 490 | 94.8 % (466 f) | 1100 B | 12 B | 40 B | 11080 B (19 f) |
| X016.BIN | AREA16 | 186632 B | 359 | 99.1 % (353 f) | 1100 B | 12 B | 0 B | 644 B (3 f) |
| X017.BIN | AREA17 | 189344 B | 350 | 98.5 % (342 f) | 1100 B | 12 B | 0 B | 1804 B (5 f) |
| X018.BIN | AREA18 | 193352 B | 356 | 98.5 % (348 f) | 1100 B | 12 B | 0 B | 1724 B (5 f) |
| X019.BIN | AREA19 | 175124 B | 306 | 99.4 % (303 f) | 1100 B | 12 B | 0 B | 0 B (0 f) |
| **Total** | | 3570948 B | 6753 | 95.6 % (6461 f) | 17600 B | 2088 B | 84 B | 137120 B (238 f) |

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
