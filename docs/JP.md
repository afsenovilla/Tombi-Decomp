# Japanese version (Ore! Tomba)

The Japanese release is a second decomp.dev version built from the shared `src/` tree (see "How the versions
share code" in the README): `--version slps01144` (alias `jp`), macro `VERSION_SLPS_01144`.

> **100 % matched on 2026-11-09**: every function of the retail disc, SYSTEM overlays and Psy-Q library included.

## Inputs

- Your own disc image, provided to CI through the `GAME_FILES_URL_JP` secret (raw `.bin`, 2352-byte sectors) and
  locally in `game_jp/` (ignored by git). Never commit the image or files extracted from it.
- `.github/workflows/report.yml`, job `report-jp`: fetches and extracts the image, names the artifact after the boot
  executable in `SYSTEM.CNF` (`<serial>_report`), and skips the report until `config/slps01144/` exists.

## Steps

1. Extract the disc (`tools/isoext.py`), record the SHA-1 of every executable and overlay, map the file layout
   (the Japanese release has one language, so no `EXE/MAIN0..8` per language) in `notes/jp/`.
2. Split the executables with splat with `config/slps01144/` (addresses differ from PAL).
3. Auto-match: hash every Japanese function with relocation fields masked and compare against the matched PAL
   functions; identical ones reuse the PAL source (generated, like `tools/areas.py twins`).
4. Decompile what differs (text/font system with kana/kanji, file loading, older library revisions) as
   `#if defined(VERSION_SLPS_01144)` branches of the shared files (`tools/version_share.py fold`).
5. `tools/build_report.py --version slps01144` writes `build/slps01144/report.json`; `tools/build_full.py` gains a JP target
   that must rebuild the Japanese executables byte-identically.

## Out of scope for now

Adapting the text and graphics editors to Japanese text.

## Status

Retail disc: boot `SLPS_011.44` (sha1 `07d577698ad31ec1a69b9d7b80a7c3f90b650888`), entry 0x80065D80, 0x8E000 bytes at
0x80010000. Same file layout as PAL with two-digit names: `SYSTEM/X00.BIN`..`X19.BIN` (copies in `AREAnn/`; AREA07
uses X01 and AREA12 uses X04, AREA19 also ships X02), plus `INFO*.BIN`, `DSPSUB*.BIN`, `OPTSUB00.BIN`.

- **Overlays** load at 0x800E9AF8 (PAL 0x800E8028), from jal targets on prologues. Every `X<nn>.BIN` starts with
  the same 0x2B680 bytes as `X00.BIN` (the shared code, PAL X000's role); the area part follows at 0x2B680 (vram
  0x80115178) with its own ID word (X01 = 9) and entry table. `config/slps01144/x<nn>.yaml` splits only that area part
  (the prefix is a `bin` segment), so shared code is counted once, in `x00`.
- **Exe text** 0x80015D1C (`main`) .. 0x8007142C (end of `SpuGetAllKeysStatus`), like `config/main0.yaml`.
- `config/slps01144/symbol_addrs_*.txt`: PAL names at JP addresses, from `tools/xmatch.py --csv` +
  `tools/jpdemo_syms.py --version slps01144`; functions without a PAL name are `func_<addr>_slps01144`. Masked match
  finds 1059 exe functions (40 % of the bytes) and 348 in X00.
- `config/slps01144/reuse.txt` (`tools/jpdemo_reuse.py --version slps01144`): JP functions that PAL C builds as is.
- Differing functions: `#if defined(VERSION_SLPS_01144)` branches in the PAL files, with a
  `// VERSION SLPS_01144 <prog> <addr> <size>` line (77 with JP C, 86 with `INCLUDE_ASM_V` for the paired functions
  not matched yet), and 4 JP-only files `src/[x0nn/]<name>_slps01144.c`. They were the 81 files of the former
  `src/jp/`, folded in by `tools/version_share.py fold`; `tools/version_share.py asm slps01144` writes the
  INCLUDE_ASM branches from a report.
- **SYSTEM code overlays** `DSPSUB.BIN`, `DSPSUB2`, `DSPSUB3`, `INFO`, `INFO2`, `INFO3`, `OPTSUB00` (no GOVER on the
  JP disc): programs `dspsub` .. `optsub00` (`JP_SYSOVL` in `tools/versions.py`, paired with PAL `d0spsub` ..
  `o0ptsub0` by `pal_prog`), raw blobs at 0x800E9AF8. `config/slps01144/<prog>.yaml` splits header (bin), code,
  trailing data (bin); code ranges DSPSUB 0x7C4-0x94B8, DSPSUB2 0x83C-0x4770 (string table first, no entry table),
  DSPSUB3 0x70C-0x6330, INFO 0x1B4-0x6264, INFO2 0xAB0-0x1621C, INFO3 0x414-0x679C, OPTSUB00 0x4-0xD84 (no entry
  table; much smaller than PAL O0PTSUB0). Symbols: `python3 tools/gen_symbols.py`, then `python3 -I tools/xmatch.py
  game_jp/SYSTEM/{DSPSUB,DSPSUB2,DSPSUB3,INFO,INFO2,INFO3,OPTSUB00}.BIN --csv build/slps01144/xmatch_sys.csv` and
  `python3 -I tools/jpdemo_syms.py build/slps01144/xmatch_sys.csv --version slps01144`.
- `python3 tools/build_report.py --version slps01144` writes `build/slps01144/report.json`; CI job `report-jp`
  uploads it as `SLPS_011.44_report` from the `GAME_FILES_URL_JP` secret.

**Progress: 100 % of code (1631936 of 1631936 bytes, 3528 of 3528 functions; README table)** with the SYSTEM overlays (2026-11-09), mostly PAL C reused as is, the rest from
the JP branches (PAL C with one to four literal constants changed and verified byte for byte, e.g. player struct
offsets 0xE4 -> 0xD4, object stride 160 -> 152, sound pitch constants).

Automatic pass (2026-10): for every unmatched JP function paired with a PAL function that has C (symbol table `PAL`
comments, or the most similar same-size PAL C function at 80 % or more masked words), the PAL C is compiled for JP and
compared word by word with the target (relocations masked, relocation positions compared); numeric literals of the
PAL C (struct pad sizes, raw offsets, constants) are then shifted by the deltas seen in differing immediates, greedily,
until the object is identical, and the result is written as the `VERSION_SLPS_01144` branch. When the unchanged PAL
C already matches, the INCLUDE_ASM branch is dropped instead. Two tool fixes came with it: the version targets
assemble `D_1F80xxxx` scratchpad symbols as constants (build_report, like the C side), and
`config/slpm80165/spad.txt` was regenerated (union with the old table) so the PAL scratchpad names such as
`VIEW_CAMERA` compile to constants. Gain: 2111 -> 2183 functions (50.7 % -> 53.8 % of code).

Near-miss pass (2026-10, 2183 -> 2225 functions, 53.8 % -> 55.2 %):
- task table: `config/slps01144/symbol_addrs_manual.txt` keeps `D_801FD800..810` unsymbolized (like the demo), so
  `startTask`, `closeTask`, `ThreadCreate` build from the PAL C (literal addresses) with no JP branch;
- `build_report` gives a multi-function PAL file's first symbol the whole `.text` when the version target is one
  function of the same total size (`chainedPartFrameTo80.c`, `chainedPartPlaceChild.c`, PAL C as is);
- objdiff ignores relocation addends, so a field reached through a splat symbol (`D_8009EE6B` for `GV2.bc3`) needs
  no change; `setPlayerAxisPointers` builds from PAL C;
- 35 JP branches with instruction-level edits, found by pairing every unmatched JP function with the most similar
  PAL function that has C (masked words) and diffing the PAL C compiled for JP: pad bits 0x10..0x80 -> 0x1000..0x8000
  (and the reverse in `updateMotocrossRaceStarter`), camera Y offset 0x800000 -> 0x780000, player flag +0xe3 -> +0xd3
  with the `G934 = 0` store moved first, player anim ids, a dropped call or store, the progress check
  `GV2.bc3 & 2` -> `D_8009E50A >= 4`. Not doable in a shared file: one PAL function whose JP copies differ between
  overlays (e.g. `initJuiceSeller` x16 matches PAL, x18 does not): needs a JP-only file.

Near-miss pass 2 (2026-11, 2386 -> 2392 functions, 61.63 % -> 61.87 %): the six partially matching units. `phoenixGossipLavaHintB`
has two JP copies that differ (X03 = PAL code via `reuse.txt`, X04 = other message id, now `src/x004/phoenixGossipLavaHintB_slps01144.c`);
`build_report` reassembles the targets of the units that claim data (jump tables) without the scratchpad `.set` constants
(`func_80057B74`, fixed); the four JP-only handlers of X02/X16/X17/X18 were matched by borrowing the idioms of their PAL
near-twins (`evilPigBagPortalEnter.c`, `a17EntranceEventStep.c`): struct members (`GVARS.b39`, `GAME.fadeScreenControl`,
`VIEW_CAMERA.pos[].p.hi`) and `D_xxxA[0]` array spellings decide what gcc 2.7's scheduler may reorder (scalar globals move
freely around struct-member stores), an inlined helper returning `unsigned char` keeps the `move v0,a0` before the test,
and per-branch `o->step = n` stores are cross-jumped into one. `tools/jpcheck.py` strips branch targets, so a wrong branch
destination (X18) only shows in the objdiff report.

