# What is left to match (2026-11-08)

Ordered from closest to farthest, per version. Best drafts of the open functions live outside the repo (session
scratchpads); the notes below say what was tried, so the next attempt does not repeat it. How to check and install:
[AGENTS.md](../AGENTS.md). Per-round history: [docs/JP.md](JP.md), [docs/STATUS.md](STATUS.md).

## PAL (SCES-01331): 99.80 % of game code (Psy-Q library counted apart)

MAIN0, X000 and X001..X019 are at 100 %. Left (`notes/todo_areas.csv`):

| Function | Size | State | Notes |
|---|---:|---|---|
| D0SPSUB 800EFD6C | 3012 B | score 659 | best draft in the 2026-11-09 session scratchpad: the `s->field` loads must stay after the stores to the Text globals (one-field-struct aliases of the same symbols at chosen store sites helped); left: loop-1 attr/font store schedule, the y store in later loops, then registers. |
| D0SPSUB3 functions | up to 7292 B | open | see `notes/todo_areas.csv` |
| Psy-Q library | 10.9 KB of 100.9 KB | 89.2 % | not counted in the game figure; sources ported from psx_tomba |

## Japanese retail (SLPS-01144): 98.24 % with the SYSTEM overlays (98.91 % without; 3467 of 3530 functions)

The SYSTEM overlays (no GOVER on the JP disc) were added on 2026-11-08; a second pass the same day matched 75 more (DSPSUB/DSPSUB3 54, OPTSUB00 8, DSPSUB2 13) and a third 65 (INFO, INFO2, INFO3: all of them). Left there: DSPSUB2 func_800ECCE8 (2 lines: `li a1,498` must stay in the jal delay slot), MapStep EF9B0, F0D90 (twin of PAL 800EFD6C).

The JP report counts the Psy-Q library too. The hand-written-asm part (libgte, BIOS stubs, start, setjmp) is in as
top-level asm. Left from the library: the parts Sony compiled from C (libsnd _SsContDataEntry and the _SsVmSetVol
pieces, __main, __do_global_dtors, _patch_card/_patch_card2, DecDCTvlc/DecDCTvlcSize, StopCARD2's merged target).

Game code, closest first:

| Function | Size | State | Notes |
|---|---:|---|---|
| main 8001AA90 | 904 B | 99.9 % | instruction-identical; jump-table naming only |
| x06 8011DD3C | 644 B | 99.4 % | instruction-identical; the target includes one padding `nop` before the next function |
| x04 80118720 | 624 B | 2 lines (`addiu a1,v1,26` for `s = &f->flag` must come after the hoisted `li 208/255/1`; `st` pinned to $5 fixed the rest) | retail loads q[-2] before q[0] in both inner cases; sibling 80118B98 matched with block-scoped pinned q pointers |
| x04 80117938 | 2768 B | ~300 lines | tile-grid visibility list, no PAL twin |
| main 80024B78 | 5112 B | not started | hand-written GTE mesh renderer, no twin; write from scratch with the GTE macros of src/modelDrawG4Lit.c |
| main _SsVmSetVol | 880 B | split | splat cut one 0x584 function into five pieces (`VM_VOL_OBJ_*`): fix the split first |

## USA retail (SCUS-94236): 99.45 % with the SYSTEM overlays (3673 of 3681 functions)

The boot exe and the area overlays X00..X19 match in every game function (4 Sony functions left, below). The SYSTEM
overlays were added on 2026-11-08 at 87-100 % (GOVER, INFO*) and 45-66 % (DSPSUB*, OPTSUB00): their big drawing,
memory-card and title/options functions differ from PAL in the English text layout and need hand work from the PAL
C (docs/USA.md).

Left: Sony library code that Sony compiled from C: libsn `__main` / `__do_global_dtors`, libapi
psyq_8005ba74 / psyq_8005bc10 (psx_tomba has them as C in src/psyq/libapi/func_8005B4CC.c).

## Demos and prototypes

`docs/JPDEMO.md`, `docs/BETA_980203.md`, `docs/PREPRE8.md`; the CI `jp-sweep.yml` installs what matches on every
push.

## Ready but not installed

psx_tomba's func_80044B0C / func_80058E14 also give the PAL twins func_800416B8 / func_8004AE7C (C ready in the
2026-11-08 session notes); the port has hand-written copies of both (port2/runtime/main0_handwritten.c): check
the port build before adding PAL files for them.
