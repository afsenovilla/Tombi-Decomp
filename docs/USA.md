# USA release (Tomba!, SCUS-94236)

> **100 % matched on 2026-11-09**: every function of the retail disc, SYSTEM overlays and Psy-Q library included.

## Matching target

`--version scus94236` (alias `usa`, macro `VERSION_SCUS_94236`), `config/scus94236/`, decomp.dev version
`SCUS_942.36`. Same layout as the Japanese retail (docs/JP.md):

- boot exe `SCUS_942.36` (sha1 `81cbc79f0230aeb4252e058039f47ac95a777f5a`, entry 0x8006B58C, 0x88000 bytes at
  0x80010000); text 0x800163B0 (`main`) .. 0x80076E38 (end of `SpuGetAllKeysStatus`);
- `SYSTEM/X00.BIN` .. `X19.BIN` (no X07, X12, X15), load address 0x800E7388 (jal targets on X00 prologues; PAL
  0x800E8028, JP 0x800E9AF8). Every `X<nn>.BIN` starts with the same 0x2DE6C bytes as `X00.BIN` (`USA_SHARED` in
  `tools/versions.py`); `config/scus94236/x<nn>.yaml` splits only the area part (vram 0x801151F4), so the shared code
  is counted once, in `x00`.
- SYSTEM code overlays `DSPSUB.BIN`, `DSPSUB2`, `DSPSUB3`, `GOVER`, `INFO`, `INFO2`, `INFO3`, `OPTSUB00` (programs
  `dspsub` .. `optsub00`, `USA_SYSOVL` in `tools/versions.py`, paired with PAL `d0spsub` .. `o0ptsub0` by `pal_prog`):
  raw blobs at the same base 0x800E7388 (header pointers = PAL - 0xCA0). `config/scus94236/<prog>.yaml` splits header
  (bin), code, trailing data (bin); code ranges DSPSUB 0x794-0x962C, DSPSUB2 0x20C-0x4E0C (the memory-card low-level
  functions come first in USA), DSPSUB3 0x6EC-0x6820, GOVER 0x1C-0xC6C, INFO 0x1EC-0x6730, INFO2 0xAE8-0x16848, INFO3
  0x454-0x6C84, OPTSUB00 0x38-0x1FC0. Symbols: `xmatch.py` on the SYSTEM files (it now also reads the PAL SYSTEM
  symbols; run `tools/gen_symbols.py` first) + `jpdemo_syms.py`, which now rewrites only the programs in the csv.

Symbols: PAL names at USA addresses (`tools/xmatch.py` + `tools/jpdemo_syms.py --version scus94236`), the task table
hints in `symbol_addrs_manual.txt` (copied from JP), `data_units.txt` (`tools/jp_data_units.py --version scus94236`:
entry/jump tables splat took for functions) and `reuse.txt` (`tools/jpdemo_reuse.py --version scus94236`). The local
game dir is `game_usa/fs/` (`bash tools/disc_fetch.sh USA`; git-ignored). CI: job `report-usa` of `report.yml`
(secret `GAME_FILES_URL_USA`, artifact `SCUS_942.36_report`) and the USA disc in `jp-sweep.yml`.

## How to reproduce

```
bash tools/disc_fetch.sh USA
python3 tools/gen_symbols.py
python3 -I tools/xmatch.py game_usa/fs/SCUS_942.36 game_usa/fs/SYSTEM/X*.BIN --csv build/scus94236/xmatch.csv
python3 -I tools/jpdemo_syms.py build/scus94236/xmatch.csv --version scus94236
python3 tools/build_report.py --version scus94236 --objdiff /root/.cargo/bin/objdiff-cli
python3 tools/jp_data_units.py --version scus94236 --write
python3 tools/jpdemo_reuse.py --version scus94236
python3 tools/build_report.py --version scus94236 --objdiff /root/.cargo/bin/objdiff-cli --no-split
python3 tools/demo_sweep.py --version scus94236 --report [--fix]
# SYSTEM overlays only (keeps the other symbol files):
python3 -I tools/xmatch.py game_usa/fs/SYSTEM/{DSPSUB,DSPSUB2,DSPSUB3,GOVER,INFO,INFO2,INFO3,OPTSUB00}.BIN --csv build/scus94236/xmatch_sys.csv
python3 -I tools/jpdemo_syms.py build/scus94236/xmatch_sys.csv --version scus94236
```