Autofix pass (2026-11, `tools/jp_autofix.py`): for every unmatched JP function, the closest PAL C function by masked
words (or its symbol-table pair) is compiled for JP and diffed against the target with `tools/jpcheck.py --norm`, which
now ignores relocation addends and PC16 relocs like objdiff and assembles the PAL scratchpad names as constants. Edits
are then tried one at a time and kept when the diff shrinks: an immediate of the diff replaced in the C literals (and
any literal shifted by the same delta: struct pads, offsets written as differences), pad masks `0x10..0xf0 ->
0x1000..0xf000`, one statement moved up to three statements away, one statement deleted (vibration, shock flags, the
player ext bytes 0xa0.., load lists: things the JP build lacks), in both orders. A match is installed as the
`VERSION_SLPS_01144` branch of the shared file; `--emit DIR` writes the best attempt of the rest for hand edits and
`--install-file` checks and installs them. The hand edits so far were of the same kinds plus: anim id switch labels
(+14, +50), `o->step = n` per branch, a `(x & 0x40)` test instead of `== 0xff`, calls merged (`loadSoundSet(5)` for
`loadSoundSet(3); queueSystemLoadList(5, 1)`), statement order (gcc 2.7 reorders scalar globals around struct members,
so a scratchpad short spelled `((short*)&SPAD.iec + 1)[0]` or `D_xxxA[0]` stays in place while `D_1F8000EE` moves).

Result of the autofix pass plus hand edits (2026-11): 2392 -> 2486 functions, 61.87 % -> 66.96 %. The full
`--install` run over the candidates at 0.8 or more takes hours (the generic edits compile up to 200 variants per
function, in both orders) and matched 6 of 131 by itself; the hand edits from its `--emit` diffs did the rest, about
one to five minutes each. `tools/jpcheck.py --strict` keeps the branch targets (a wrong branch destination otherwise
passes as IDENTICAL, as happened twice) and is what `--install`/`--install-file` verify with; relocated branch and
jump targets (labels of the target asm) are ignored on both sides.

Merged symbols (2026-11): 44 JP "functions" held a second function body after a `jr ra` (Ghidra missed the entry):
split in `config/slps01144/symbol_addrs_*.txt` (`func_<addr>_slps01144 ... split from <parent>`), which gives 3151 functions
(was 3101) and better PAL pairs for both halves; two "functions" of the old table were only the dead epilogue of a
`for (;;)` task (`gameMainTask`) or a tail jumped into from elsewhere (`updateA00UnlayeredType0D`), merged back. The
scan (commits 8dd088d, 7a64b01): any instruction but `nop` following a `jr $ra` and its delay slot with no label in
between. The remaining partial unit, `func_80030A68_slps01144` (76 %), reuses the PAL `playerEffect4EStepD` as is.

Hand pass 3 (2026-11, 2533 -> 2901 functions, 68.66 % -> 92.25 %): the PAL C renamed for JP and diffed with
`tools/jpcheck.py --norm` (seconds per function, no autofix run), then edited by hand; `--strict` before every
install, since `--norm` masks branch targets and twice called a wrong control flow IDENTICAL (`bootPlayIntroMovie`:
the JP case numbers differ, `drawCategory3Objects`: the retail jump table). New JP differences seen: the sprite drawers
(`drawObjectSprite`, `Scaled`, `RotScaled`, `Rotated`, `PartsRotated3D`) drop the GTE flag test after RTPS and clip at
`< 0xf1` / `< 0x141` (PAL `0x100` / `0x140`); `drawCategory3Objects` knows render kinds 0..18 only; `initDisplay` has an
NTSC twin (`func_80016244`, 320x224, screen y 8) that `bootPlayIntroMovie` calls; JP checks the event flags
`GAME.area00_eventControl & 0x20 / 0x40` where PAL reads `D_8009D0A7` / `D_8009CE4A == 0xff` (`charlesTalkCheck`,
`updateA00Tornado`); `collidePlayerWithCat5Objects` inlines the per-object test; `drawHudVitality` reads the level from
the sprite record (+0x52) instead of a parameter; `queueAreaSectionLists` has two list tables and starts the CD load
thread itself; a few functions are the demo (`SLPM_80165`) version byte for byte and share its branch
(`initHud`, `updateA00Tornado`: `#if defined(VERSION_SLPM_80165) || defined(VERSION_SLPS_01144)`); the player step
functions take the same rewrites every time (ext bytes 0xe0..0xe4 -> 0xd0..0xd4, `playerSetFallLean(o)` -> the inline
bitten-jump tail, no `GV2.bb0` / vibration stores; a landing block split into `b69 == 1` / tile-collide branches that
differ only by the `b0f = -8` store, the `BAC = 0` store before `AnimJump`; `updatePlayer` copies 74 anim entries and
reads the pad halfword through the pinned `D_8009F200`); split pairs
(`func_80135A20` = the event-flag stub, `func_80135A2C_slps01144` = the spawn body) get one JP-only file each;
`COS_TABLE` is now named in the JP symbol table (`NEG_SIN_TABLE + 0x200`). A `jal`-target scan inside symbol ranges
found nothing but data words, so no more merged functions that way.

Later in the same pass (72.6 % -> 75.04 %): the big player steps (`playerStepIdle`, `Walk`, `UseItem`, `Jump`,
`HurtKnockback`, `HazardRespawn`, `playerHandleHazardContact`) match once their JP edits are applied: no `substep`
stores, no `cd`/`ce` bytes, no `GAME_PLAYER_MODE` resets, `GVARS.i34 = 0` before `wac = 0`, the landing code inlined
in `playerStepJump` (no `playerLandFromJump` call), `getHazardDamage` case 1 ends with `r = 0` for area 0x3000a,
pad masks byte-swapped, ext fields -0x10 and the 74-entry anim table; `cdLoadTask` seeks with `CdControlF(CdlSeekL)`
and reads through `CdReadyCallback` + `CdlReadN` (no `CdRead`/`CdReadSync`, no `DrawSync` state), with the WVD head
and body tables swapped; `updateHud` uses `DAT_8009c990[0x40c + i]` where PAL has `[0x40f]`; `drawHud` and
`updateHud` need the default `-O2 -G0` (strength-reduced pointer walks), as do `cdLoadTask`, `drawMessageBox` and
`getHazardDamage`. Compiler idioms that decided matches: a struct-member store does not block a scalar load from
hoisting, so a global that must be read after such a store is spelled through an array alias (`D_xxxxA[0]`,
`PLAYER_MESSAGE_BOXA[0]`); a scratchpad address used twice in one block is kept in a register by cse unless it is
spelled as the `D_1F80xxxx` symbol (`cdLoadTask`, `MovieDecodeFrame`); two locals of equal priority take saved
registers in declaration order (`cdLoadTask`: the pointers declared before `nhead`/`nbody`); `f - (f & 0x8000)`
counts one more reference than `f & 0x7fff` before combine and moves `f` up the allocation order (`drawMessageBox`);
`if (c) break;` before a `case` label keeps gcc from merging the case tree (`playerStepHurtKnockback`); a hard-register
local (`register ... asm`) pins a saved register when a parameter and a local take each other's `s` register
(`openMessageBox`: `anim` in `$18`, `MovieDecodeFrame`: `dec` in `$17`), with an `asm("")` so the pinned value is
not computed in place.

The weapon code (75.04 % -> 76.48 %): `weapon0Swing`, `weapon5to7Swing`, `weapon8to9Swing` and `drawWeaponObject`
match with JP edits. The trail-sprite spawns (`spawnWeaponTrailObjects`, `spawnWeaponHitDebris`) are gone and
`func_800EF1C8(flag, x, y, z)` is called instead: once with 1 when the weapon starts its return and once with 0
after the swing/return cases (in `weapon5to7Swing` after cases 0-3 and 6, which gcc cross-jumps into one call
block, so the call sits after the `if` in each case, not in each branch). `weapon8to9Swing` calls `AnimAdvance`
in the cancel branch and no longer mirrors `waa` into `d8c`; a store to `PLAYER.b9d` next to a call taking
`&PLAYER` is spelled through the JP symbol (`D_800A7C15`) or gcc keeps `&PLAYER` in `a0` for both.
`drawWeaponObject` switches on the JP anim ids (bases 180/183/186, 37/35/33 cases: `0xb4`, `0xbd`, `0xc0`,
`0xcc-0xce`, `0xd5`, `0xd8`, ...) with the `>= 0xdb` fall-back, and projects without the GTE flag test.
`l4Type16ObjectStep` (x01) keeps the per-segment flag inside the 52-byte segment record (offset 0x22, fields +12)
instead of the object's flag bytes and needs the default `-O2 -G0`.

