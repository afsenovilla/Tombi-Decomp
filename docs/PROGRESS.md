# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-08). Do not edit by hand._

## Summary

| Level | Progress | Bytes | Functions |
|---|---|---|---|
| **C that matches byte for byte (matching)** | **92.4 %** `[######################..]` | 486160 / 526284 | 1259 |
| Named by us | 6.5 % `[##......................]` | 34236 / 526284 | 189 / 1044 |
| With the TObj structure applied (coverage, not C progress) | 75.9 % `[##################......]` | 399676 / 526284 | 568 / 1044 |
| Matching, all 18 programs (MAIN0 + X000 + the 16 other area overlays) | 80.0 % `[###################.....]` | 3278808 / 4097232 | 6091 |

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 98992 bytes (901 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library |
|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 228328 B (516 f) | 14.5 % | 56.6 % | 84.4 % (741 f) | 98952 B |
| X000.BIN (AREA00 overlay) | 297956 B (528 f) | 0.3 % | 90.7 % | 98.5 % (518 f) | 40 B |

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
| X001.BIN | AREA01, AREA07 | 330276 B | 603 | 53.0 % (306 f) | 1100 B | 584 B | 92 B | 153320 B (284 f) |
| X002.BIN | AREA02, AREA19 (alt.) | 200656 B | 366 | 86.6 % (299 f) | 1100 B | 312 B | 40 B | 25528 B (59 f) |
| X003.BIN | AREA03 | 301504 B | 589 | 58.4 % (312 f) | 1100 B | 1352 B | 108 B | 122772 B (255 f) |
| X004.BIN | AREA04, AREA12 | 282416 B | 572 | 63.1 % (317 f) | 1100 B | 2832 B | 168 B | 100176 B (222 f) |
| X005.BIN | AREA05 | 180984 B | 330 | 96.0 % (298 f) | 1100 B | 344 B | 0 B | 5872 B (26 f) |
| X006.BIN | AREA06 | 211508 B | 397 | 82.5 % (299 f) | 1100 B | 1028 B | 0 B | 34796 B (94 f) |
| X008.BIN | AREA08 | 175296 B | 308 | 99.1 % (298 f) | 1100 B | 12 B | 0 B | 516 B (7 f) |
| X009.BIN | AREA09 | 258724 B | 507 | 67.6 % (301 f) | 1100 B | 956 B | 104 B | 81636 B (190 f) |
| X010.BIN | AREA10 | 275380 B | 535 | 63.9 % (310 f) | 1100 B | 4000 B | 56 B | 94360 B (198 f) |
| X011.BIN | AREA11 | 190544 B | 355 | 91.1 % (298 f) | 1100 B | 344 B | 24 B | 15408 B (50 f) |
| X013.BIN | AREA13 | 182492 B | 330 | 95.2 % (298 f) | 1100 B | 12 B | 0 B | 7712 B (29 f) |
| X014.BIN | AREA14 | 236716 B | 490 | 73.4 % (301 f) | 1100 B | 524 B | 40 B | 61336 B (180 f) |
| X016.BIN | AREA16 | 186632 B | 359 | 93.1 % (298 f) | 1100 B | 928 B | 24 B | 10912 B (48 f) |
| X017.BIN | AREA17 | 189344 B | 350 | 92.2 % (300 f) | 1100 B | 148 B | 44 B | 13388 B (40 f) |
| X018.BIN | AREA18 | 193352 B | 356 | 89.8 % (299 f) | 1100 B | 832 B | 0 B | 17704 B (49 f) |
| X019.BIN | AREA19 | 175124 B | 306 | 99.2 % (298 f) | 1100 B | 12 B | 0 B | 344 B (5 f) |
| **Total** | | 3570948 B | 6753 | 78.2 % (4832 f) | 17600 B | 14220 B | 700 B | 745780 B (1736 f) |

Grand total, all programs: **3278808 / 4097232 B matching (80.0 %)**; core (MAIN0 + X000) 92.4 %, area overlays 78.2 %.

## What is NOT counted (the real denominator is larger)

- `MAIN1..8.EXE`: they share ~98 % of the code (`.text`) with MAIN0; they are treated as variants and not added.
- `SCES_013.31` (loader, 651 KB): almost all Psy-Q library; not analyzed.
- The overlays are only counted in the "area overlays" section and the all-programs row: the summary above is MAIN0 + X000.
  AREA07 and AREA12 reuse X001/X004, AREA15 has no code; X1nn..X8nn are other languages.
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
