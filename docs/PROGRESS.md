# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-09). Do not edit by hand._

## Summary

**Whole game: 91.3 % of the game's code matches byte for byte** (1161500 of 1271516 bytes).

| Part | Code | Matching bytes | Matching |
|---|---:|---:|---|
| MAIN0.EXE: engine, shared by every area | 227780 B | 199056 B | 87.4 % `[#####################...]` |
| X000.BIN: AREA00 + object code reused by all areas | 297956 B | 294564 B | 98.9 % `[########################]` |
| X001..X019.BIN: code specific to the other areas | 745780 B | 667880 B | 89.6 % `[#####################...]` |
| **Whole game** | **1271516 B** | **1161500 B** | **91.3 %** `[######################..]` |

- Bytes of machine code in game functions; Sony's Psy-Q library (99540 B) is not counted: 67596 B of it (67.9 %) also matches, from Psy-Q sources ported from psx_tomba, and is tracked apart.
- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each
  other; those copies match automatically and are not added again (counting every copy separately gives 96.9 %).
- Also tracked for MAIN0 + X000: 9.5 % of the code named by us, 76.0 % typed with the TObj structure.

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 99540 bytes (907 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library | Psy-Q matching |
|---|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 227780 B (510 f) | 21.4 % | 56.8 % | 87.4 % (1093 f) | 99500 B | 67556 B (67.9 %) |
| X000.BIN (AREA00 overlay) | 297956 B (528 f) | 0.3 % | 90.7 % | 98.9 % (519 f) | 40 B | 40 B (100.0 %) |

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
| X001.BIN | AREA01, AREA07 | 330276 B | 603 | 93.7 % (578 f) | 1100 B | 0 B | 0 B | 19840 B (23 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200656 B | 366 | 99.3 % (360 f) | 1100 B | 0 B | 0 B | 392 B (4 f) |
| X003.BIN | AREA03 | 301504 B | 589 | 96.6 % (574 f) | 1100 B | 0 B | 0 B | 9128 B (13 f) |
| X004.BIN | AREA04, AREA12 | 282416 B | 572 | 97.1 % (561 f) | 1100 B | 0 B | 0 B | 7000 B (9 f) |
| X005.BIN | AREA05 | 180984 B | 330 | 99.4 % (325 f) | 1100 B | 0 B | 0 B | 68 B (3 f) |
| X006.BIN | AREA06 | 211508 B | 397 | 94.3 % (366 f) | 1100 B | 0 B | 0 B | 10864 B (29 f) |
| X008.BIN | AREA08 | 175296 B | 308 | 99.2 % (304 f) | 1100 B | 0 B | 0 B | 292 B (2 f) |
| X009.BIN | AREA09 | 258724 B | 507 | 96.5 % (494 f) | 1100 B | 0 B | 0 B | 7964 B (11 f) |
| X010.BIN | AREA10 | 275380 B | 535 | 95.0 % (512 f) | 1100 B | 1212 B | 0 B | 11468 B (20 f) |
| X011.BIN | AREA11 | 190544 B | 355 | 98.8 % (347 f) | 1100 B | 0 B | 0 B | 1240 B (6 f) |
| X013.BIN | AREA13 | 182492 B | 330 | 99.0 % (323 f) | 1100 B | 0 B | 0 B | 792 B (5 f) |
| X014.BIN | AREA14 | 236716 B | 490 | 97.0 % (479 f) | 1100 B | 0 B | 0 B | 5884 B (9 f) |
| X016.BIN | AREA16 | 186632 B | 359 | 99.1 % (354 f) | 1100 B | 0 B | 0 B | 644 B (3 f) |
| X017.BIN | AREA17 | 189344 B | 350 | 99.1 % (346 f) | 1100 B | 0 B | 0 B | 600 B (2 f) |
| X018.BIN | AREA18 | 193352 B | 356 | 98.5 % (349 f) | 1100 B | 0 B | 0 B | 1724 B (5 f) |
| X019.BIN | AREA19 | 175124 B | 306 | 99.4 % (304 f) | 1100 B | 0 B | 0 B | 0 B (0 f) |
| **Total** | | 3570948 B | 6753 | 97.3 % (6576 f) | 17600 B | 1212 B | 0 B | 77900 B (144 f) |

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
| 2888 B | `8003afb4` | MAIN0.EXE |
| 2672 B | `8010f400` | X000.BIN |
| 2632 B | `800f2b98` | X000.BIN |
| 2604 B | `80056420` | MAIN0.EXE |

## How it is measured

- Unit: bytes of code per function (the size reported by Ghidra).
- *Named*: it is in `notes/names_*.csv` and is not a library function. *Typed*: its first parameter is `TObj *`.
- *Matching*: functions in `src/*.c` marked with `// MATCHING <address> <bytes>`; verified byte for byte
  against the retail executable with `tools/ncheck.py` / `tools/matchcheck.py` (relocations masked).
- The typing figures come from the latest Ghidra export (`notes/functions_*.csv`) and may lag behind.