Overlay twins paired by callee overlap (76.48 % -> 78.36 %): `pigGateDiscoveryScene` (x02, 7.3 KB: the orbit
advances by 1 and the sprites sit at `+0x90`, not `-(... + 0x60)`), `pigGateSevenFriendsScene` (no `GCD.bcfdb`
store), `spawnPigGateSceneNpcs` (every group lowers by 0x12c), `treasureChestGuardTalk` (player reset
unconditional), `purpleCrawlerHit` (no `cloneItemPickupObject`), `komaPigLeap` (anim ids -7, 60-frame timers,
`active = 1`), `stepPuppyBaron` (no `D_800A609C` branch), `motocrossBikeJumpStepB` (pad mask), `animateArea9Palettes`
(0x104 record, columns -0x50), `playerStepRideSeesaw` (x01: pad masks, ext fields -0x10, `playerSetFallLean` inlined
with `a = 16` hoisted, no `GV2.bb0` store), `updateChainSegmentEnemy` (`b6b != 0xff`, no anim-frame stash),
`updateA00L2Type2C` (`b9d & 2`, no `b9d = 0`, one exit sets `state = 0; substep = 6`) and the x00 copy of
`updateHoodedOldMan` (no health branch, no `GAME_PLAYER_MODE == 3` tests, `BUTTON_MASK_C`, no `PLAYER_STATE = 5`
resets, `GCD.bcdad = 0xff` instead of the health refill). tools/jp_autofix.py --pair on the mid-size pairs adds
the rest.

Next (78.36 % -> 79.20 %): `updateMotocrossBike` (no spawn-point branch), both `playerStepClingRotating` copies (pad
masks, no anim-id switch, no timer/anim resets in the grab blocks, `PlayerSetAnimIfChanged(o, 0x25)` before the pad
test, `ba4 = 0`, the release block with struct-member accesses so the `PLAYER_EXT` load hoists), `rewardItem`
(`setEventComplete(EVENT_INEEDABOMB, 1)`, cross-jumped with the tear-jar case), `segmentedCrawlerContact`
(`o + 0xd4 = e` before the impact effect), `stepSegmentedCrawlerHurt` (no tile probe, the d94 chain and the counter
decrement in case 2, unconditional in case 5) and the tools/jp_autofix.py --pair matches `raceResult`,
`func_80122404`, `playerStepRiseToLedge`. Left: `updateBlackCrow` (init block scheduling), `updateL8Type01Object`
(JP keeps `b04` in `s1` and reuses it for the constants 1 and 2 after the equality branches).

Small leftovers of the pairing (79.20 % -> 79.52 %): `updateBendingBranch` (no `+ 0x10`), `bucketFillWithWater`
(no `case 0xe`), `purpleCrawlerContact` (`o + 0xd4 = e` before `wac = 2`), `updateSpringVine` (`w1e = 8`,
`w76 = D_8013AA64[b0c]`, subtype 1 sets `b0f = 6`, `box0 = box1 = 4`), `updateSegmentedCrawler`
(`ObjCullRegister` before the `b0c` test, no margin cull, case 2 calls `stepSegmentedCrawlerHurt`),
`playerStepSwimSpin` (the `velY < 0` path repeats `playerSetAnim(o, 0x44, 0)` and only sets `step`/`state`),
`playerStepContactStop` (pad mask) and `gameMainTask` (no `initLighting`, no `D_1F8001FC = 0`).
Then (79.52 % -> 79.66 %): `updatePuppyRumorNpc` (no `GAME_PLAYER_MODE == 1`, `BUTTON_MASK_C`),
`chainSegmentEnemyContact` (`o + 0xd4 = p`), `updateChainObject` (case 1 is a three-way switch on the section:
swing, draw links, or `cullObjectToDrawListMargin(o, 64)`), `raceStartCountdown` (case 1 nudges `PLAYER.h` by
0x10000; the message box is stored through `&PLAYER + 0x90` so gcc derives `&PLAYER` from it) and
`komaPigDefeated` (x01: case 0 sets movetab/velV/b0a/animFrame/anim/b0f and marks the d90/d94 chains; the anim
pointer is read into a local right after `state++`, before the stores).

Data counted as code (79.66 % -> 81.16 %): tools/jp_data_units.py now keys each row on the splat file's first
item (the `.byte`/`.short` lines before the first word made build_report miss `x02 8011B910`, `x05 80116998`,
`x11 801188AC`, `x17 80118314`, `x18 801194AC` and `x00 80136940`: pointer/record tables with no instruction in
them) and splits a table-then-code unit at its first prologue when nine words in ten before it are pointers or small
constants: the X00 head (`800E9AF8`, 7196 bytes of entry and jump tables) is data and its 384-byte tail of dispatch
stubs is the new `func_800EB714`. The code total drops from 1445372 to 1418744 bytes (3096 functions), the same
treatment the PAL build gives its pointer tables in tools/units.py DATA.

Scheduling leftovers (81.16 % -> 81.27 %): `updateBlackCrow` matches once its init block is written as the retail
memory order with temporaries (`t = o->b04` and `sp = SPAD.i2dc` read first, `hp = o->h` after the `b0f` store,
`yy = o->y` after `b04 = t + 1`, `hv = hp->p` and `dp = o->d` before the `wc6`/`wc4` stores, `dv = dp->p` before
`wca`/`d64`, `wc8 = dv` last): with every access a struct member nothing may pass anything, so the source order is
the instruction order. Tried and left: `initGameConfig` (the `BUTTON_CONFIG_INDEX` and `D_1F8001B3` loads sit after
the `saveSlot` store whatever the source order, locals, volatile, scalar or struct accesses, or scheduler flags;
JP also has `D_1F8000F2 = -120`, `saveSlot = 3` and `D_1F8001C8 = 0; D_1F800200 = 69` at the end),
`updateL8Type01Object` (JP keeps `b04` in `s1` and reuses it for the constants 1 and 2 after the equality branches,
while reloading `b04` for the increment), `updateParentFollowModel` (two induction pointers in the record loop) and
`drawBalloonFrame` (tail x/y in `t0`/`a3`).

Masked-word pairing at a low threshold (jp_autofix.candidates(0.45), 81.27 % -> 81.31 %): `inventoryScreenHandler`
(the 99.9 % unit, now exact), the x00 copy of `updateA00L2Type2C` (same edits as x04), `rewardGoldenBowl` and
`rewardHeart` (no `GVARS_PLAYER_HURT_STATE` guard, like the demo), `cameraModeHandler6` (`< 4` only, limits
0x54/0x88). Near: `weaponHitCat2Type08` (JP drops the clone-on-first-hit block; two lines of tail placement left).

Opcode-signature pairing (opcode sequence only, size within 25 %) finds nothing above 0.7 for the ~190 unmatched
mid-size functions: they are new code, not edited copies. The big functions with a twin (81.31 % -> 81.86 %):
`playerHurtLandRecover` (no `bd1 == 1` anim variants, no cd/ce bytes, no landing block in the airborne branch, timer
30, `d8c = 0` before `timer = 0x19`, no `bd1 = 0`) and `playerNormalModeDispatch` (pad masks, no cd/ce, no
`GAME_PLAYER_MODE = 0`, case 3 lands unconditionally, a new case 0x39 = `dirA; func_80101F30; chk`, the 0x63 exit
clears `D_8009EE58`; the `turn()` double store stays).
`updateFrog` (x00 twin, 81.86 % -> 82.04 %): the frog's case 2 just sets the flag, starts event 3 and adds the
item; no `w78` branch; no section-0 condition block; section 5 only tests `PLAYER_H`; the give-back state sets
`GVARS.b3e`, completes event 3 before the spawns, clears `b3e`, sets `b04 = 3` and has no timer state; the angle is
an int masked once and copied (`asm("" : "=r"(ang) : "0"(ang))` keeps the copy), with int trig prototypes.

The 64-256 byte band (82.04 % -> 82.27 %, 18 functions): the eight x14 pig-shot contact handlers are the PAL ones
without the `wac == 2` and `SPAD.b1a4` guards (one, 801230FC, is a table of durations and packed coordinates, now a
data unit: tools/jp_data_units.py takes a return-less unit wedged between two data units); `playerFallIfHeadHit`
inlines `playerSetFallLean` (timer store after the `animFrame & 1` test); `a04ItemTriggerCheck` has no
`displayExpBar` test; `shuttlePlatformGrabProbe` compares `box0 - d` signed and reads the ext fields at -0x10;
`playerInitAreaHook` sets `o->d60 = 1` instead of `GV2.bb0 = 0` and lands in step 0 / state 0; `drawRaceHudIfActive`
tests the whole flag byte like the demo. New JP code written from the disassembly: x00 801106F8 / 80110748 (pad
steer of velX / velY through a `volatile` pointer to the pad word plus a local copy of the argument, which gives the
`lui/addiu/lhu 0(reg)` form and the `move v1,a0`), x01 80138790 / 80138130 (talk triggers: result variable, early
`return 0` before the nested ifs, `SPAD.s1fc & SPAD.s3c8` in that order), x09 8011F470 (a chain of eight calls on
the same two arguments), x03 80118B28 (decor sprite with anim 13, `b0d = 0x80`, `d64 = 0x2000`, AnimAdvance when
culled in). Tried and left: x01 80131240 (216 B): gcc turns its `if (probe()) return 2; return 0;` tails into
`sltu`/`sll` store-flags and cross-jumps the two probe calls, while retail keeps them as branches with a `move
a0,s0` copy in the first one; tails as nested ifs, a result variable, argument copies and -O1 all stay 33-52 lines off.

