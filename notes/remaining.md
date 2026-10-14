# Remaining unmatched functions (triage)

Source: `build/report.json` (mtime 2026-10-14 09:27; HEAD 6d90603 at 11:27 only touched tools/text/psxemu/dist/notes, no src or asm, so the report is still current: 2524/2588 matched, 64 unmatched; not rerun; since then 10 of the 11 DATA entries were reclassified as data, see below). Wip scores from `tools/ncheck.py --score` (0 = identical). No wip file carries a `// CC` or `// AS gnu` header: every wip compiles with the default gcc-2.7.2-psx (+ maspsx 2.86). A wip that spans several csv pieces repeats the whole-wip score on each piece. Addresses are the function's own, the overlay is the unit. The `kept as asm` WARN lines (`build/baseline_asm.txt`) are all rodata/overlap issues in already matched Psy-Q/stub units; none is one of these 64, so there is no LIB entry.

Categories: DATA = table/padding misread as a function (verified by reading the words), LIB = Psy-Q SDK code, NEAR = game code with score <= 10, FAR = game code with score > 10 or no wip.

## Totals

| Category | Functions | Bytes |
|---|---|---|
| DATA | 0 | 0 |
| LIB | 0 | 0 |
| NEAR | 1 | 396 |
| FAR | 50 | 40072 |
| total | 51 | 40468 |

Reclassified as data (second batch, `DATA` in `tools/units.py`, 884 B): x000 80116258 (96) and 801162B8 (116), x001 8011658C (52; the row continues as code from 801165C0), x003 80116D50 (548; code from 80116F74), x005 and x013 801188FC (20 each), x005 and x008 80117CAC (16 each). Their only jals are dispatcher cases of other areas (second dispatcher: area 7 -> 80116258, 12 -> 801162B8, 6 -> 80116D50, 3 -> 801188FC; first: 17 -> 8011658C, 0 -> 80117CAC) and the MAIN0 pointers are slots of other areas' tables (801162D0/80116DD0/80116E4C: areas 13/9/9; 80116EFC: area 17); the object-type slots (801162D0, 801165A8, 80116EC0) belong to types whose handlers are real code in other overlays. The third batch (x016 8011B170, x018 8011C758) is below. x002 8011E688 (jal'd by the player-step code of its own overlay) was kept at first and is now the fourth batch, below.