Data measure (90.88 %): docs/JP.md "Data measure" (same pipeline, `src/data/scus94236/`).

## First figures (2026-11-08)

USA is much closer to PAL than JP is (xmatch: 61 % of the exe bytes and 85-91 % of every overlay pair with PAL by
masked hash).

| Step | Code | Functions |
|---|---:|---:|
| first report, shared tree as is | 77.35 % | 2855 / 3571 |
| tables taken out of the code units (`data_units.txt`) | 92.17 % | 2855 / 3209 |
| `reuse.txt` | 93.09 % | 2879 / 3209 |
| `demo_sweep.py` (131 installed: 104 pairings, 13 branches, 14 USA-only files) | 93.95 % | 3010 / 3209 |
| `demo_sweep.py --fix` (39 more, PAL C edited by the tools/jp_autofix.py edits) | 95.66 % | 3049 / 3209 |
| second `demo_sweep.py --fix` after the JP Psy-Q asm files (24 more) | 95.79 % | 3071 / 3209 |
| psx_tomba ports (35) + hand-written Psy-Q asm (64 functions in 50 files) | 97.47 % | 3152 / 3218 |
| X13 updateSceneSequence (PAL file, `FPS` 60 for NTSC) | 97.65 % | 3154 / 3218 |
| message box 8002FA24 (psx_tomba) + 30 near-100 % functions (15 reuse the JP branch, 15 USA files: NTSC screen
  height = PAL - 8, pad struct one byte earlier, scratchpad bytes that are RAM globals in USA) | 99.12 % | 3185 / 3218 |
| library cleanup: one file per BIOS/libcard unit, FGO_01_OBJ_160, DecDCTvlc(Size), libcard patch; three PsyqStamp /
  vlc_size data words moved to data_units.txt | 99.22 % | 3199 / 3215 |
| the last 8 game functions (PAL/JP twins with USA names; drawObjectSpriteRotScaled with the 0xe0 screen
  height) | 99.77 % | 3207 / 3215 |
| libsnd `_SsContDataEntry` (0x430) and `_SsVmSetVol` (0x584) from psx_tomba, one function each in all versions | 99.94 % | 3209 / 3213 |

What is left is mostly in the boot exe (83 %): functions that differ from PAL by more than the automatic edits.

## SYSTEM overlays (2026-11-08)

| Step | Code | Functions |
|---|---:|---:|
| before (exe + X overlays only) | 99.94 % | 3209 / 3213 |
| SYSTEM overlays added, PAL pairing from `symbol_addrs_<prog>.txt` | 96.93 % | 3589 / 3681 |
| `reuse.txt` lines of the SYSTEM programs (`jpdemo_reuse.py`, only those lines appended) | 97.10 % | 3594 / 3681 |
| `demo_sweep.py` (28 pairings) | 97.11 % | 3622 / 3681 |
| `demo_sweep.py --fix` (7 more: 6 branches in PAL files, 1 USA file) | 97.52 % | 3629 / 3681 |

Per overlay after the sweep: GOVER 100 %, INFO3 98.0 %, INFO2 96.5 %, INFO 87.8 %, DSPSUB3 66.3 %, DSPSUB 65.6 %,
OPTSUB00 64.0 %, DSPSUB2 44.9 %. The 52 functions left are mostly the large sub-screen / memory-card / title drawing
functions, where the English text layout differs from PAL by more than the jp_autofix edits; no PAL pair was found
for many of them (splat `func_`), so they need hand work (PAL C as the starting point).

| Hand passes (2026-11-08/09) | Code | Functions |
|---|---:|---:|
| SYSTEM hand pass (DSPSUB*, OPTSUB00, INFO*, from the PAL C) | 99.45 % | 3673 / 3681 |
| dspsub2 800E954C (PAL 800E999C port), both overheadBuildMapStrip (JP trick), libapi psyq_8005ba74/bc10 | 99.81 % | 3678 / 3681 |
| libsn `__main` / `__do_global_dtors` (hand-written Sony asm, from the JP files) | 99.83 % | 3680 / 3681 |
| dspsub 800EEE84 (twin of PAL D0SPSUB 800EFD6C) | **100 %** | **3681 / 3681** |

## Full link (stage U0, 2026-11-09)