Main (slps01144) part of the band (82.27 % -> 82.38 %, 10 functions): `loadCurrentArea` without the list file, the
section resources and the sound task start; two libcard inits (80064BEC / 80064C78: critical section, `_patch_card`,
ChangeClearPAD(0), then a 4-argument setup and `D_8009BFE4 = 1`); the script opcode 80038F70 (`scriptOpCompareVar`'s
`rd` and pc layout, writes `o->vars[dst]`); the CD sector callback 8001FD7C (`volatile` d294 only, written as
`x = x - 1`, pointer p290 plain); two TIM uploads (80016ACC = the demo's func_8001619C, 80016B38 only the CLUT);
object init 8002FD60 (`active = 1` written first, anim and d3c loaded into temporaries); 8003E0EC (item 8, event 169)
and the twelve-call chain 80024860. Tried and left: the sound family 8001E2D4 / 8001E388 / 8001E458 / 8001E528
(SfxPlay with `SoundFindFreeVoice`'s first loop inlined: everything matches except that retail puts the sfx id in
s1 and the -1 in s2, ours the other way round; parameter types, -1 placement and an asm copy do not flip it) and
8001E81C (JP-only key-on through SsUtKeyOnV); 80027D58 (`increaseMaxHealth` without the volatile reload: retail
stores the max byte before the `andi`, ours after); the pad readers 80016960 / 80020794 (retail keeps `&D_800A1380`
/ `&D_8009F200` in a register across the function and reloads the other pad word by symbol); the flag bit test/set
800219F0 / 80021A40 (`GAME + 52 + (GAME << 5)` table; retail materialises `&GAME` in a register); the libcard
80064E1C / 80064E84 (frame 16 and a kept branch: probably an older SDK compiler).

Two queue entries (82.38 % -> 82.65 %): `func_80101F30` (x00, 1620 B, the new JP player step called from case 0x39
of `playerNormalModeDispatch`): a landing step written from the disassembly with the PAL steps as idiom reference
(drift add, AnimAdvance, `PLAYER_EXT[8] = 1`, three helpers, anim 4; a bitten (`wac == 2`) exit, a `b69 == 1`
landing and a `probeSidesAndApplyTileResponse(o, 4, 0)` landing, both with the velX damping by 208, the slope table
`D_801145D4[wb0]`, the landing sfx guarded by `GCD.bcf06` / `GCD.bce61`, the bitten-jump-kind switch and the `bbe`
flag dispatch to steps 16/23/27/30/31, then `func_80056C34` / `func_800EFDF8`). Matching needed: the drift words
and the GCD bytes read through their structs (a scalar load gets hoisted above the neighbouring struct store),
and in the `b69` landing the slope byte loaded into a temporary before `step = 1; state = 0`, the pad test into a
variable, the `d8c` store after it and the `if` last (that keeps retail's `j` to the shared `beqz` with the store in
the delay slot). `updateSceneSequence` (x02, 2304 B) matched at the first try as the PAL file without case 21 and
without the max-health compare of case 0, with `func_80040940(q, 0, 0)` for the player and `DAT_1f8001fc &
DAT_1f8003c8` for the pad. `weaponHitCat2Type08` stays two lines off: the structure (case 1 stores first, frame
read last, shared `one - (t & 1)` with `one` kept in s2) is right, only the temporary's register differs (a0 / v0 for
v1).

Three more queue entries (82.65 % -> 83.08 %): `komaPigLeap` x04 twin (1828 B): the JP switches on `state`, not
`substep`; case 0 plays sfx 15, has no timer, sets `b9c = 1`, `wac = 0x29`, `b69 = 0`, `d8c = 0` and `animFrame = 1 -
w7a` (the state increment goes into an int temporary behind an asm barrier before the frame expression, which puts
retail's `lbu` first and keeps `addiu` ahead of the `subu`); cases 1-2 do not call AnimAdvanceWithBox; case 2's
leap sets `active = 3`, `b9c = 1` written first so the shared constant 1 lands in the delay slot; case 5 sets
`active = 1`, timers 360 / 120. Its x03 sibling (80122250) is the same file with wac 0x21 / 0x1b / 0x1a / 0x22-0x24
and a `velV < -0x300` clamp after the `b9c = 1` test of the rebound. `masterOfTheSkiesScene` x03 twin (2432 B): case
1 snaps PLAYER_H to the boss and plays anim 0x14 unconditionally, case 2 only waits and sets anim 0x15, cases 3-4 let
the player fall (`y += velY << 8; velY += 0x10`) until velY > 0 / until 16 above the boss (the y read into a short,
compared, then stored directly to `P.y.p.whole`), case 50 / 82 drop their timers (state stores inside both arms of
the case-82 if so the constant is hoisted) and case 120 takes the 200 timer; `pad[16]` keeps the 64-byte frame.
Left: `playerStepRideSeesaw` x10 (2356 B): the PAL file is register-pinned throughout and the JP reads the seesaw
record at +0x30 as shorts (`lh 48` instead of `lw 140`), a rewrite rather than an edit; `blackCrowFlyAround` (2904 B)
likewise.

Near misses of the earlier sweeps revisited (83.08 % -> 83.34 %): `updateLeafButterfly` (JP case 2 / step 0 does the
event start inline: `b0f = -7`, anim, `d8c = 0`, `category |= 0x80`; step 1-2 start event 178 when GCD bce..57 == 1
and bcfc..5c == 0), `playerStep3FArea10` (ext fields at -0x10, pad mask 0x4000, `if (b9e) break` after `b69 = 0`
and no bitten-jump-kind switch), `updateLookAndSeeViewpoint` (step 0 only tests `b69`; the fade sequence sets the
player to b04 5 / step 98 and `SPAD.b1cd = bce40 ? 8 : 5`; the end resets the player to b04 1 / step 0 / state 0),
`gameplayTick` (no movie-skip block; an inventory-screen reset block when `GAME_INVENTORY_SCREEN == 0xff`, the
screen byte through a barriered pointer, the three shorts and MENU_STATE as `[]` externs so the loads from the
SPAD.p1d4 record stay ahead of the stores; `updateMainObjects` and two JP calls in the object pass) and
`playerStepLaneShiftFall` (no area-10 start, no case 2, `playerSetFallLean` inlined with `d84 = 0` before the
frame test and `timer = 10` after it, no `da0+2 = 0`, the `S16(o, 0xd0)` test).

Scratchpad globals in JP-only C (83.34 % -> 83.38 %): name them `D_1F80xxxx` (upper-case hex): jpcheck and
build_report assemble those as constants, which gives retail's `lui 0x1f80; lw 468(reg)` form; a struct declared
in the file or an address cast keeps the address in a register instead. With that, 8001B808 (loading-screen text:
the SPAD 0x1d4 record reloaded through three differently named pointers, `w66 == 0` tested first) and 80043154 (the
TObj variant of probeFlaggedSegmentA: `w` pinned to $5, `h` to $4, the field index `k` to $2 behind a memory
barrier after the 0x284 store; returns 1 with `ba1 = 2` for field 13, else 0). x10 80119D9C stays at 30 lines
(case 0 store order: retail keeps anim before b0f and d3c before b04, ours reorders them).

Callee-overlap pairing rerun on the current unmatched list (scratch pair.py, score 1.0 and size within 25 %)
and the plain PAL copies diffed strictly (83.38 % -> 84.01 %, 18 functions, each one or two lines): the talk
functions of x04 (singingMinerRumor, mermaidSighting x3, mermaidNecklace, paintingSlotHint, lostTreasure pending)
drop `animFrame = 1` at case 0, set `GVARS.b3e = 1` before the message box, use other message ids (7/8/9/10) and
test only `GCD.bce3d`; `initTunnelDigger` moves the digger 32 left instead of to 0xc4d; `soakedPhoenixGuide
Section1Scene` zeroes the player step (not 100) and falls at -0x300; the x03/x04 `blackCrowContact` twins read the
bite target at -0x10 and have no `SPAD.b1a4` guard, the x03 one only on `r == 2` and only when `b->wb4` is set,
storing `b->w7a = bh < ah` first thing in case 2; `wingedForkPigCheckCollide` has no `y < -0x46a` branch;
`updateFlowerGardenEvent` sets b04 to 3 when nothing spawned (the result copied through an asm barrier for the
`move v0,a0`); `wingedForkPigShotContact` / `burningFireContact` / `phoenixSpiritHintTalk` / `windblownMonkey
RepeatTalk` lose the b1a4 guard, the `d1 = 1` byte, the player-facing frame or get the standard talk-end player
reset; `updateL3Type3BObject` tests only `GCD.bcf06`; the x00 `playerStepSwimSpin` copy keeps two
`playerSetAnim(o, 0x44, 0)` calls (the `velY < 0` exit skips `b69 = 0`); `wingedForkPigHit` leaps 16 and drops the
clone on the landing (`t = hit(o); if (t || velV > 0x580)` keeps a single block). The JP-only files must name
callees by their JP symbol (`func_80020DB4`, `probeSidesAndApplyTileResponse`), the PAL `FUN_` names do not link, and the x00 copies name the
player record `D_8009DEE0` and the tile probe `func_80040940` (the x00 target keeps the raw names).
`lostTreasureBossTalk` x04 twin: no timer after event 0x51 and no case 6. Left: the x10 / x14 copies of
`updateA00FarBackdropWest` (80119D9C with w1e 8, anim [0] and a simplified case 1; 80118580 with w1e 13): retail keeps
the object pointer in a3 and the two screen-origin halves in a1/a2, ours in a1/a2 and a0; pins do not flip it.

Three rewrites from the same pairing (84.06 % -> 84.15 %): `spawnA00Tornado` x00 (only when `GAME + 0x24` bit 0 is
set; sub 0 lowers z by 0x80 and not y; the -1 in a variable shared by both objects, wba/wbc/wbe = 6/2/6, the c0
halves read back right after each store, `k = 6` pinned to $7 behind an asm barrier placed after the anim load so
its `li` precedes the first read); `drawObjectProjectedSpriteB` x00 (the projection result is ignored; subtype 0
gives code 0x2d without the colour bytes, else 0x2c with them; x2 stored before y1; the prim pointer bumped through
the scalar `D_1F800164`); and the x14 `updateA00PoolType11` (JP-only: w1e 1, b0d 0x80, timer 40, velH by frame,
`a.raw += velH << 8` while `GVARS.b42` is clear, cases 2-3 free).

`wingedForkPigContact` x10 twin (484 B, -> 84.19 %): two early exits to the hit block (`e->wb4 == 0` read
unsigned and subtype < 2; or `b9c == 0`, subtype < 2 and `b04 == 1`), the r < 3 bite without `b69 = 0` / `w7a`,
no `SPAD.b1a4` guard, case 2 without `b69 = 0`. Still at a register name each: `wingedForkPigDefeated` x03 (JP: no
step-dependent velV, wac 10, `b0a = 2`, `animFrame &= 1`, the box bytes from `D_80134EAC[wac * 4]`; retail reads the
frame into v0 before the constant 2 lands in v1) and `updateWingedForkPigShot` x03 (JP: anim 0 at init, sfx 115
and `active = 2` on `b69`, back to step 0 on the anim end, the box halves from `D_80134E74 + w2 * 8` of the anim or
of the subtype table; retail keeps w2 in v0 and the table base in v1).

`acquireSpriteSlot` JP branch (692 B) and the x04 JP-only pusher `func_8011BF1C` (560 B, -> 84.28 %). The slot
routine keeps the PAL code with strength reduction on (`-O2 -G0`), the five strips as `val[1] + (u16)(h / 5) * k`
and the byte loop bound `w * (u16)(h / 5)`. The pusher (an object that shoves the player while `GVARS.playerLanded`
is set: box 24/48/8/16, step 0 waits for `b69` then `velH = 0x200` and `GVARS.b3f = 1`, step 1 decays velH by 8 to
128 and ends past h 177 with the standard player reset, state 18) needed two things: the spawn-point global read
through a one-field struct (`D_8009E52A.pt`, else gcc hoists the `lhu` above the stores) and an empty `case 2:` in
the step switch (the retail tree tests `== 1`, then `< 2`, then `!= 0`; without the third case gcc tests 0 first).

Relocation names, not code: two JP files compiled identically under `jpcheck --norm` but the report had them at
97.9 % / 99.9 % because the JP branch kept PAL symbol names that do not exist in the JP link (`FUN_8001e4f0`,
`MOVE_TABLE_1/2`, the overlay anim tables `D_8013A1E0..`, `KOMA_PIG_ANIM_BOXES`, `DISPLAY_BUFFER_*`, `GV6.s70` for
the raw `D_8009F200`, `GAME.selectedSection` for `GAME_SELECTED_SECTION`). `func_801275C8` and `func_8001AA90` now
use the JP names, yet the report still lists them (97.9 % / 99.9 %, with x00 `func_8010D854` at 99.96 %) although
`jpcheck --strict` without `--norm` shows only the target's own symbol spellings (`applyAnimVelocityX` for
`func_80020A00`, the `jtbl_` names, `.rodata` offsets): objdiff compares something jpcheck does not (open).

