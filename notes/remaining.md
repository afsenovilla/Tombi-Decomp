# Remaining unmatched functions

State at the close of the naming phase (October 2026): `tools/build_full.py` links MAIN0.EXE, X000.BIN and the eight
SYSTEM overlays byte for byte, every area overlay X001..X019 is 100 % matched through its twins, and the whole-game
figure of `docs/PROGRESS.md` is 98.95 %: 2950 of 2955 functions. The five that do not match are all in the memory
card / sub-screen overlays (D0SPSUB, D0SPSUB2, D0SPSUB3). Each one has a functionally equivalent C draft in `src/wip/`
(not counted as progress; the build keeps the retail assembly for the range) whose header comment is the record of
what was tried, what fixed the structure, and what remains. Scores are `tools/ncheck.py --score` (0 = identical);
every draft compiles with the default gcc-2.7.2-psx + maspsx, no `// CC` or `// FLAGS` header.

| Unit | Function | Address | Size | Score | Draft | What remains |
|---|---|---|---|---|---|---|
| D0SPSUB | func_800E89F8 | 800E89F8 | 4280 | 6 | `src/wip/d0spsub_func_800E89F8.c` | sub-screen state machine; registers only after the case-0x84 tail fix (in-struct stores to D_8009CD98..9E so the scheduler stall reproduces) |
| D0SPSUB | func_800ED0C4 | 800ED0C4 | 3488 | 20 | `src/wip/d0spsub_func_800ED0C4.c` | memory card menu step; the schedule matches, the case-3 confirm block differs in allocation only (the `t`/3 shared pseudo dies in two places and goes to global alloc) |
| D0SPSUB | func_800EFD6C | 800EFD6C | 3012 | 786 | `src/wip/d0spsub_func_800EFD6C.c` | text/slot drawing loops; prologue, 0x90 frame, s0-s4 and the first 0x138 bytes match, the rest is shifted register names (the score counts every one) |
| D0SPSUB2 | func_800E999C | 800E999C | 1852 | 182 | `src/wip/d0spsub2_func_800E999C.c` | memory card save: builds the header in SAVE_DATA_BUFFER; structure matches, s0/s1 swapped (global alloc ranks the buffer pointer above the slot parameter) |
| D0SPSUB3 | func_800EC89C | 800EC89C | 3424 | 743 | `src/wip/d0spsub3_func_800EC89C.c` | port of D0SPSUB func_800ED0C4 with the three retail differences (subScreenMoveTabCursor, mask 0x5008, no mode cases 4..7); inherits the open points of its source |

D0SPSUB3 also carries the 4280-byte twin of D0SPSUB func_800E89F8 (`dup` in the overlay table of `docs/PROGRESS.md`);
it follows automatically once the D0SPSUB function matches, the same way the x0NN twins do.

All five are register allocation and scheduler order problems with the structure already right; the drafts in `src/wip`
record the GCC 2.7.2 observations (sched1 stalls, `birthing_insn_p`, local vs global alloc ranking) that moved the
scores, so the next attempt starts from them, not from scratch. The earlier triage of this file (64 unmatched functions,
the DATA reclassifications of x000 80116258 / 801162B8, x001 8011658C, x003 80116D50, x005 / x013 801188FC, x005 / x008
80117CAC, x016 8011B170, x018 8011C758 and x002 8011E688, with the proofs that their jals cannot run while the overlay is
loaded) is in the git history of `notes/remaining.md` up to October 26 2026; the reclassified ranges are `DATA` entries
of `tools/units.py`.