`python3 tools/build_full.py --version usa -j 8` links all 26 USA files from the C (`build/scus94236/full/`, ~5 min) and
checks them by sha1: `SCUS_942.36`, `X00.BIN` .. `X19.BIN` and the eight SYSTEM overlays are **byte-identical** to the
disc (also `cmp`-checked against `game_usa/fs`), with `<prog>.full.ld` PROVIDE tables and `<prog>.map` like the PAL
build. Each version function is placed at its USA address from the file `tools/units.py` `version_sources` gives it
(the same resolution as the report; `units("scus94236")` lists them); an area overlay links its area part only, the
X00 prefix comes from the X00 linked first. 3 651 of the 3 680 C units link (the rest are the same Psy-Q units the PAL
link also keeps as asm: libgpu/libcd .rodata shared between files, `_SsVmPBVoice`, `playerStepClingRotating`,
`pigGateDiscoveryScene`), plus 18 USA data files at odd addresses that cannot replace word lines (raw bytes, nothing
to prove).

The link infers every relocation from the retail bytes, so it proves the bytes and the address each name stands for;
the **name audit** (`build/scus94236/full/<prog>.names.txt`) then says what the names mean:
- `named`: a name the USA symbols give another address. Almost all are *template reuse*: one PAL file whose code has
  the same shape builds several USA functions that call other targets (`playerPushOutOfObjectBoxThunk.c` for ~20 jal
  thunks, `topdownActorHandler.c`, `bitingPlantSway.c`, `GetGraphType.c` / `CdSetDebug.c` for "return a global"). The
  link PROVIDEs such a name renamed (`name__ADDR`) per object, so **the port generator must compile a shared file once
  per USA unit and apply the per-object renames** (stage U1);
- `raw (version C)` / `raw (shared C)`: D_/func_ names with another number (PAL/JP addresses). The USA-only files and
  `#if defined(VERSION_SCUS_94236)` blocks were renamed to the USA addresses (234 names); what is left is in shared PAL
  code or in `JP || USA` branches, where the PAL/JP name stays (a port must never resolve a D_ name by its number);
- `unknown`: PAL names with no USA symbol (their USA address is the link's).

Bugs the link found (all invisible to objdiff with `functionRelocDiffs: none`; each one would have been a USA port bug):
- `start_scus94236.c`: the JP copy used one symbol for the bss start and `$gp` (USA 8009AFB0 / 80097FA8), and JP
  addresses for the rest;
- `freeObjectByLayer.c`: the build stamp before its jump table is "98/3/22" "21:11" in USA (PAL "98/8/26" "14:10");
- `queueLoadList.c`: its table runs past the end of the USA exe (zeros, bss): build_full now places a data section
  that ends past the file when those bytes are zeros (report_data pads the target the same way);
- static tables of PAL C with PAL pointers or values, USA reads the retail table instead (extern at the USA address):
  `initWeaponTrailSprite`, `spawnA00TornadoOrbiters`, `updateCharlesTalkScene` (other contents), x01
  `updateChainSegmentEnemy`, x03 `phoenixSpiritHintWalkC`, x10 `updateSwimTeacher`, x14 `evilPigPlayAttackSfx` (sound
  ids one higher in USA);
- cross-area twins in `reuse.txt` that set another area's event flag (`evilPigBagPortalOpen` of x01 for x02/x03/x04/
  x09/x10/x16: `GCD.bcf0b` instead of the area's own byte; `spawnNpcSet20` / `stepHidingSonTalk` / `updateHidingSon`
  of x03 for x04/x10: `GCD.bcfcd` instead of `bcfcb` / `bcfcc`): 53 reuse pairs now take the program's own PAL module
  (`tools/jpdemo_reuse.py` prefers it too);
- dspsub2 `memCardWaitSwEvent` / `memCardClearSwEvents` were built from the PAL twin that waits on the other event
  set (MEMCARD_SW_* instead of MEMCARD_HW_* in USA);
- `_padIfSioRW` USA branch: PAL addresses of two libpad globals.

USA data names: `python3 tools/jp_globals.py --version scus94236 --write` (286 data symbols of PAL BLOCKS and named
globals, `// BEGIN jp_globals` blocks of `config/scus94236/symbol_addrs_*.txt`); the audit confirms them (2 000 more
names agree). Open: `SCRIPT_FLAGS` (block rule 8009C10C, but the USA script flag ops read 8009C20C), `PLAYER_H` /
`PLAYER_Y` in the sub-screen condition templates.

## Native port (stage U1, 2026-11-09)

