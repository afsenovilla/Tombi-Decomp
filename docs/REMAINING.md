# What is left to match (2026-11-08)

Ordered from closest to farthest, per version. Best drafts of the open functions live outside the repo (session
scratchpads); the notes below say what was tried, so the next attempt does not repeat it. How to check and install:
[AGENTS.md](../AGENTS.md). Per-round history: [docs/JP.md](JP.md), [docs/STATUS.md](STATUS.md).

## PAL (SCES-01331): 99.80 % of game code (Psy-Q library counted apart)

MAIN0, X000 and X001..X019 are at 100 %. Left (`notes/todo_areas.csv`):

| Function | Size | State | Notes |
|---|---:|---|---|
| D0SPSUB 800EFD6C | 3012 B | score 185 (matchcheck 156) | best draft `notes/drafts/d0spsub_func_800EFD6C_score185.c` (frame 0x90 and prologue match). Left: loop 1 keeps the `D_801091C4` base in s4 all loop, s2 as base for the attr/font stores, y/attr/w store order; then register names. Store forms were searched at random (keep the frame at 0x90: `D_8011113C.y/.w/.x` in later loops shrinks it to 0x80, a dead end). |
| Psy-Q library | 100876 of 100904 B | 100 % (rounded) | not counted in the game figure; every Psy-Q unit has a source (2026-11-09: the last 87 ported from the JP/USA files) |

## Japanese retail (SLPS-01144): 99.71 % with the SYSTEM overlays (3527 of 3528 functions)

Everything matches, Psy-Q library included, except dspsub func_800F0D90 (4720 B). It is not a plain twin of PAL
D0SPSUB 800EFD6C: 7 cases instead of 4, menu global D_800A6120, Text globals D_80111408..., 19 calls of the JP
DrawSprite func_800EC9D8; it has to be written from the JP disassembly.

## USA retail (SCUS-94236): 99.83 % with the SYSTEM overlays (3680 of 3681 functions)

Everything matches except dspsub 800EEE84 (2956 B, the USA twin of PAL D0SPSUB 800EFD6C, which is still open).

## Demos and prototypes

`docs/JPDEMO.md`, `docs/BETA_980203.md`, `docs/PREPRE8.md`; the CI `jp-sweep.yml` installs what matches on every
push.

## Ready but not installed

psx_tomba's func_80044B0C / func_80058E14 also give the PAL twins func_800416B8 / func_8004AE7C (C ready in the
2026-11-08 session notes); the port has hand-written copies of both (port2/runtime/main0_handwritten.c): check
the port build before adding PAL files for them.