Contact and attack twins paired by callee names (scratch `pair4.py`: JP callee set translated to PAL names through
the config comments, PAL file with the same callees and a size within 20 %), 84.28 % -> 85.03 %, the 85 % mark: the JP contact routines of
the overlays drop the `SPAD.b1a4` guard and the `wb4 == 0` early-out, mark the hit object with `b0 = 4`, `b69 = 0`
and (when `b3` is set) `w7a = w2e`, link it at `d4` (PAL `e4`), and set `b69 = 8` on a plain touch:
`spikyLizardContact` x03 (plus a second x03 copy `func_8011E874` that returns when `b3 == 4`), `blackCrowContact`
x04 copy `func_8011DB0C` and x10 copy `func_8011C464` (no `b69 = 8`, the facing goes to `w2e`), `redFlowerContact`
x09, `komaPigContact` x01 (subtype 8 keeps the PAL item check). Event side: `updateMasterOfTheSkiesEvent` x03
(case 2 is a call to the JP-only `func_80128ADC(o, 7)`: the shared "play movie `id`, then warp to area 10" tail
that PAL inlines in each handler; `id` is an `int` cast to `short` only at the `cdSeekStream` call, else gcc copies
a1 to a3 at entry), the five `evilPigAttack*` x14 (no `evilPigSetBoxByDirection` before the flash; Lava Caves
spawns the shot in substep 0 and Trick Village drops the `selectedSection != 7` test), `updateEvilPigBoss` x14 (no
hit stars), `updateRotatingPlane` x09 (no `ba4 = 0`), `updateSwimTeacher` x10 (no `GVARS.b3f = 1`), `komaPigIdle`
x03 and x04 (no `movetab` in case 6; the x04 copy uses anim 10 in cases 6 and 10).

