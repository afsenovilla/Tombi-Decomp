# Tombi! decompilation progress (PAL Spanish, SCES_013.31)

_Generated with `python tools/progress.py` (2026-10-08). Do not edit by hand._

## Summary

| Level | Progress | Bytes | Functions |
|---|---|---|---|
| **C that matches byte for byte (matching)** | **59.3 %** `[##############..........]` | 312152 / 526284 | 1088 |
| Named by us | 6.5 % `[##......................]` | 34236 / 526284 | 189 / 1044 |
| With the TObj structure applied (coverage, not C progress) | 75.9 % `[##################......]` | 399676 / 526284 | 568 / 1044 |

"Game code" = functions inside the code of the analyzed programs, **excluding** those from
Sony's library (Psy-Q), which Ghidra already identifies. Those are an extra 98992 bytes (901 functions).

## Breakdown by program

| Program | Game code | Named | Typed (TObj) | Matching | Psy-Q library |
|---|---|---|---|---|---|
| MAIN0.EXE (game core) | 228328 B (516 f) | 14.5 % | 56.6 % | 71.1 % (680 f) | 98952 B |
| X000.BIN (AREA00 overlay) | 297956 B (528 f) | 0.3 % | 90.7 % | 50.3 % (408 f) | 40 B |

## What is NOT counted (the real denominator is larger)

- `MAIN1..8.EXE`: they share ~98 % of the code (`.text`) with MAIN0; they are treated as variants and not added.
- `SCES_013.31` (loader, 651 KB): almost all Psy-Q library; not analyzed.
- The remaining `X*.BIN` overlays of the 20 areas (only `AREA00/X000.BIN` has been analyzed).
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
