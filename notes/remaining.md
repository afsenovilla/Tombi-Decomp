# Remaining unmatched functions (triage)

Source: `build/report.json` (mtime 2026-10-14 09:27; HEAD 6d90603 at 11:27 only touched tools/text/psxemu/dist/notes, no src or asm, so the report is still current: 2524/2588 matched, 64 unmatched; not rerun; since then 8 of the 11 DATA entries were reclassified as data, see below). Wip scores from `tools/ncheck.py --score` (0 = identical). No wip file carries a `// CC` or `// AS gnu` header: every wip compiles with the default gcc-2.7.2-psx (+ maspsx 2.86). A wip that spans several csv pieces repeats the whole-wip score on each piece. Addresses are the function's own, the overlay is the unit. The `kept as asm` WARN lines (`build/baseline_asm.txt`) are all rodata/overlap issues in already matched Psy-Q/stub units; none is one of these 64, so there is no LIB entry.

Categories: DATA = table/padding misread as a function (verified by reading the words), LIB = Psy-Q SDK code, NEAR = game code with score <= 10, FAR = game code with score > 10 or no wip.

## Totals

| Category | Functions | Bytes |
|---|---|---|
| DATA | 3 | 1168 |
| LIB | 0 | 0 |
| NEAR | 1 | 396 |
| FAR | 51 | 42676 |
| total | 55 | 44240 |

Reclassified as data (second batch, `DATA` in `tools/units.py`, 884 B): x000 80116258 (96) and 801162B8 (116), x001 8011658C (52; the row continues as code from 801165C0), x003 80116D50 (548; code from 80116F74), x005 and x013 801188FC (20 each), x005 and x008 80117CAC (16 each). Their only jals are dispatcher cases of other areas (second dispatcher: area 7 -> 80116258, 12 -> 801162B8, 6 -> 80116D50, 3 -> 801188FC; first: 17 -> 8011658C, 0 -> 80117CAC) and the MAIN0 pointers are slots of other areas' tables (801162D0/80116DD0/80116E4C: areas 13/9/9; 80116EFC: area 17); the object-type slots (801162D0, 801165A8, 80116EC0) belong to types whose handlers are real code in other overlays. The three KEPT rows below are called by the player-step code of the overlay that holds them, so they stay (and the earlier "no refs" note for x002 8011E688 was wrong).

## Table

| Cat | Unit | Function | Address | Size | Best score | Wip / compiler | Main obstacle |
|---|---|---|---|---|---|---|---|
| DATA | x018 | func_8011C758 | 8011C758 | 680 | n/a | - | KEPT: pointer table (170 words) but FUN_80106a18 (shared player step 0x48, present in X018 at 80106AE8) jals it when the held/hit object kind is 0xE, so it can run while X018 is loaded; x006 8011C758 is real code |
| DATA | x016 | func_8011B170 | 8011B170 | 448 | n/a | - | KEPT: pointer table (112 words) but func_80100788 (shared player step 0x47) jals it from X016 itself (80101634); step 0x47 is only set by X014 code, not provable dead |
| DATA | x002 | func_8011E688 | 8011E688 | 40 | n/a | - | KEPT: 10 words of s16 offset pairs (anim data, pointed to from 8011EFDC) but func_80100788 step 0x25 jals it from X002 itself (801011BC); not provable dead |
| FAR | main0 | func_80056420 | 80056420 | 2604 | 27 | src/wip/drawBalloonFrame.c (gcc-2.7.2-psx) | drawBalloonFrame; o pinned $17, o->f via $5 int <<16; left: x+w temp reg, uv sched |
| FAR | x001 | func_80122F54 | 80122F54 | 2192 | 281 | src/x001/wip/func_80122F54.c (gcc-2.7.2-psx) | TR temps fix arg order, BLKB cross-jumps, v0/a0/a1 pins at the 8012767C call; left: s1 var in s5, in-place v1 temps (s1-shape variant in header: 373) |
| FAR | x001 | func_801244A0 | 801244A0 | 1800 | 14 | src/x001/wip/func_801244A0.c (gcc-2.7.2-psx) | SPAD.b1d2 block fixed (t bump + w=v global); left: v a1 vs game v1 in the a->wb0 block; w must be live across wb0 else-arm (fake copy gives 4) |
| FAR | x001 | func_80127074 | 80127074 | 1544 | 28 | src/x001/wip/func_80127074.c (gcc-2.7.2-psx) | case 0 dispatch fixed (full if/else case 0 first), plain params, e bump, short cx/cy; left: cx/fx s5/s7 swap |
| FAR | x010 | func_8011EAE8 | 8011EAE8 | 1200 | 28 | src/x010/wip/func_8011E990.c (gcc-2.7.2-psx) | piece of 1544 B wip (see 8011E990) |
| FAR | main0 | func_800402ec | 800402EC | 880 | 50 | src/wip/func_800402EC.c (gcc-2.7.2-psx) | b/xm share $9, x&7 via $2 temp, h $3; left: sx src/sched, branch-1 fold + cross-jump, delay-slot move, loop-end n load |
| FAR | x010 | func_8011E358 | 8011E358 | 856 | 14 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
| FAR | x010 | func_80120C88 | 80120C88 | 824 | 16 | src/x010/wip/func_80120C88.c (gcc-2.7.2-psx) | box0/box1 load order, permuter already ran 40k iterations |
| FAR | x010 | func_8011E6B0 | 8011E6B0 | 564 | 14 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
| FAR | x006 | func_8011D8C4 | 8011D8C4 | 540 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | main0 | func_80069ce8 | 80069CE8 | 528 | no wip | gcc-2.7.2-psx default | no wip; ObjAlloc-style game code (2 calls, 0x28 frame), never attempted |
| FAR | x006 | func_8011D5F4 | 8011D5F4 | 492 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x010 | func_8011E1A4 | 8011E1A4 | 436 | 14 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
| FAR | x010 | func_8011E990 | 8011E990 | 344 | 28 | src/x010/wip/func_8011E990.c (gcc-2.7.2-psx) | piece of 1544 B (with EAE8); same shape as x001 func_80127074 wip (28); left: cx/s7 s5/s7 swap |
| FAR | x006 | func_8011D4E0 | 8011D4E0 | 276 | no wip | gcc-2.7.2-psx default | no wip; D4E0..DBF4 is one 1812 B sprite-draw function (5 csv pieces), untouched |
| FAR | x006 | func_8011DAE0 | 8011DAE0 | 276 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x006 | func_8011D7E0 | 8011D7E0 | 228 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x010 | func_8011E000 | 8011E000 | 216 | 14 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B (E000,E0D8,E1A4,E358,E6B0); +0x678 fixed (plain velX store); left: same wb0/w regalloc as x001 801244A0 |
| FAR | x010 | func_8011E0D8 | 8011E0D8 | 204 | 14 | src/x010/wip/func_8011E000.c (gcc-2.7.2-psx) | piece of 2276 B wip (see 8011E000) |