```
sudo tools/setup_native.sh                                    # old-gcc, maspsx and include/tomba/psyq (required)
python3 tools/build_full.py --version usa -j 8               # the symbol source (build/scus94236/full)
python3 tools/extract_data.py game_usa/disc.bin data_usa      # the USA disc's data/ (manifest: serial, boot, boot_sha1)
cmake -S port2 -B build/port2-scus94236 -DPORT_VERSION=usa && make -C build/port2-scus94236 -j8
build/port2-scus94236/tombi2 --data data_usa --headless --skip-movies --frames 4500 --pad-script 1000:start,...
```

The order relative to the PAL build does not matter (separate `build/scus94236/full` and `build/port2-scus94236`;
checked: USA alone, and PAL `build_full` + PAL port first, then the USA steps, both give 45 stubs). Expected:
`gen/report.txt` "stubs: 45", the full link prints no `does not compile` line. **The PSY-Q headers
`include/tomba/psyq` must be there before `build_full.py`** (git-ignored, copied by `tools/setup_native.sh`; a new
worktree does not have them: copy them from the main checkout). Without them the 28 `// HEADERS tomba26` files and
every file that includes their headers (611 USA units) fail to compile and the link keeps them as asm: every sha1
is still OK, but the port generated from that link lacks them (126 stubs: dispatchTasks, openTask, allocModelObject,
initObjectPools, DRAW_ENV_1...; crash at frame 0 in SetDefDrawEnv from initDisplay). That was the "126 stubs" report
of 2026-11-09 (a full link made without the headers, the port built after they were installed). Now
`build_full.py` refuses to start without `include/tomba/psyq`, writes `<full>/compile_failures.txt`, and
`port/tools/gen_port.py` refuses a full link listing any failed file (PAL too; `PORT_ALLOW_ASM_FALLBACK=1` overrides).

`PORT_VERSION` (CMake cache variable, passed to the generator as an environment variable; empty = PAL, the default
build, unchanged) selects `port/tools/portver.py`: it points `tools/units.py`, `tools/sysovl.py` and `tools/areas.py` at
the USA files under the PAL program names (MAIN0 = SCUS_942.36, X000 = SYSTEM/X00.BIN, X0nn = SYSTEM/Xnn.BIN,
D0SPSUB = SYSTEM/DSPSUB.BIN, ...), takes the units from `units("scus94236")` (plus the `// VERSION` files without a
symbol line: the message box 0x8002FA24) and the symbols from `build/scus94236/full`, and writes
`gen/version/port_version.h` (overlay base 0x800E7388, first area function 0x801151F4, boot file and sha1, 60 Hz,
GAME 0x8009BCC8) for the runtime. `gen_port.sources_version` builds every USA unit from its file, once per unit:
- the branches of the other versions are blanked first (`strip_versions`, also in the headers), so the fixes, the
  address resolver and the arity forwarding see only the USA code; the game C is compiled with `-DVERSION_SCUS_94236`;
- each copy of a template file gets the names its linked object (`build/scus94236/full/obj/<prog>__<ADDR>.link.o`)
  resolved: a name whose address is not the one the port links that name to becomes `name__ADDR`, or directly the
  native function at that address (`/* @ADDR */` keeps the PORT_ARG forwarding by address); a second definition of a
  name is `name__ADDR`, a template copy takes the USA function's own name (GetVideoMode from GetGraphType.c);
- a D_ name is never resolved by its number unless that number is its USA address; the static tables a file defines
  take the ELF's address, the scratchpad `.set`s (spad_remap applied) are one table;
- Psy-Q: the PAL library files and the USA-only files from 0x8005B38C on are left to the shims (a library unit with no
  name of its own takes the PAL name of its pair, e.g. SsSetAutoKeyOffMode); `symkind.txt` tells gen2's link which
  names are data (TITLE_MENU_GLYPHS sits at an X000 function start);
- main(): `src/main_scus94236.c` itself, renamed `port_game_main` (its frame wait calls `port_vblank`, port/fixes.txt);
  `runtime/main0_handwritten.c` / `x000_handwritten.c` (PAL units by PAL address) are PAL-only.

Runtime: the data folder's boot sha1 decides; a folder of another known release is refused with a message naming the
right build (both ways). `--lang`, `--text-patch`, `--picture ntsc`, `--cut-content`, `--civ-test` are PAL-only (ignored
with a message). The second-screen panel works (U3, below: "Second-screen panel"). Default 60 Hz; the play time counts 1/60 s natively (port_play_time_tick
off).

