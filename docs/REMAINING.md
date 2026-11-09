# What is left to match (2026-11-08)

Ordered from closest to farthest, per version. Best drafts of the open functions live outside the repo (session
scratchpads); the notes below say what was tried, so the next attempt does not repeat it. How to check and install:
[AGENTS.md](../AGENTS.md). Per-round history: [docs/JP.md](JP.md), [docs/STATUS.md](STATUS.md).

## PAL (SCES-01331): 100 % of game code (2026-11-09)

Every game function of every overlay matches; the Psy-Q library matches too (100876 of 100904 B counted; every unit
has a source). The last one was D0SPSUB 800EFD6C: one store order (x, y, w, attr, str, font) in every block, and the
store forms (one-field aliases vs plain externs) decide the scheduling against the `s->` loads.

## Japanese retail (SLPS-01144): 99.71 % with the SYSTEM overlays (3527 of 3528 functions)

Everything matches, Psy-Q library included, except dspsub func_800F0D90 (4720 B). It is not a plain twin of PAL
D0SPSUB 800EFD6C: 7 cases instead of 4, menu global D_800A6120, Text globals D_80111408..., 19 calls of the JP
DrawSprite func_800EC9D8; it has to be written from the JP disassembly.

## USA retail (SCUS-94236): 100 % with the SYSTEM overlays (3681 of 3681 functions, 2026-11-09)

## Demos and prototypes

`docs/JPDEMO.md`, `docs/BETA_980203.md`, `docs/PREPRE8.md`; the CI `jp-sweep.yml` installs what matches on every
push.

## Ready but not installed

psx_tomba's func_80044B0C / func_80058E14 also give the PAL twins func_800416B8 / func_8004AE7C (C ready in the
2026-11-08 session notes); the port has hand-written copies of both (port2/runtime/main0_handwritten.c): check
the port build before adding PAL files for them.