Third batch (`DATA` in `tools/units.py`, 1128 B): x016 8011B170 (448, 112 pointer words) and x018 8011C758 (680, 170 pointer words). Proof that their jals cannot run while that overlay is loaded:
- Writers of PLAYER.step 0x47 / 0x48 / 0x25, all units (MAIN0, X000, all overlays): `li` + `sb` to offset 5 / absolute 0x800A603D with branch and delay-slot following, plus `sh`/`sw` forms at offset 4/5: 0x47 only X014 80123598 (`PLAYER.step = 0x47` in func_80122FA0); 0x48 only the shared FUN_8010dd8c (8010DEB8, step 0x29 when wac == 2, sets D_8009D2E8 = PLAYER+0xE4); 0x25 shared func_800EFC8C (800EFEC0) and X009 8011DE34 / 8011E034.
- Data: the only data-driven writer of PLAYER.step is script op 0x97 (FUN_8003a89c, args from op 5). Every op-5 + op-0x97 pair on the disc (raw files and all 816 unpacked GAMs) is in AREA15/*.WSS, with step values 0, 1, 2, 3, 8, 9, 0x20, 0x64 only. The other register-valued MAIN0 writes (scriptOpShowMessageBox, FUN_800339a4, func_800317C0, FUN_8003566c, ...) load constants (2, 3, 4, 0xE, 0x18). Player step++ sites (0 -> 1, 0x62 -> 0x63) never reach these values.
- Area changes: every area mode handler (gameplayMainHandler, areaMode2/4/5/6Handler; modes from D_80077084 are only 1, 2, 4, 5, 6) runs InitSubsystems -> clearPlayerObject (memset PLAYER, 0x178 B) at step 0, after areaTransitionHandler / loadCurrentArea set step 0. So a step 0x47 set in AREA14 cannot survive into AREA16 or AREA18, and step 0x47 (hence 0x47 -> 0x48) never happens there.
- x018 kind 0xE: D_8009D2E8 is only ever written with PLAYER+0xE4 (the bitten object). AREA18 has no object of type 0xE: spawn records (`tools/objects.py`, all layers: types 3, 7, 26, 77, 94), section objects (PTR_8007bee4[18]: types 1, 2, 4), no literal type store of 0xE in MAIN0 / shared code / X018 (only X001 801189D8), ObjSpawn / spawnLayer3Object calls use 0..8 / 6, X018's own spawner uses 0x32, weapon objects use D_8009D2B2 (0, 5..9); func_80020D98 and spawnUnlayeredObject* are not called from MAIN0, shared code or X018.

Fourth batch (`DATA` in `tools/units.py`, 40 B): x002 8011E688 (10 words of s16 offset pairs, anim data pointed to from 8011EFDC; X002 func_80100788 step 0x25 jals it at 801011BC). Proof that step 0x25 never happens while X002 is loaded:
- AREA02 collision: WHM blocks (load kind 0x70, `SYS/LD{0..8}AR02.BIN`, all identical across the 9 LD sets): list 2 (sub-piece, 2628 B), lists 3/4 (D205/D306, 968 B), list 5 (1708 B), list 6 (1612 B). Decoded as `getCollisionPlaneAt` reads them (u16 row offsets at +8, per row u16 column offsets, per column s16 n + n x {flags, y, h, info}), list 2 has wall entries `flags & 0x4004 == 0x4004` / `0x4008` (info 0x0104 / 0x0107, h -72 / -24 / -12), which `FUN_800416b8` turns into class 1 more than 8 px below the top. So the terrain probe CAN set b9e = 6 and step 13 (climbing) does happen in AREA02: the collision data alone does not rule step 0x25 out.
- But `func_800EFC8C(o, f)` takes f = (D_8009F0EC == 1), D_8009F0EC = the probe result `func_8004BBC0` = D_1F8003C0. The terrain branch zeroes D_1F8003C0, sets b9e 5/6 and returns 1, so a probe that sets b9e = 6 always gives f = 1 -> step 0xD, never 0x25. Object callbacks of the probe only set b9e 1, 3, 10, 11, 12 (MAIN0 stores at 80042964..8004BA78; the X002-range callbacks have no b9e store).
- Writers of b9e = 6 (every `sb/sh/sw` at offset 0x9C/0x9E in MAIN0 and X002 disassembly): only MAIN0 8004BC70 (the probe) and X002 80109A00 (FUN_801099b8 step 13, state 0). No raw pointer to 8004BBC0, 801099B8 or 800EFC8C exists in MAIN0/X002 data.
- All six probe callers in X002 call func_800EFC8C right after it: four only when the result != 0 (800F355C, 80104C40, 80106430, 80107E34; then b9e was 0 before the probe, so b9e = 6 means result 1). The other two, func_800F67BC (step 9, 800F6B80) and FUN_8010db0c (step 0x43, 8010DCC0), call it whenever b9e != 0, so f = 0 with b9e = 6 needs b9e == 6 on entry to step 9 / 0x43 at state != 0 (both handlers clear b9e at state 0, before the probe). Step 13 leaves b9e = 6 only on exit to step 2 / state 3, whose handler (FUN_800f2b98) clears b9e first. Every write of step 9 / 0x43 (X002 800F66BC, 8010D7D4, the generic setters 800EE9A8 / 800F0AFC, MAIN0 80030C2C) also writes state 0; the remaining register-valued step writes are object init/copy code or script op 0x97 (AREA15 only). Player step++ sites never produce 9 / 0x43.
- Other step 0x25 writers: only func_800EFC8C (800EFEC0) in X002/MAIN0 (X009 8011DE34 / 8011E034 are not loaded in AREA02).

## Table

| Cat | Unit | Function | Address | Size | Best score | Wip / compiler | Main obstacle |
|---|---|---|---|---|---|---|---|
| FAR | x010 | func_80120C88 | 80120C88 | 824 | 16 | src/x010/wip/func_80120C88.c (gcc-2.7.2-psx) | box0/box1 load order, permuter already ran 40k iterations |
| FAR | x006 | func_8011D8C4 | 8011D8C4 | 540 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | main0 | func_80069ce8 | 80069CE8 | 528 | no wip | gcc-2.7.2-psx default | no wip; ObjAlloc-style game code (2 calls, 0x28 frame), never attempted |
| FAR | x006 | func_8011D5F4 | 8011D5F4 | 492 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x006 | func_8011D4E0 | 8011D4E0 | 276 | no wip | gcc-2.7.2-psx default | no wip; D4E0..DBF4 is one 1812 B sprite-draw function (5 csv pieces), untouched |
| FAR | x006 | func_8011DAE0 | 8011DAE0 | 276 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
| FAR | x006 | func_8011D7E0 | 8011D7E0 | 228 | no wip | gcc-2.7.2-psx default | no wip; piece of 1812 B D4E0 function |