Checked (headless, i386): logos, title (NEW GAME / LOAD carousel), new game, AREA00: the 100 Year Old Wise Man's
dialogue, event title, HUD, walking, scrolling, enemies; report 0 unresolved literals, 45 stubs (PAL 148), 1 stub call
in 4500 frames (SpuInit). The PAL build generates the same files as before (report, syms.ld, data.c, funcmap.c, stubs.c
identical) and its frames are byte-identical over 3000 frames. Not checked: other areas, movies, saves, sound output,
the Windows / Android builds of USA.

Open (U3): `port/fixes.txt` lines that only hit inactive branches are listed at generation (`fix_audit`, 27 for USA:
widescreen culling 0x141, HUD position, the NTSC-picture emulation, which USA does not need); the runtime features
with PAL addresses (animlog, lang); the launchers (U2).

### Second-screen panel (stage U3, 2026-11-09)

`--second-screen` / `--dump-panel` work in the USA build (docs/PORT.md "Second screen"). What differs from PAL:
- addresses: `port2/runtime/panel_addrs.h` (79 `PV_*` macros, a USA and a PAL column), written by
  `port2/tools/gen_panel_addrs.py` from the PROVIDE tables of both full links as symbol + offset (GAME + 0x13, PLAYER +
  0xA6, MAP_NODES, EVENT_NAME_STRINGS, SUBSCREEN_COND_OPS + 0xB4...); the PAL column must equal the addresses the panel
  was written against, and `--check data game_usa/fs` compares the 14 tables that hold the same bytes on both discs
  (map graph, menu order, item tables, NOW LOADING sprites, sine table): all equal. RAM moves by 0xC98 (GAME block) /
  0xCA0 (objects), the sub-screen tables by 0xD08..0xF54;
- files: `SYSTEM/DSPSUB.BIN`, `DSPSUB3.BIN`; one page `SYSTEM/A00014.000` at (64, 0) holds PAL's `A10301` + `A00300`
  (item and event icons identical, the sub-screen font retouched); `A00003.GAM` = PAL `A10003.GAM`; NOW LOADING
  `SYSTEM/A00030.000` (= PAL `A00900`); the sub-screen sprite table inside `AREA19/A003.GAM` (+0x509C unpacked,
  tools/event_icons.py); the palette block at DSPSUB `SUBSCREEN_CLUT_IMAGE` (+0x962C);
- text: the sub-screen strings use ASCII - 32 up to '?' (0 = space) and ASCII - 33 from 'A' (PAL: Latin-1 + 140); no
  WFM files, the panel reads the WFM3 banks inside `AREA01/A008.GAM`, `AREA02/A003.GAM`, `AREA00/D003.GAM` (8 / 16 px
  dialogue banks, the 24 px title bank); the retouched glyphs are named by `tools/wfm_charmap_usa.json` (57 hashes,
  `tools/wfm_charmap_usa.py`: each USA bank against its PAL English twin, dialogue by dialogue), merged into
  `runtime/panel_charmap.h` after the PAL ones. Same glyph counts as PAL English (76 / 73 / 60);
- item tab: the 18 condition ops and the use handler are the USA natives (some built from another op's template:
  `subScreenCondPlayerH__800ED4F0` is CondPlayerD, `func_800E89F8` the handler); English only (`--lang` is PAL-only),
  the chrome says "Tomba".
Checked headless (i386): every tab (Status, Events, Map, Items) in AREA00 s0 and AREA01 s0, the NOW LOADING mirror
(identical to PAL), item use through the panel (Blackjack, Normal Pants: handler result 1), `--hud-bottom`. Against
the PAL renders 0.1-0.4 % of the pixels differ (the retouched glyphs), AREA00's map 17 % (its far backdrop is the
game's own NTSC frame). Top-down rooms (AREA05 s0, AREA02 s3; AREA02 s3 also without the panel) crash in the USA build
(`I0NFO2/3_overheadPlayerHandler` reads PAL 0x8009C963: a USA fix, not the panel). Not checked: the device, the
wasm2c / Android build of USA.


