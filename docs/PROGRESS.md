# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-08). Do not edit by hand._

## Summary

**Whole game: 41.4 % of the game's code matches byte for byte** (526112 of 1272064 bytes).

| Part | Code | Matching bytes | Matching |
|---|---:|---:|---|
| MAIN0.EXE: engine, shared by every area | 228328 B | 197996 B | 86.7 % `[#####################...]` |
| X000.BIN: AREA00 + object code reused by all areas | 297956 B | 294564 B | 98.9 % `[########################]` |
| X001..X019.BIN: code specific to the other areas | 745780 B | 33552 B | 4.5 % `[#.......................]` |
| **Whole game** | **1272064 B** | **526112 B** | **41.4 %** `[##########..............]` |

- Bytes of machine code in game functions; Sony's Psy-Q library (98992 B) is not counted.
- Each function counts once. The 16 area overlays share about 4800 functions with MAIN0/X000 or with each
  other; those copies match automatically and are not added again (counting every copy separately gives 81.1 %).
- Also tracked for MAIN0 + X000: 6.5 % of the code named by us, 75.9 % typed with the TObj structure.

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 98992 bytes (901 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library |
|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 228328 B (516 f) | 14.5 % | 56.6 % | 86.7 % (749 f) | 98952 B |
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
| X001.BIN | AREA01, AREA07 | 330276 B | 603 | 53.9 % (329 f) | 1100 B | 548 B | 92 B | 150448 B (264 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200656 B | 366 | 88.3 % (313 f) | 1100 B | 288 B | 40 B | 22040 B (47 f) |
| X003.BIN | AREA03 | 301504 B | 589 | 59.1 % (332 f) | 1100 B | 880 B | 108 B | 121368 B (242 f) |
| X004.BIN | AREA04, AREA12 | 282416 B | 572 | 64.9 % (348 f) | 1100 B | 2336 B | 144 B | 95560 B (201 f) |
| X005.BIN | AREA05 | 180984 B | 330 | 96.5 % (306 f) | 1100 B | 332 B | 0 B | 4852 B (19 f) |
| X006.BIN | AREA06 | 211508 B | 397 | 84.6 % (313 f) | 1100 B | 12 B | 0 B | 31464 B (81 f) |
| X008.BIN | AREA08 | 175296 B | 308 | 99.1 % (301 f) | 1100 B | 12 B | 0 B | 492 B (4 f) |
| X009.BIN | AREA09 | 258724 B | 507 | 68.7 % (318 f) | 1100 B | 932 B | 104 B | 78924 B (175 f) |
| X010.BIN | AREA10 | 275380 B | 535 | 65.2 % (341 f) | 1100 B | 3904 B | 56 B | 90684 B (174 f) |
| X011.BIN | AREA11 | 190544 B | 355 | 92.2 % (314 f) | 1100 B | 332 B | 24 B | 13404 B (35 f) |
| X013.BIN | AREA13 | 182492 B | 330 | 95.5 % (305 f) | 1100 B | 12 B | 0 B | 7148 B (22 f) |
| X014.BIN | AREA14 | 236716 B | 490 | 74.0 % (315 f) | 1100 B | 524 B | 40 B | 59868 B (166 f) |
| X016.BIN | AREA16 | 186632 B | 359 | 94.0 % (318 f) | 1100 B | 456 B | 24 B | 9676 B (34 f) |
| X017.BIN | AREA17 | 189344 B | 350 | 93.4 % (315 f) | 1100 B | 112 B | 16 B | 11280 B (29 f) |
| X018.BIN | AREA18 | 193352 B | 356 | 91.4 % (314 f) | 1100 B | 820 B | 0 B | 14700 B (35 f) |
| X019.BIN | AREA19 | 175124 B | 306 | 99.2 % (301 f) | 1100 B | 12 B | 0 B | 320 B (2 f) |
| **Total** | | 3570948 B | 6753 | 79.2 % (5083 f) | 17600 B | 11512 B | 648 B | 712228 B (1530 f) |

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
