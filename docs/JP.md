# Japanese version (Ore! Tomba)

The Japanese release is a second decomp.dev version built from the shared `src/` tree (see "How the versions
share code" in the README): `--version slps01144` (alias `jp`), macro `VERSION_SLPS_01144`.

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
- `python3 tools/build_report.py --version slps01144` writes `build/slps01144/report.json`; CI job `report-jp`
  uploads it as `SLPS_011.44_report` from the `GAME_FILES_URL_JP` secret.

**Progress: 82.38 % of code (1168620 of 1418632 bytes, 2711 of 3095 functions; README table)**, mostly PAL C reused as is, the rest from
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

Hand pass 3 (2026-11, 2533 -> 2711 functions, 68.66 % -> 82.38 %): the PAL C renamed for JP and diffed with
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