Parallel agent batch (85.03 % -> 85.60 %): six more near-twins, each a small JP edit: `bossRoomPlatformCaptureBoss` and
`evilPigBossDefeated` x14 drop one call (`spawnBossCaptureEffectRing`, `spawnEvilPigHitStars`), `updateRedFlower` x09 drops two
`active` stores, `playerStep25Area9` x09 uses the x0x100 pad masks and inlines `playerSetFallLean`, `updateWindblownMonkey` x03
writes five player globals before the step change, and `updateFoodForFuelObject` x11 sets `d84/d88/d8c` once for every `b0c`.
Second agent round (-> 85.71 %): `evilPigBossHit` x14 (no hit stars, two extra states that count down and replay anim 14) and
`blackCrowPerched` x04 (no snap below 3 in the approach, `SPAD.s1c8` test inverted) match; `playerStepSwimSpin` x00
(`velY < 0` path skips `b69 = 0`) is jpcheck-identical but still 99.96 % in objdiff; `initGameConfig` main is at 16 lines
(drops several stores, `VIEW_CAMERA_Y = -120`, the seed stores move to the end; scheduling left).
Duplicate `// VERSION` lines: when two src files carry the line for the same JP function, build_report builds only one of them
(the last one it walks), so the JP-renamed rewrites of `func_801275C8` and `func_8010D854` were never built (the report kept an
older copy with PAL names). Dropping the stale lines (and naming `MulCosDup`, `D_8009DEE0`, `func_80040940` for JP) took both to
100 % (85.87 %). `jpcheck --norm` hides wrong callee names: check without `--norm` that every relocation name resolves in the JP
map. `func_8001AA90` stays at 99.91 % (jump-table target offsets 8 bytes off).
JP-only round (-> 86.07 %, written from the retail disassembly, one file `func_ADDR_slps01144.c` each): x09 8011C484, 8011E2A4;
x00 800F26AC (landing helper), 801249EC (weapon hit, also built for x01); x03 8012AEC4; x14 8011A0AC, 80119F14 (contact
reactions); x04 801279EC, 80115D9C / 80115EB4 (JP forms of checkScrollItemDoorA/B); x01 8011D2F0. Left: x10 80119D9C (twin of
x04 80115D9C, object pointer in a1 instead of a3) and x14 80118580 (36 lines, same a1/a3 puzzle in case 0).
Second JP-only round (-> 86.37 %): x04 8011E340 (weaponHit form, type-6 counter), 8011B39C, 80115BE4 (cameraModeHandler1
form); x10 80117908, 80116458 (UV cycler), 8011996C (updateCollapsingBridge form), 80117F94 (rotatingWheelSpinStep form);
x03 80131064; x01 8011FE6C; x09 80119758 (updateWaypointRide form); main 80023384 (modelDrawF3Lit form, inline vertices).
Left: x03 8011EEBC (18 lines, b68 value in a0 vs v0).
Third round (-> 86.71 %); most "JP-only" functions are PAL functions the callee pairer missed (inlined or extra calls,
raw globals for struct fields): x14 8011B8E0 = evilPigBossAttack, x04 8011D0FC / x10 8011C280 = updateMightyFishFood,
x10 80116278 = updateUvAnimBgModel, x01 80120F58 = playerStepSinkToGround, x04 80127CAC = the stepMermaidTalk dispatcher;
x03 8012AFF0 / 80129974 and x10 80123744 are more copies of the movie-then-warp tail; x01 80123880 is a slope push-out.
Left: main 800249B8 (2 lines, prologue order), main 800412B0 (51 lines, register allocation, twin of wip func_800406E8).
Instruction-shape pairer (`tools/jp_pair_insns.py`, difflib ratio of the mnemonic sequences of the JP target against every
PAL object of build/base/, ~1 min): 103 pairs where the callee pairer gave 15. Sweep: playerStepLaunched x03 identical as is;
agents closed evilPigBagPortalEnter in six overlays (no `SPAD.b3d1 = 1`, `nextSection` per overlay), motocrossBikeRide and
motocrossBikeJumpStepA x06 (pad mask 0x1000), updateSpringPad x03, segmentedCrawlerGrabbed / CheckWall x01 (-> 87.55 %).
Then x04 playerContactCat2Type07 8011DC88 (no `b1a4` guard), mermaidDivingOfferTalk (`b3e = 1`), updateStormBackdrop (no
`y += 0x40`, camera term `<< 4`) (-> 87.70 %). x06 8011DD3C (updatePitchforkPig, `cullObjectToDrawList(o, 160)`) is
jpcheck-identical but the retail object has a trailing nop past the 0x288 size, so autofix does not install it.
Pairer round 2 (-> 88.24 %): x01 checkRaceEventTrigger (no `GAME_PLAYER_MODE` guards, A test on `s1fc & s3c8`),
playerStepRideSlopeContact (field at d3, `playerSetFallLean` inlined), updatePuppyBaron (no sound block),
segmentedCrawlerHurtFlyOff; x03 wingedForkPigGrabbedB / FlyAcross (wall check inlined, one probe); x04 updateSceneSequence
(no health-restore case, 26 cases); x10 updateLeafButterfly 80122040 (event-start body inlined); x14 8011631C (JP-only).
x01 updateParentFollowModel 8013029C is jpcheck-identical per the agent but 95.5 % in objdiff (prim walk, open); x03
spikyLizardDefeated 80126EC4 is at 18 lines (instruction order).
Pairer round 3 (ratio >= 0.45, -> 88.68 %): x03 windblownMonkeyGiveUmbrella, animateArea3Palettes, wingedForkPigWander;
x01 lostSeedKidTalk; x04 blackCrowSwoop; x09 801201B4 (JP-only); main 8001E910 (playSFXAndSetNote with the free-voice
search inlined; own file, because jp_autofix keeps the PAL file's flags and this one needs `-fno-strength-reduce`).
Left: main 80024300 (lit F3 draw, 26 lines, colour clamp registers).
Pairer round 4 (120-200 diff lines, -> 89.25 %; half the pairs were wrong and rewritten from the disassembly): main
areaTransitionHandler (cases 0-3 only), processObjectCollisions (collide loop -> one call), 8002457C (fade F4, JP-only);
x01 updateCharlesEventObj; x04 8011A268 (stepAreaEventDoor form); x09 80119A4C, 80125484, 8011B69C (AP box form), 8011D21C;
x10 8011A5D0; x11 80117AFC. Left: x11 80117260 (factoryBossIronCastleDoorTalk minus the anim loads, 10 lines, case 5 order).
Pairer round 5 (200-290 lines, -> 89.73 %): x06 motocrossBikeSetSpeed (no rumble, masks x0x100), x03 playerStepSpringBounce
(fall lean inlined); own files: x14 801189B4, 8011C5B4 (evilPigBossIntroTalk form), x09 8011F1E0 (drawArea9ModelGT4 form),
x16 8011525C (updateAreaScroll form; keeps its old `func_8011525C` symbol: two names on one address make splat fail),
x01 8011DDE4 (needs `-O2 -G0` without `-fno-strength-reduce`), x11 801169A8. Left at register level: x10 8011F240
(41 lines), x04 80118990 / 80118720 / 80118B98 (UV animators, 60-208 lines).
Pairer round 6 (290-420 lines, -> 90.12 %, the 90 % mark): x01 80123014 (collidePlayerWithTiltedBar form), 801245B0
(segmentedCrawlerWeaponHit form, no item clone), x14 80117B54 (updateBossRoomPlatform form), x10 8012453C (10-state NPC
script), x00 8010E500 (playerAirAccelerateX form, no `GV6.s10 == 7` branch, params read in place from `D_80114748`).
Left at register level: main 8004CB14 (JP pause backdrop, not drawPauseMenu; 23 lines), x00 80120314 (31), x00 800F6874.
Round 7 (-> 90.75 %): x00 800F4CDC (playerStepSpinOnObject, 5464 B: init store order, pad masks x0x100, no rumble), x01
8011B4FC (updateRopeChains, 2336 B: built without `-fno-strength-reduce`, one `ObjCullRegister` test), main 8002C070
(updateL8Type01Object, fewer guards) and 8001E618 (playSFXWithPitchSlide, first free-voice loop inlined). The sweep had
missed these main ones because their units carry `_slps01144` names: check them as `func_ADDR_slps01144`.
Then main 8002B2D4 (cameraEventScrollHandler: no `disableSelectMenu` store, `D_80071694[nextArea]` decides stopBgm),
90.84 %. main 8001D5E0 (moviePlayerTask: no DecDCTinSync / timeout / CdControlB loop) is at 8 lines (prologue order).
Permuter (decomp-permuter on CPU, set up per function with `tools/jp_permute_setup.py FILE PROG NAME DIR`; needs a
`mips-linux-gnu-objdump` on PATH, a symlink to the mipsel one): x03 8011EEBC (spikyLizardWeaponHit form) closed by a
redundant `else if (a->type || o) v = 2; else v = 2;` that gives the retail branch shape (-> 90.87 %).
Then main 8004CB14 (the JP pause backdrop, not drawPauseMenu): hand-ordered coordinate stores from the permuter's best,
then a second permuter run from that seed found `do { px = &D_80078404[k]; } while (0);` inside a `do { } while (0)`
around the loop (-> 90.95 %). Seeding the permuter again from a hand-improved best works better than a long single run.
x11 80117260 (factoryBossIronCastleDoorTalk without the anim loads): case 5 needs `do { w = o->wbc; } while (0);`
before the scratchpad store and `o->animFrame = w` after it (permuter hint, finished by hand) (-> 90.99 %).
x04 80118990 (UV byte-split animator): `register unsigned char st asm("a2") = o->state` by hand, then the permuter
reseeded from it found the copy `new_var2 = st` for `state = new_var2 + 1` (-> 91.02 %).
x00 800F6874 (seesaw pivot, reads `D_800A0C7C`): `register TObj *g asm("a0")` plus a copy `new_var2 = g` for the
case 20 switch, with case 31 back on `g` (permuter run, last two lines by hand) (-> 91.09 %).
Inline pairer (`tools/jp_pair_insns.py --inline`, ~10 min: the PAL sequence with one callee spliced in at its jal, or
the JP one with a JP callee spliced in; 29 new pairs at >= 0.55): x09 8011A288 (stepBucket with a shorter JP
bucketFillWithWater inlined), x04 80124270 (redBirdStep3 with new constants and an inlined floor probe; it was not
blackCrowFlyAround) (-> 91.35 %). PAL data names in agent drafts (`D_8013E70C` and the like) do not resolve in the JP
link: rename them to the retail relocation names before installing.
Then x10 8011B4F8 (the seesaw ride, playerStepRideSeesaw standalone in the x01 func_80121B5C form: rotation in d30,
fall lean inlined, `asm("")` in the goto tails against cross-jumping) (-> 91.52 %).
Inline pairer round 2 (-> 91.94 %; the pairer's PAL name was often wrong, but it pointed at the right family): main
80049704 (drawMessageBoxFrame form, the balloon frame is a call), 8002F7C0 (updatePoolType31Object, calls
func_8002FD60), 8001E81C (one-argument sound wrapper, sibling of 8001E910), 8002F994 (debug object updater with
sprintf/drawDebugText), 8001906C (the JP title loop is a three-entry debug-text menu); x00 800EB714 (a small updater
plus the 13-case area dispatcher, two functions in one splat symbol); x09 8011ED08 (modelDrawFT4 form, no object),
8012397C (updateDeathFruit form); x04 8011A6BC (updateAreaMechanisms with its substep switch moved to func_8011D598).
Then main 800200C4 (the JP queueLoadList: flag-tested skip prelude, no 0xf8 case, built without
`-fno-strength-reduce`), x03 80119850 (a small JP updateAreaMechanisms, 8-way subtype switch) and x10 80124F28 (the
31-state mermaid dialogue machine) (-> 92.25 %).

Inline pairer at 0.40 (2026-11-06): playSFX (8001E2D4, the voice search inlined, `a` copied through an asm barrier),
main 8002BA74 (a JP camera-controller variant with an extra state switch; its symbol size was 0x30C, the real one
0x388, which cut the target short) and x10 80119B8C (parallax backdrop with a GAME_SELECTED_SECTION shift switch).
The permuter then fixed the three playSFXWithNote / 8001E458 / playSFXWithNoteAndVolume (the last queueSoundCommand
result held in an `unsigned` variable, the asm copy in a `do { } while (0)` with the call: the delay slot fills).
Open, scheduling only: x10 80119D9C and
x14 80118580 (case-0 loads hoisted above stores), printInfoMessage = 8002EDB0 (JP form, 54 register lines).
Next pairs (2026-11-07): main 80018818 (bootStartupTask without the logo states), main 8001FE6C (a synchronous CD
load, not weaponHitCat2Type08), x09 8012908C and x11 80116348 / x10 801249FC (JP-only NPC talk handlers rewritten
from the disassembly). Two symbol sizes were 4 bytes short (the last `nop` after `jr ra`), which objdiff scores
as 99.6 %: check the size against the next symbol when a jpcheck-IDENTICAL function reports below 100.
Open: x10 80115DB8 (register allocation in case 0/4), _SsVmSetVol (splat cut one 0x584 function in five).
Last round (2026-11-07): x00 80131A70, x14 8011B5A8, x09 801286E4, x10 80117378 and x10 801158B4, all JP-only and
rewritten from the disassembly; three symbol sizes fixed (801158B4 ran over CdFlush). Open, scheduling only: x14
8011FD98 (since matched by the permuter: `short f`, `d88 - -0x80`, a split d88 store), x10 8011BE2C (20 lines, case-2 load order). x09 80120CC4 and 80122168 since matched (permuter `char t`, an asm barrier on the loaded SPAD value; `chk()` written as an early return).
A further hand round on the open five found no match: x10 8011BE2C (~20 lines, s1fc load after the BITTEN pointer
in retail), x10 80119D9C / x14 80118580 (~30-36, `o` in a3 in retail: probably one more long-lived temp in case 0),
main 8002EDB0 = printInfoMessage (~40) and x10 80115DB8 (40; its case 3/7 lacked `hi = lo + 10`, now fixed). Best
drafts are kept outside the repo; all five are register allocation only.
JP-only batch (2026-11-07): x09 8011EAD4 / 8011EFD4 (GTE draw loops), x06 8011AA34, x01 8011FFFC, x09 8011C2D8
(parallax backdrops with JP tables), x01 8011AD50 (nested state switch, empty `case 2:` labels), x14 8011E0B8
(baccusVillagePigShotCheckLanded without the d38 guard), main 8001C2D8 (an older pause menu). Close but open:
x00 80131858 (one register), x03 80126EC4 (35), x14 8011FB54 (60), x10 8011EF5C (103).
Next batch: x14 8011B048 (evilPigBossTeleport without the effect rings), x09 801246FC (jellyfishSwimB with JP
globals), x00 8010EC44 (playerUpdateSpeedFromPad without the PAD_INFO 7 branch), x17 801153B8 (updateAreaScroll)
and x14 801155DC (updateSectionClearController; its size ran over CdFlush again). Open: x09 8011F65C (one addu
operand order, 8 lines), main 8001CA50 (94 lines, loop-invariant hoist order). Also matched: x01 8011677C
(selectArea1VisibleModels with 5 sections) and x10 80126728 (swimTeacherFloatTalk with JP text ids).
Then x10 801259C4 (swimTeacherIntroTalk without the pad vibration), x18 80118854 (a JP-only Mizuno item turn-in
talk) and x13 801162DC (updateSceneSequence at 60 Hz constants, the stats screen drawn with sprintf).
Then x03 80116748 (updateAreaScroll without one PLAYER.active store) and x09 80126994 (jellyfishDefeated with an
inline item drop). Open in main: 80026B7C (80 lines, one loop), 80055F2C (160), 80042280 (460), 8002DA3C (680).
Large overlay functions turned out to be PAL files almost as is: x10 8011DCC8 (solidSceneryPlayerCollision, the
JP pad word D_8009F200 with bits x0x100), x04 80115FD0 (updateAreaScroll, item windows 16 wider, pad bit 0x1000),
x01 80115C64 (updateAreaScroll; scratchpad shorts reloaded through `asm(".set NAME,0x1f8001xx")` absolute symbols)
and x01 8012C8CC (komaPigHit without func_8002B920). Open: x04 80117938 (no PAL twin, a tile-grid visibility list,
~300 lines). The 0x1000 bytes at main 80014D1C were libc/libpress rodata (printf digit strings, MDEC_rest
messages, vsprintf jump tables) that splat had split as code: the c subsegment now starts at 0x651C (main,
0x80015D1C), which drops them from the code total. main 80024B78 is a hand-written GTE mesh renderer with no twin.
main 80025F70 is subtractScaledVertices built without -fno-strength-reduce, with one count per loop and the dst
offsets equal to src. Open: main 800498AC (drawBalloonFrame, 144 lines, registers of the tail arrow), main 80026B7C
(since matched: 11 unused `int uN = (int)p;` locals add pseudos, which lifts local-alloc's quantity limit so the
movstrsi scratch registers of the 8-copy loop are no longer spilled to t3/t4). A stronger-model round also matched
x00 80131858 (a pointer copy `q2 = q` in state 3 moves the state-1 temp to v1) and x09 8011F65C (`d + o->box2` with
the unsigned short copy). Open: x10 8011BE2C (16 lines, local-alloc tie in case 2). Then x14 8011FB54 (the Rand
result kept in the loop index; JP sound ids swapped). Open: x10 80119D9C / x14 80118580 (only `o` in a1 vs a3),
x03 80126EC4 (20). Since matched: x10 80119D9C / x14 80118580 (one joint `asm("" : "=r"(k), "=r"(j) : "0"(k),
"1"(j))` after the two scratchpad loads keeps `o` live, so it gets a3), main 8002EDB0 printInfoMessage (one pointer
q for three temps, `register char *base asm("$4")`, -1 in a variable, case -6 sums through short temps) and main
800498AC drawBalloonFrame (PAL C with JP colours, no default case, `register short w asm("$9")`).
Then x01 updateParentFollowModel (PAL loop form, no extra q pointer), x00 80120314 (one pointer for the header
chain), main 8001D5E0 moviePlayerTask (MOVIE_DEC_ENV fields with no local pointer) and main 80024300 (`unsigned char
r`). x06 8011DD3C is instruction-identical but scores 99.4 %: the next function starts after one padding nop that the
target includes. Open: x03 80126EC4 (2 lines), main 80016E6C (18), main 800412B0 (101), x04 80118720 / 80118B98.


Left over with a known diff: `func_800498AC` (drawBalloonFrame, 2608 bytes: the tail-sprite `x`/`y` land in
`a1`/`v1` instead of `t0`/`a3`, everything else matches), `func_80027D58` (increaseMaxHealth:
the `andi` after the `sb`), `func_80016E6C` (initGameConfig: the `BUTTON_CONFIG_INDEX` load scheduled first),
`func_8001A8CC` (areaMode2Tick), `func_8010D854` (swim spin), `func_8001CE50` (acquireSpriteSlot: JP masks with
0xffffff and walks the slot table differently), the lit `modelDraw*` variants (JP lights in software, other argument
layout) and the pairs whose JP twin is a different function (`rewardJewel`, `areaTransitionHandler`,
`playerSelectGroundAnim`, `playerReleaseClingObject`, `updateEffectHitbox`, `spawnBreakPieces`, `updateFlashEffect`,
`spawnGroundDebris`, `playerFloatDirFromPad`, `updateTreasureChestGuard`, `playerHurtTumbleGetUp`,
`playerFlipOntoObjectPlace`, `updateEggShellPiece`, `l4Type03Step`). `func_801230FC` (x14) is a data table marked as a
function in the symbol table; `func_80030CD0` was the tail of `func_80030A68` (now sized 0x274).

What remains, by kind (for the paired functions with PAL C):
- about 40 that differ in size from their PAL pair (edited code, not just offsets);
- multi-function PAL files whose JP counterpart is split differently (other than the whole-file case below);
- about 145 JP functions are paired with PAL functions that have no C yet, and about 840 have no PAL pair at all
  (edited or JP-only code, text/font system): manual decompilation of the near-misses (`fuzzy_match_percent` 90 %
  or more) is the next step.

Not linked yet: like the demo, the JP objects are compared by objdiff only; `tools/build_full.py` has no JP target.

## Native port (stage J1, 2026-11-11)

```
sudo tools/setup_native.sh                                    # old-gcc, maspsx and include/tomba/psyq (required)
python3 tools/build_full.py --version jp -j 8                # the symbol source (build/slps01144/full, 25 programs)
python3 tools/extract_data.py game_jp/disc/tomba_jp.bin data_jp   # the JP disc's data/ (manifest: serial, boot, boot_sha1)
cmake -S port2 -B build/port2-slps01144 -DPORT_VERSION=jp && make -C build/port2-slps01144 -j8
build/port2-slps01144/tombi2 --data data_jp --headless --skip-movies --frames 4500 --pad-script 1000:start,1300:circle:3x20/60
```

The same pipeline as the USA port (docs/USA.md "Native port": `PORT_VERSION`, `port/tools/portver.py`,
`gen_port.sources_version`, `gen/version/port_version.h`), with the JP table: boot `SLPS_011.44`, overlay base
0x800E9AF8, shared prefix 0x2B680, the SYSTEM overlays `O0PTSUB0` = OPTSUB00.BIN, `D0SPSUB` = DSPSUB.BIN, `I0NFO` =
INFO.BIN, ..., no GOVER (the PAL SYSOVL table entries the version lacks are dropped), the library part from `_bu_init`
0x8005844C on (Psy-Q left to the shims). `gen/report.txt`: 51 stubs (psyq/bios 48, game-asm 2), 0 unresolved literals.

What the JP game needed beyond the generator (all in the first JP port commit):

- **CD reader**: the JP `cdLoadTask` (src/cdLoadTask.c JP branch) is the older libcd one: `CdControlF(CdlReadN)`
  after `CdReadyCallback(cb)`, the callback copying each sector out with `CdGetSector` and ending the read with
  `CdReadyCallback(0)` + `CdlPause`. `port2/shims/psyq.c` delivers the whole file inside the ReadN command
  (`cd_readn`; the load-list / overlay relocation of `CdRead` moved to `cd_after_read`, shared by both readers).
- **Pad**: no libpad in the JP executable. Its vblank handler (`src/vblankHandler_slps01144.c`) polls the older libetc
  `PadRead(1)` = `func_80064B74` (PadInit = `func_80064B24`, BIOS InitPAD / StartPAD). `port/fixes.txt` (`@version
  jp`) feeds it `port_pad_read_libetc()` of `shims/pad.c`: the host pad in PadRead's bit order (pad 1 in the low half,
  the two BIOS bytes big-endian, pressed = 1: select 0x0100, start 0x0800, d-pad 0x1000..0x8000, L2 R2 L1 R1
  0x01..0x08, triangle circle cross square 0x10..0x80). JP confirms with circle.
- **Frame wait**: `SetRCnt` / `StartRCnt` of the JP `func_80015D1C` (main's wait) removed, `port_vblank()` instead.
- **PAL names in JP branches**: a JP branch that calls a PAL function by its PAL address name (`func_800E8A14`,
  `func_8001D6A4`) gets a data-image address and faults; the JP names are `func_800EA048` / `func_800EA060` (O0PTSUB0
  bootLogoSetSpriteBase / bootLogoDraw) and `func_8001D5E0_slps01144` (the movie thread) in bootLogoState, and
  `func_800323A8` for weaponSetPlayerAnimC in weapon0Swing. A JP file kept as asm by the link (`.rodata overlaps`)
  is not checked against the retail bytes, so these only show at run time (stub / fault lines of the log).
- **asm("SYM+N") externs** (`D_800A1386 asm("D_800A1384+2")`): gas evaluates the expression, clang (wasm2c, the NDK)
  takes it as a symbol name; `port2/tools/gen2.py` rewrites such a declaration as a macro over the symbol's bytes.
- `--second-screen` / `--dump-panel` are refused: `runtime/panel_addrs.h` has no JP column yet.

Checked (headless, i386): logos, title (PUSH START, NEW START / LOAD START / OPTION), new game, AREA00: the opening
dialogue (ちょっと まてよ ...), the event title 100年老人との出会い, HUD, attacking the pigs, walking. 12 stub calls
in 4500 frames (PadInit, GetVideoMode, SpuInit). The PAL build regenerates the same stubs.c / funcmap.c and plays the
same. Not checked: other areas, movies, saves, sound output, the Windows / Android builds of JP.

(The MAIN0 object-handler table at 0x8007593C held `func_800EB7C8`, a native X000 function inside another unit, which
made `gen2.emit_data` take the whole table for data: its X000 handlers were called through the fault handler on the PC,
~30 calls per frame, and trapped on wasm2c (`Invalid call_indirect`). A native X000 function now counts as a function
start of a run; the PAL data.c is unchanged by that.)

Open: the 7 `.rodata overlaps` data units of the JP link (`src/data/slps01144/`:
freeObjectByLayer, bootLogoState, playerEffect4EStepC, weapon0Swing, weapon8to9Swing, the two treasure chest files,
updateHoodedOldMan, updateCharlesTalkScene, phoenixSpiritHintWalkC, func_80129CE4); `-v` reports (area / player
position) read PAL addresses; the panel's JP address column (GAME 0x8009E508, GCD 0x8009E8A8, SCRIPT_FLAGS
0x8009E94C: the PAL offsets apply if the structs match).

Launchers and CI: `tools/game_library.py` marks SLPS-01144 supported (`tombi-pc-jp`, `build/port2-slps01144`,
`saves_jp`, no PAL-only feature), Android `Library.java` gives it `libtombi_jp.so` / `saves_jp`
(`port2/android/game_libs.py --version jp`); `port2-exe.yml` and `android-apk.yml` build the JP game from the
`GAME_FILES_URL_JP` secret as they build the USA one from `GAME_FILES_URL_USA` (matching build cached as `fulljp-*`,
the APK libraries as `jplibs-*`; `versions.txt` of the PC bundles lists `jp`).

## SYSTEM overlays (2026-11-08)

| Step | Code | Functions |
|---|---:|---:|
| before (exe + X overlays only) | 98.79 % | 3036 / 3092 |
| SYSTEM overlays added, PAL pairing from `symbol_addrs_<prog>.txt` | 92.42 % | 3307 / 3530 |
| `reuse.txt` lines of the SYSTEM programs (`jpdemo_reuse.py`, only those 276 lines appended) | 92.59 % | 3312 / 3530 |
| `tools/jp_autofix.py --install` (10: 9 branches in PAL files, 1 JP file) | 92.94 % | 3322 / 3530 |

Per overlay: INFO2 84.0 % (157/191), INFO 68.4 % (37/51), INFO3 67.0 % (37/52), OPTSUB00 41.0 % (8/16), DSPSUB2
14.1 % (12/28), DSPSUB 13.9 % (20/57), DSPSUB3 11.8 % (15/43). The DSPSUB* sub-screens / memory card are far from PAL
(Japanese text layout, xmatch finds only 4-6 % of their bytes); xmatch pairs most of JP DSPSUB3 with PAL D0SPSUB. The
208 functions left need hand work; jp_autofix near misses: memCardProbe (3 lines), subScreenMapDrawRoomName (27),
overheadBuildMapStrip in INFO and INFO2 (298).

## Data measure (2026-11-09)

The decomp.dev "Data" bar of the JP and USA reports is counted like PAL's (tools/report_data.py), in
`build_report.jpdemo_data`:

1. the data sections a unit's C places at their retail address (jump tables, constants, strings), as before. The
   area overlays had none until now: the report took the vram of the area segment (0x80115178) as the address of
   file offset 0, so every claim compared the wrong bytes;
2. the X00 bytes every area overlay repeats at its start (`JP_SHARED` / `USA_SHARED`, identical to `X00.BIN`) are
   left out of the overlays' data as they are of their code: they are X00's, counted once in `x00` (before, 16 copies
   of them were 82 % of the JP data total);
3. the PAL data files (`src/data/<PAL module>/*.c`, symbolic C) that the version has too: tools/version_data.py
   compiles them, finds each section in the version's file with the relocated words masked (unique hit, or the shift of
   its placed neighbours) and accepts it only when every pointer points at the version's address of its symbol
   (version symbol name, PAL address carried over through placed data files or the function pairing; a symbol nobody
   knows must resolve to one address in all its uses);
