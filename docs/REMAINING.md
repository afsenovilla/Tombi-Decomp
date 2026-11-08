# What is left to match (2026-11-08)

Ordered from closest to farthest, per version. Best drafts of the open functions live outside the repo (session
scratchpads); the notes below say what was tried, so the next attempt does not repeat it. How to check and install:
[AGENTS.md](../AGENTS.md). Per-round history: [docs/JP.md](JP.md), [docs/STATUS.md](STATUS.md).

## PAL (SCES-01331): 99.68 % of game code (Psy-Q library counted apart)

MAIN0, X000 and X001..X019 are at 100 %. Left (`notes/todo_areas.csv`):

| Function | Size | State | Notes |
|---|---:|---|---|
| D0SPSUB2 800E999C | 1852 B | score 178 | `src/wip/d0spsub2_func_800E999C.c`. Retail h = s0, slot = s1, and the 0x180 block copy runs on s0 itself; the copy destination is a fresh pseudo that gets a3 in ours. A global-alloc order problem: h must be allocated before the copy destination. Barriers on h fix the s0/s1 swap but break the B32 copy's v1 and the `lh D_1F8003D8` hoist. ~90 attempts and 4 permuter runs so far. |
| D0SPSUB 800EFD6C | 3012 B | score 786 | `src/wip/d0spsub_func_800EFD6C.c`, registers shifted through most of the function; barely attempted. |
| D0SPSUB3 functions | up to 7292 B | open | see `notes/todo_areas.csv` |
| Psy-Q library | 10.9 KB of 100.9 KB | 89.2 % | not counted in the game figure; sources ported from psx_tomba |

## Japanese retail (SLPS-01144): 98.62 % (68 functions, ~19.5 KB)

The JP report counts the Psy-Q library too. The hand-written-asm part (libgte, BIOS stubs, start, setjmp) is in as
top-level asm. Left from the library: the parts Sony compiled from C (libsnd _SsContDataEntry and the _SsVmSetVol
pieces, __main, __do_global_dtors, _patch_card/_patch_card2, DecDCTvlc/DecDCTvlcSize, StopCARD2's merged target).

Game code, closest first:

| Function | Size | State | Notes |
|---|---:|---|---|
| main 8001AA90 | 904 B | 99.9 % | instruction-identical; jump-table naming only |
| x06 8011DD3C | 644 B | 99.4 % | instruction-identical; the target includes one padding `nop` before the next function |
| main 8001CA50 | 1024 B | 48 lines (flags -O2 -G0) | map-tile spawn loop; loop-invariant hoist order, frame 96 vs 104 |
| x04 80118720 | 624 B | 32 lines | retail loads q[-2] before q[0] in both inner cases; sibling 80118B98 matched with block-scoped pinned q pointers |
| x04 80117938 | 2768 B | ~300 lines | tile-grid visibility list, no PAL twin |
| main 80024B78 | 5112 B | not started | hand-written GTE mesh renderer, no twin; write from scratch with the GTE macros of src/modelDrawG4Lit.c |
| main 800249B8 | 448 B | 2 lines | GTE inline asm (the permuter cannot parse it) |
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