**U3 saves, debug menu, mods (2026-11-09).** Saves: the USA save is the PAL layout with 1/60 s play time, a 3-frame
icon (header byte 0x13) and the title digits 2 bytes earlier (docs/SAVE_FORMAT.md "USA saves"); the USA build writes
`BASCUS-94236TOMBA-nn` from the save screen and loads it back (checked headless, also after editing it with
`tools/tombi_save.py`), and the save tools read the time per product. Debug menu and mods: `port_version.h` now also
names `GVARS`, `PLAYER`, `SCRIPT_FLAGS`, `IS_DEBUG_MODE_ENABLED` (from the version's full link), `runtime/debugmenu.c`
uses them (GAME field offsets are the same in both releases), and `--poke` takes `NAME[+OFF]`; the launchers' mods
write named fields. Working in the USA build (headless): `--debug-menu` (noclip, fly, vitality, AP, item, all events,
warp, overlay; no Cut content line), the debug area select (`--poke 900:IS_DEBUG_MODE_ENABLED:1 --poke 1250:GAME:A:2
--poke 1250:GAME+2:0:2 --pad-script 1000:start,1300:circle:3`: AREA10 with the port's place names), start AP / lives,
infinite vitality / lives, unlock all events. Still PAL-only: `--title-debug` (the USA title carousel is
`titleDrawMenuCarousel_scus94236.c`, which the fixes.txt hook does not reach), `--cut-content`, `--civ-*`. The PC
launcher keeps the USA card in `saves_usa/` (as Android's `files/saves_usa`).

## Feature parity: fixes, room sweep, movies (stage U3, 2026-11-09)

**fixes.txt per version.** A line `@version usa` (any `tools/versions.py` name or alias, several allowed; `@version
all` ends the section) limits the fixes after it to that build (`gen_port.load_fixes`); the PAL fix set is the same
dict as before (checked), and the PAL generated sources are unchanged (same lines; only the pre-existing order of
`#define` lines varies between runs). The 27 audit warnings need nothing: the `PORT_NTSC(...)` picture fixes are what
USA draws natively, `buttonConfigDrawCursor`'s width is `--lang 5`'s, the `< 0x141` culls are the JP / demo branches
(USA spells `< 0x140`, which the PAL fixes reach). The audit cannot see PAL functions USA builds from another file (a
`_scus94236.c` port or a JP twin): grepping the generated USA sources for the PAL fixes' old text found them. The
`@version usa` section at the end of fixes.txt: widescreen HUD lives (y 208), the culls of the drawObjectSpriteRotScaled
port (0x80049994), AREA01/09 wrapping and AREA06/10 parallax backdrops, the Select popup's `port_ws_mark`, the event
title fly-in / fly-out (PAL `#ifdef PORT2` blocks of spawnEventTitle / l8Type01MoveStep), the `--title-bg` mark (USA
has no black band: `runtime/titlebg.c` puts the landscape at row 0) and the `--title-debug` DEBUG carousel entry (the
USA title font numbers letters from 'A' = 32, PAL 33: `PORT_TITLE_GLYPH_BASE`). `--launcher-exit` is PAL-only for now
(its words are drawn with the PAL disc's pause / title glyphs).

**Title "LDAI".** Not a bug: the carousel draws the entries off the front in perspective, 4 angle units a glyph, so
"LOAD GAME" right of NEW GAME shows its first letters squeezed ("LDA]"); the PAL build draws the same, and rotated to
the front it reads LOAD GAME.

**Room sweep.** `port2/tools/sweep_areas.py` takes the version from the data folder's manifest (USA: GAME 0x8009BCC8,
`build/port2-scus94236/tombi2`; the debug flag 1F8001B4 is the same byte). First USA sweep: 96 of 96 rooms with
findings; after the generator fixes below: **95 of 96 clean**, AREA10 section 3 a watchdog line under four parallel
jobs (clean alone; the PAL sweep had the same flake). PAL sweep of the same tree: 96 of 96 clean. Generator fixes
(`sources_version` / `area_plan`, USA only):
- two names of one address (LIST_COUNTER / LIST_COUNTER_S) were not made volatile (`volatile_aliases` had no
  address resolver): playerTryTalkToNpc looped past its list on Square (30 rooms);
- the arity of overlay / area / template definitions was looked up by the spelled name, not the native one
  (I0NFO2_overheadPlayerHandler, probeGrabPointInBoxA built as B): dispatchScreenEffect passed no object to the
  top-down player handlers (read 0x8009C963: AREA02 s0/s3, AREA05 s0/s2, AREA08 s0/s2, AREA19 s0/s2), and the
  NULL-call faults of AREA01 s1/s3/s4, AREA02/19 s0, probeGrabPointInBoxB in AREA00 s1/s2;
- an area's units start at its own code (AREA_OWN): the varying X00 prefix function playerAirSpinToFall (0x800EDFA0,
  also I0NFO2's topdownActorPatrolHStep) had "no native code" in every area (60 rooms);
- names X00's link gives to area functions (playerInitMode's FUN_801212a0 = X01 playerInitAreaHook) were linked as
  X000 image data: calls into RAM through the fault handler (AREA10: thousands per run);
- an area shares only X00's prefix: X00's own AREA00 functions made area data a function (X017
  WATERFALL_HINT_NPC_DEFS at a00TornadoStep's address, linked to a thunk: AREA17 s2-s4 faulted);
- a unit the full link kept as asm has no object: its area callees are now resolved from the version's symbols
  (playerClingJumpRelease, sevenFriendsMemberSetAnim / SetAnim were stubs). Stubs 45 -> 42.
Left: 156 emulated page-0 reads in playerPushOutOfObjectBox in 12 rooms (USA only; reads return 0, no fault), SpuInit
called once at boot (a Psy-Q unit left to the shims; its PAL C only calls the empty `_SpuInit`, ignored by the sweep).
`audit_pointers.py` runs on the USA gen (`PORT_VERSION=usa ... --gen build/port2-scus94236/gen`) but counts ~1340
`label` words per area: the USA area code ranges (gen2 `ranges_X0nn`) hold the area's own units only, not the X00
prefix, so the prefix's jump tables read as labels (harmless at run time, the sweep is clean).

**Movies / audio.** Headless with movies on: LOGO.STR, OP_INST.STR (title attract), BOY.STR (new game), and through
`--movie FRAME:ID` 100US.STR (id 3) and END_US.STR (id 20) decode with sound (`--dump-audio`: RMS 3000-9000 through
each movie; gameplay BGM ~3300). The USA ids: 0 OP_INST, 1 BOY, 2 MIST, 3 100US, 4 MCC, 5 NOZOITE1, 6 SORA1, 7 SORA2,
8 NOZOITE2, 9 MILLION, 10 HONOO2, 11 BUNMEI, 12 BUNMEI2, 13 HANA, 14 ARASHI, 15 MANTION, 16 BAKKASU, 17 JUNGLE,
18 KARAKURI, 19 MABUTA, 20 END_US.

**Renderers.** AREA00 (new game) and AREA03 s0 (debug select): software 4:3, `--widescreen 16:9` and OpenGL
(`--internal-res 2 --widescreen 16:9`, Xvfb) look as PAL does (HUD at the wide edges, event title, message box);
`--fps 120` headless: 2.0 presents per 60 Hz vblank (PAL 2.4 per 50 Hz), no repeated presents while walking.

## English text: USA vs PAL (2026-11-09)

Same English script with a light Americanisation / copy-edit pass (compared bank by bank, dialogue by dialogue):

- **Dialogue**: 5,317 dialogues in 21 banks on each disc, same count and index order (PAL `MFNTnnnB.WFM` = USA WFM3
  banks embedded in the area GAM/.000 files and `SYSTEM/DSPSUB*.BIN`). 5,243 identical (same glyph-id word streams),
  37 only Tombi→Tomba and UK→US spelling (favour, rumour, honour, catalogue...), 12 line/box/page breaks only (or with
  spelling), 21 real wording changes ("local travel bureau"→"local visitors' bureau", "you woke up!"→"you found me!",
  USA fixes PAL's "You're wire broke?"). No PAL-only or USA-only dialogue.
- **Event titles**: 9 reworded ("Rusty Steel Car"→"Rusty Iron Car", "The Greatest Smile!"→"The World's Greatest
  Smile!", ...). Item names identical; 24 PAL place names broken over two lines, not in USA.
- **Memory card screen** reworded ("No Tombi saves found"→"No block found for Tomba!", ...). Pause menu strings
  byte-identical (only y differs). Title/options text the same, other glyph numbering.
- **Fonts**: the same font retouched for USA (8 px 67/67 identical, 16 px dialogue 59/81, 24 px title 21/58), and the
  menus / sub-screens / memory card use other glyph numberings: no text file is byte-identical.

For translations: PAL English is enough as the source of every line (1:1 by bank and index); only ~80 lines and the 9
titles differ if a translation should follow USA wording. Building text for the USA disc needs its own encoder (banks
inside GAM, USA glyph bitmaps and numbering, NTSC positions).
