# Remaining unmatched functions (triage)

Source: `build/report.json` (mtime 2026-10-14 09:27; HEAD 6d90603 at 11:27 only touched tools/text/psxemu/dist/notes, no src or asm, so the report is still current: 2524/2588 matched, 64 unmatched; not rerun; since then 8 of the 11 DATA entries were reclassified as data, see below). Wip scores from `tools/ncheck.py --score` (0 = identical). No wip file carries a `// CC` or `// AS gnu` header: every wip compiles with the default gcc-2.7.2-psx (+ maspsx 2.86). A wip that spans several csv pieces repeats the whole-wip score on each piece. Addresses are the function's own, the overlay is the unit. The `kept as asm` WARN lines (`build/baseline_asm.txt`) are all rodata/overlap issues in already matched Psy-Q/stub units; none is one of these 64, so there is no LIB entry.

Categories: DATA = table/padding misread as a function (verified by reading the words), LIB = Psy-Q SDK code, NEAR = game code with score <= 10, FAR = game code with score > 10 or no wip.

## Totals

| Category | Functions | Bytes |
|---|---|---|
| DATA | 3 | 1168 |
| LIB | 0 | 0 |
| NEAR | 1 | 396 |
| FAR | 52 | 43888 |
| total | 56 | 45452 |

Reclassified as data (second batch, `DATA` in `tools/units.py`, 884 B): x000 80116258 (96) and 801162B8 (116), x001 8011658C (52; the row continues as code from 801165C0), x003 80116D50 (548; code from 80116F74), x005 and x013 801188FC (20 each), x005 and x008 80117CAC (16 each). Their only jals are dispatcher cases of other areas (second dispatcher: area 7 -> 80116258, 12 -> 801162B8, 6 -> 80116D50, 3 -> 801188FC; first: 17 -> 8011658C, 0 -> 80117CAC) and the MAIN0 pointers are slots of other areas' tables (801162D0/80116DD0/80116E4C: areas 13/9/9; 80116EFC: area 17); the object-type slots (801162D0, 801165A8, 80116EC0) belong to types whose handlers are real code in other overlays. The three KEPT rows below are called by the player-step code of the overlay that holds them, so they stay (and the earlier "no refs" note for x002 8011E688 was wrong).

## Table