4. the version's own data files, `src/data/<ver>/<prog>/D_<addr>.c` (`// DATA addr size <ver>/<prog>`), written by
   `python3 tools/version_data.py --version <ver> gen` from `build/<ver>/data_rest.json` (the ranges the last report
   left): the retail bytes as words, like the PAL "unclaimed range" files. The WFM2/WFM3 banks (found by their magic)
   and `config/<ver>/data_assets.txt` (the sub-screen menu image of DSPSUB / DSPSUB3) are never written: ship
   scripts, not pixels; they stay unmatched, as the same assets do in PAL.

A version data file that later overlaps C (a new claim) is reported as `not reported` and left out; regenerate:
report, delete `src/data/<ver>/<prog>/`, `version_data.py gen`, report again.

| Version | Data before | after 1-3 | after 4 | left (assets; plus ~1 KB of JP unit claims that differ) |
|---|---:|---:|---:|---|
| JP SLPS-01144 | 0.94 % | 18.59 % | 83.74 % (525196 / 627172) | DSPSUB 64020, DSPSUB3 36880 |
| USA SCUS-94236 | 0.97 % | 20.95 % | 90.88 % (715288 / 787092) | DSPSUB 46568, DSPSUB3 25236 |

Code stays 100 % (3528 / 3528, 3681 / 3681). Open: the generated files are plain words; pointers as symbols (what
the PAL animation-table files do) would make them relocatable. Most PAL tables differ from JP/USA by a few words
(other text, moved fields), so the per-array reuse of PAL files (placing each array of a file on its own) would
replace more of the generated words with symbolic C.