| Cat | Unit | Function | Address | Size | Best score | Wip / compiler | Main obstacle |
|---|---|---|---|---|---|---|---|
| DATA | x018 | func_8011C758 | 8011C758 | 680 | n/a | - | KEPT: pointer table (170 words) but FUN_80106a18 (shared player step 0x48, present in X018 at 80106AE8) jals it when the held/hit object kind is 0xE, so it can run while X018 is loaded; x006 8011C758 is real code |
| DATA | x016 | func_8011B170 | 8011B170 | 448 | n/a | - | KEPT: pointer table (112 words) but func_80100788 (shared player step 0x47) jals it from X016 itself (80101634); step 0x47 is only set by X014 code, not provable dead |
| DATA | x002 | func_8011E688 | 8011E688 | 40 | n/a | - | KEPT: 10 words of s16 offset pairs (anim data, pointed to from 8011EFDC) but func_80100788 step 0x25 jals it from X002 itself (801011BC); not provable dead |
| NEAR | x001 | func_80127700 | 80127700 | 396 | 2 | src/x001/wip/func_80127700.c (gcc-2.7.2-psx) | one operand-order diff: addu v1,a0,v1 vs v1,v1,a0 (permuter 100k+ iters, ~250 forms tried) |
| FAR | x004 | func_80125818 | 80125818 | 3148 | 204 | src/x004/wip/func_80125818.c (gcc-2.7.2-psx) | 11-case substep AI draft 3156/3148 B; see header for case 4/5/10 leads; near() a0/v1 regs |
| FAR | main0 | func_80056420 | 80056420 | 2604 | 31 | src/wip/drawBalloonFrame.c (gcc-2.7.2-psx) | drawBalloonFrame; o pinned $17; left: o->f load reg, x+w temp reg, uv sched |
| FAR | x001 | func_80122F54 | 80122F54 | 2192 | 411 | src/x001/wip/func_80122F54.c (gcc-2.7.2-psx) | o/s0-s4 pinned, BLKA-first ladders; left: rsin arg/TR order, BLKB cross-jump, s1 temp |
| FAR | x001 | func_801278C0 | 801278C0 | 1820 | 72 | src/x001/wip/func_801278C0.c (gcc-2.7.2-psx) | rebuilt from 244A0 top; negated forms fix reassociation; left: s-reg permutation, box0 temp, abs(cy) test |
| FAR | x001 | func_801244A0 | 801244A0 | 1800 | 52 | src/x001/wip/func_801244A0.c (gcc-2.7.2-psx) | close; remaining regalloc/load-order diffs after ported by-temp trick |
| FAR | x001 | func_80127074 | 80127074 | 1544 | 314 | src/x001/wip/func_80127074.c (gcc-2.7.2-psx) | 278C0-style rewrite, p/e pinned; left: regalloc of abs/k/fx/fy temps |
| FAR | x010 | func_80125FB4 | 80125FB4 | 1280 | 124 | src/x010/wip/func_80125FB4.c (gcc-2.7.2-psx) | case 5 cross-jumped into case 8 in the game |
| FAR | x001 | func_801253AC | 801253AC | 1212 | 226 | src/x001/wip/func_801253AC.c (gcc-2.7.2-psx) | frame fixed by pad[16], int sabs inline; left sabs negate source, local regalloc |
| FAR | x010 | func_8011EAE8 | 8011EAE8 | 1200 | 307 | src/x010/wip/func_8011E990.c (gcc-2.7.2-psx) | piece of 1544 B wip (see 8011E990) |
| FAR | x010 | func_8011752C | 8011752C | 1020 | 92 | src/x010/wip/func_8011752C.c (gcc-2.7.2-psx) | mask/shift temps and store interleave; regalloc |
| FAR | main0 | func_800402ec | 800402EC | 880 | 122 | src/wip/func_800402EC.c (gcc-2.7.2-psx) | x/ys/cnt unpinned, (x-1)-sx order fixed; left: bs/d regs, b sext in place, xm copy to t1, branch-1 tail cross-jump |
| FAR | x010 | func_8011E358 | 8011E358 | 856 | 54 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
| FAR | x010 | func_80120C88 | 80120C88 | 824 | 16 | src/x010/wip/func_80120C88.c (gcc-2.7.2-psx) | box0/box1 load order, permuter already ran 40k iterations |
| FAR | main0 | func_8003ff48 | 8003FF48 | 816 | 99 | src/wip/func_8003FF48.c (gcc-2.7.2-psx) | global-alloc order o/k/xi (o has lowest priority, needs ~13 refs) |
| FAR | x001 | func_8011EEC0 | 8011EEC0 | 744 | 2 | src/x001/wip/func_8011EEC0.c (gcc-2.7.2-psx) | only loop 2 o+0xa5 addiu scheduled 7 insns early |
| FAR | x010 | func_8011E6B0 | 8011E6B0 | 564 | 54 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
| FAR | x006 | func_8011D8C4 | 8011D8C4 | 540 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | main0 | func_80069ce8 | 80069CE8 | 528 | no wip | gcc-2.7.2-psx default | no wip; ObjAlloc-style game code (2 calls, 0x28 frame), never attempted |
| FAR | x006 | func_8011D5F4 | 8011D5F4 | 492 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x010 | func_8011E1A4 | 8011E1A4 | 436 | 54 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
| FAR | x010 | func_8011E990 | 8011E990 | 344 | 236 | src/x010/wip/func_8011E990.c (gcc-2.7.2-psx) | piece of 1544 B (with EAE8); abs var s needs s0, a/dx/dy regs differ |
| FAR | x006 | func_8011D4E0 | 8011D4E0 | 276 | no wip | gcc-2.7.2-psx default | no wip; D4E0..DBF4 is one 1812 B sprite-draw function (5 csv pieces), untouched |
| FAR | x006 | func_8011DAE0 | 8011DAE0 | 276 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x006 | func_8011D7E0 | 8011D7E0 | 228 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x010 | func_8011E000 | 8011E000 | 216 | 54 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B (E000,E0D8,E1A4,E358,E6B0); compare-temp trick applied, residual regalloc |
| FAR | x010 | func_8011E0D8 | 8011E0D8 | 204 | 54 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
