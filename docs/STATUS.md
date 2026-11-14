# Status and open tasks (hand-off between Claude Code sessions)

Update this file whenever something lands or a task is decided. Newest first.

## Last release

v0.3.1 (build of 66a8281, 2026-11-10): .zip / .7z disc images in both launchers, language flags on the cards, Disco and
home layout fixes, the Japanese card title オレっ！トンバ, release number in the PC launcher. Previous: v0.3.0 (5da1fc8).

Release number: `VERSION` (repo root) is the version the Android launcher shows (build.gradle versionName) and the
default of release.yml; bump it before each release (it used to show 0.1.<CI run number>).

## Matching status (2026-11-09): milestone, the three retail releases are 100 % matched

| Version | Code | Functions | Left |
|---|---:|---:|---|
| PAL SCES-01331 | 100 % of game code; Psy-Q library 100 % | all | nothing |
| USA SCUS-94236 | 100 % | 3681 / 3681 | nothing |
| JP SLPS-01144 | 100 % | 3528 / 3528 | nothing |
| JP demo SLPM-80165 | 55.13 % | 1381 / 1717 | hand passes (docs/JPDEMO.md) |
| Beta 980203 | 92.35 % | 1800 / 1837 | hand passes (docs/BETA_980203.md) |
| PrePre 8 PCPX-96084 | 32.34 % | 997 / 1425 | hand passes (docs/PREPRE8.md); same compiler as retail, ASPSX 2.34 |

Demo work 2026-11-10: tools/demo_pair.py (fuzzy twins over every version's C) + demo_sweep --fix in CI, then three
rounds of hand passes per demo. Checked at the end: PAL full link (26 sha1 OK), reports PAL/JP/USA 100 %, PAL port
sweep 96/96 clean, Python tests, CI exe + APK green.

Details: docs/REMAINING.md. The last PAL function (D0SPSUB 800EFD6C) replaced a wip draft that the port built: that
draft cleared HUD_STATE+0x74..0x76 (past the 0x68-byte HUD state) where the game clears +0x64..0x66.

## In progress / open

0. **Nintendo 3DS port (branch `claude/wizardly-heisenberg-5fllxj`, tested by the user on an Old 3DS, 2026-11-13)**:
   `--renderer pica` (n3ds/pica_c3d.c + shims/gpu_pica.c), 60 Hz with frameskip, the panel on the bottom screen
   (no map tab on an Old 3DS, the game's HUD there: `--hud-bottom`), improved Spanish translation stored on the SD.
   Start screen (n3ds/main_3ds.c, L held at boot): language, movies (skip = straight to the title),
   50/60 Hz, picture 4:3 / wide (`--widescreen 5:3`, experimental, off by default) / 3D (`--stereo`, experimental).
   Drawn in the look of the title screen's OPTIONS (runtime/launcher_game.c: the Tomba squares SYSTEM/A00801.GAM +
   A00802, green on the top screen and yellow on the bottom one, the 8 x 16 font A00804 + A00803, the green bar, the
   fixed texts in the LOAD screen's cream message boxes, the keys hint with drawn arrows and an A key drawn like the black Thor buttons, the title logo A00202 at half size, Start touchable; the disc is opened before the game and closed again,
   port_cd_close), the plain DejaVu screen (n3ds/launcher_ui.c) when the disc cannot be read; PC preview
   `PORT_LAUNCHER_PREVIEW=DIR` (with `--second-screen`).
   The OPTIONS screen's music plays under it (SOUND/SND_SYS.WVD + S_SYS00.SEQ through shims/sound.c, an SDL audio
   device of its own closed before the game's; PC check: PORT_LAUNCHER_MUSIC_WAV=FILE with the preview, its envelope
   matched the game's own options music, r 0.83, same level); moving an option or its value plays the disc's language
   menu cursor sound (its boot program's second VAB, sound 0: SsUtKeyOnV(23, vab, 0, 0, 8, 0, 100, 100)).  Not yet seen
   on the console.
   Measured on the console: gameplay 58-60 vblank/s in most rooms (the mansion AREA04 ~52: game logic 14 ms), Baccus
   wide 59 (tiles of the top-down map copied by 32 x 32 cells, not decoded again; on the 3DS ws_topdown.c draws no
   map windows beside the game's: 8 px of black at a map edge).  3D: the left eye is the game's picture, the right
   one the same batches drawn again by the GPU with a per-vertex shift (GTE SZ, else drawing order), in front of the
   screen too; ~52 in the first village with the slider up before the last present change (presents no longer wait
   for the GPU: not measured yet).  Open: the 3D's speed on the console; the PC syntax check for 3DS code uses
   devkitPro headers in a scratch folder (not in the repo).
0. **Widescreen room ends (2026-11-12)**: `runtime/ws_edge.c` carries a room's model on past its end (the free
   outward edges of the ground strips, risers and tree cards extruded along X with the room's own tiling; docs/PORT.md
   "Widescreen", "Room ends").  On: AREA00 section 0, left end (the sky strip left of the first hill at 16:9 / 21:9 /
   32:9).  Next: look at the other rooms with `--ws-edge all` (both ends; the right end of the first room is the way on
   to section 4 and was not reached headless without a room change) and add them to `ROOMS`; not built on wasm2c /
   the device yet.
0. **Widescreen top-down rooms: the wide frame stays inside the map picture (2026-11-12)**: at a map edge the game's
   camera clamp keeps only the 4:3 view inside the picture, so up to `pad` columns of black showed on that side (e.g.
   Dwarf Village's left edge, 53 px at 16:9). Fix `runtime/ws_topdown.c` `port_ws_topdown_begin` / `_end` (fixes.txt
   hooks in `drawGameplayScene`): while the scene draws, it moves dx columns (objects through `gte.c` `port_gte_dx`
   on RTPS / RTPT, the map through its packets, speaker balloons through `drawMessageBox`), dx putting the wide edge on
   the picture edge, or centring a picture narrower than the wide frame; culls keep pad + |dx|. Checked headless, i386
   and the 64-bit wasm2c PC build: AREA02/19 s0, AREA05/08 s0/s2, AREA19 s2 at 16:9 and 21:9 walking to the edges,
   every frame equal to the old frame moved by dx (pixel compare, objects/NPCs/Tombi with the map), the NPC dialogue
   balloon's tail on the speaker, a screen-positioned message box and the pause menu where they were, the hut
   interior (map hidden) unchanged; 4:3 and a side-view room at 16:9 byte-identical to before. Not checked: the
   device, USA/JP discs, `--fps` interpolation visually (runs); GL (xvfb) one room.
0. **Widescreen top-down maps drawn whole (2026-11-12)**: in the overhead rooms (I0NFO Dwarf Village AREA02/19 s0,
   I0NFO2 AREA05/08 s0/s2, AREA19 s2) the side strips past 4:3 filled in 32-px pieces as the camera moved (the user on
   the AYN Thor, `--widescreen auto`). Cause: the background is a 384x288 window of the map picture uploaded to fixed
   VRAM every frame, at most 32 px wider than 4:3 per side. Fix `runtime/ws_topdown.c` (fixes.txt hooks on the six
   map draw functions, PAL/USA/JP): the windows left / right are uploaded into the same VRAM at marker packets inside
   the ordering table (`shims/gpu.c` DrawOTag -> `port_ot_mark_fn`) and drawn, then the game's window put back; the
   game's window is cut at the picture's edge (it showed the wrapped next row). Checked headless, soft renderer: all
   seven sections at 16:9 and 21:9 (4:3 middle byte-identical to before in every frame, strips filled up to the
   picture's edge; the black past it is now gone, entry above), AREA05 on GL (xvfb); no widescreen: frames identical.
   Tunnels (I0NFO3 AREA02 s3) were already full width (3D blocks). Not checked: the device, USA/JP discs, `--fps`
   visually (runs).
0. **Proportional place names on the save list (default on, 2026-11-12)**: `runtime/mcprop.c` + port/fixes.txt hooks
   on `memCardDrawFileLocation` / `memCardDrawString` (PAL only): each glyph advances by its opaque width read from the
   font in VRAM + 1 px, the space 4 px (retail: 8-px cells). `--memcard-mono` restores the retail spacing,
   `--memcard-prop GAP[,SPACE]` tunes it. `check_text_patch.py` checks the place names against 229 px with the same
   widths. Spanish location 2 is "Cabaña del Anciano centenario" again (206 px). Headless PAL renders only.
0. **Mod "Saving does not refill vitality" (`--no-save-heal`, 2026-11-12)**: retail PAL/USA refill the vitality when a
   save is **loaded** (D0SPSUB2 `memCardFileListStep` sub 10, `GAME.playerHealth = playerMaxHealth` after the copy;
   PAL 0x800E8E30, USA 0x800E8A50), not when saving; JP never refills on load. port/fixes.txt guard + both launchers
   (docs/PORT.md "Mods"). Checked headless on PAL i386 only (vitality 1 saved -> 4 loaded without the flag, 1 with it;
   default frames identical); USA build and the device not run.
0. **JP retail port (SLPS-01144) boots and plays (2026-11-11)**: stage J1 of docs/PORT_VERSIONS_PLAN.md. `python3
   tools/build_full.py --version slps01144` (25 programs byte-identical), `cmake -S port2 -B build/port2-slps01144
   -DPORT_VERSION=jp && make -C build/port2-slps01144`, data from `python3 tools/extract_data.py game_jp/disc/tomba_jp.bin
   data_jp`. Headless the port goes logos → title → new game → AREA00 (the opening dialogue, the 100-year-old man event,
   HUD, fighting, walking; JP confirms with circle). What the JP build needed beyond the generator: the older libcd
   reader (`CdlReadN` + `CdReadyCallback` + `CdGetSector`, shims/psyq.c `cd_readn`), the older libetc pad
   (`func_80064B74` = PadRead polled by the vblank handler, fed by shims/pad.c `port_pad_read_libetc` through a
   `@version jp` fix), the JP overlay names in the JP branch of bootLogoState (`func_800EA048/60`, movie thread
   `func_8001D5E0_slps01144`), `SetRCnt/StartRCnt` removed in the JP frame wait; gen2: `asm("SYM+N")` externs as macros
   (clang / wasm2c), a native X000 function counts as a function start in a MAIN0 pointer table (the JP object table
   went data and trapped on wasm2c). Launchers (PC: supported, `tombi-pc-jp`, `saves_jp`; Android `libtombi_jp.so`),
   CI (`GAME_FILES_URL_JP` in port2-exe.yml / android-apk.yml, caches `fulljp-*` / `jplibs-*`), docs/JP.md "Native
   port". Open: the second-screen panel has no JP address column (`--second-screen` is refused in the JP build); the 7
   `.rodata overlaps` data units of the JP link; the Windows / Android JP builds are only built by CI (not run on a
   device yet).
0. **Wings glide in the port (2026-11-10)**: `--wings-glide JPDEMO_DIR --wings-pre9 PRE9_DIR` (its own switch, no `--cut-content` needed; PC launcher: Cut content page, own card with checkbox and the two demo disc images, the needed files are extracted into .wings/; the early-title field was removed from the launcher). `--cut-content` is now a pick list: `--cut-content=boatman,letter,board,berries,bubble,titles` (bits 1..32 in `port_cut_content`), the plain switch = all; the launcher shows one checkbox per item
   plays the JP demo's cut top-down Charity Wings animation (frames 77-79 + the feather item sprite spinning) in the
   wings exit and the landing of the three overhead engines (I0NFO2 Baccus Village / the pier, I0NFO Dwarf Village, I0NFO3 tunnels) (`port2/runtime/wings.c`, hooks in port/fixes.txt, docs/PORT.md). Checked headless in
   AREA08 s0 and AREA02 s0; Android launcher wired (Options > Cut content). Confirmed by the user on Windows and on the AYN Thor (2026-11-10); the Baron fix (5a750ccd2: the swapped tables are the area's own tables plus the three frames) confirmed by the user with Baron riding along (no duplicate Tomba). Side view too (e3ff5fefd): PrePre 9's orphan script (frames 282-284, fist raised) with the feather on the rise (playerEventStepRiseFadeOut) and the drop-in (playerEventStepDropIn), through `--wings-pre9`; checked headless in AREA00 / AREA02 s1. The user saw nothing in the side view with 0.4.0: both launchers extracted only `TOMBA/A00002.GAM` from the PrePre 9 image, not the `TOMBA/AREA01/A007.000` + `A008.GAM` the side set reads (fixed: the three files; the PC launcher re-extracts at start when a file is missing and the image is still set, Android asks for the image again). The rise then still showed retail's jump frames under the feather: `PlayerSetAnimIfChanged(o, 4)` runs every frame of the rise and reset the anim (guarded like the air spin, 0.4.2). Confirmed by the user on PC with 0.4.2: rise and drop-in in the side view, glide and landing in the overhead areas, Baron riding along. The full link (`tools/build_full.py`) had to be
   rebuilt first: a stale build/full made the port crash at frame 0 (initGlobalState).

0. **PrePre Vol.9 Tombi animations (2026-11-10)**: the demo's player frame table (TOMBA/AREA01/A007.000 = AREA02/D105.000,
   379 frames) holds 106 frames retail (AREA00/A004.000, 358) never got: 173 identical, 100 retouched, 106 new. Its player
   animation table (TOMBA/X01.BIN +0x380) has 56 ids against retail's 90, the same id is the same move in both (41 equal,
   5 retouched, 5 replaced: id 49 somersault -> roll land, id 33 dive-to-crouch -> crouch walk, id 37, idle/crouch
   single-frame -> animated). **20 orphan scripts** survive in TOMBA.EXE data (0x80011A50..0x800125E8, same region as the
   player scripts, nothing points at them): lift (112-114 / 115-120 / 121-124, three chained scripts), pull (255-258),
   punches (195-203), carry-walk (269-276, 277-281), 264-268, 238-240, 241-245, 298-300, 304-306, 317-320 + 321-326 loop,
   339-340, all `hold` ends with real tick counts. The lifted / pulled object is not in the demo: every AREA01/AREA02
   placement record of Pre9 has a retail twin at the same position (types renumbered, e.g. Pre9 L4 13 = retail L4 7), the
   texture pages differ from retail's only by retouches, and the log of A002 (4 rotation frames) is in retail AREA01 too.
   Pre8's PLAYER.WPP (313 frames, older art, palette in TOMBA.EXE +0x989A8) shares no frame with Pre9 or retail and has no
   script table (NPC tables only). Scratch scripts of the session were not kept; tools/anim.py got the CLUT row 16/17 fix.


0. **Port for the USA and JP discs**: staged plan in [docs/PORT_VERSIONS_PLAN.md](PORT_VERSIONS_PLAN.md) (U0 USA full
   link proof → U1 USA boots headless → U2 launchers/CI → U3 parity → J1 JP → demos). Open questions for the user there.
   **U0 done** (2026-11-09): `tools/build_full.py --version usa` links all 26 USA files byte-identical, name audit per
   program, 14 relocation/data bugs of the USA C fixed (docs/USA.md "Full link"). **U1 done** (2026-11-09):
   `cmake -S port2 -B build/port2-scus94236 -DPORT_VERSION=usa` builds a USA tombi2 that runs the USA disc headless
   through the title, a new game and AREA00 gameplay (docs/USA.md "Native port"); the PAL build is unchanged
   (generated files and frames identical). A USA full link made without `include/tomba/psyq` silently kept 611
   files as asm (port: 126 stubs, crash at frame 0); build_full now refuses that and gen_port refuses any link with
   C that fell back to asm (docs/USA.md "Native port"). **U2 done** (2026-11-09): PC and Android launchers are a game
   library (docs/PORT.md "Game library", `tools/game_library.py`, `Library.java`; `list` look by default, the user's
   pick), USA runs `tombi-pc-usa` / `libtombi_usa.so`, port2-exe.yml and android-apk.yml build them when
   `GAME_FILES_URL_USA` is set. **U3 in progress** (2026-11-09): the second-screen panel (docs/USA.md "Second-screen
   panel": `runtime/panel_addrs.h` per version; PAL panel PNGs identical), saves, debug menu, area select and the launcher
   cheats (docs/USA.md "U3 saves, debug menu, mods") work in the USA build; next: fixes.txt USA spellings, top-down rooms.

0. **Data bar of JP / USA** (2026-11-09): 0.94 % -> 83.74 % (JP), 0.97 % -> 90.88 % (USA); docs/JP.md "Data
   measure". Fixed the overlay base of the version data claims, the X00 prefix of the area overlays counted once,
   PAL data files reused where the version has them (tools/version_data.py), the rest as `src/data/<ver>/` files
   (`version_data.py gen`). Left: the sub-screen image / WFM banks (assets, as in PAL). Open: pointers as symbols in
   the generated files, per-array reuse of PAL files.

1. **NTSC picture (`--picture ntsc`, opt-in, experimental)**: top-down areas (AREA02/05/08/19) draw sprites/scenery
   out of step and movies flicker. An agent found the cause for top-down rooms (USA map strips start at the visible
   tile row, PAL 24 rows above; USA camera limits table MAIN0 0x8007B680; sprite cull 0xF1) but had not committed.
   The default picture is PAL at 60 Hz, so this does not block releases.
2. **FR/DE/IT/NL**: done on 2026-11-04 (no second disc needed). The SCES-01330 Rev.1 dump in `GAME_FILES_URL_SCES01330`
   is byte-identical to SCES-01331 Rev.1 except SYSTEM.CNF and the boot file name, so every PAL disc carries all
   nine MAINn.EXE and `--lang 1..4` already ran (lang.c). Now exposed: port usage, PC launcher (list, flags,
   launcher-language default), Android launcher (`LANG_N` appended). Checked on PC headless: in-game text in the
   four languages, panel names from `D<n>SPSUB`. Not checked: the device, the Android build. Open: the panel's own
   chrome is English for 1..4 (only en/es exist, `T()` in panel.c). Rev.0 (July 1998 build of either serial, MAIN0
   md5 3edd55b0) is not supported: 793 files differ from Rev.1 and the port reads overlay tables at Rev.1 addresses.
   Since 2026-11-04 the PC launcher, extract_data.py and the Android app name a Rev.0 disc as such (sha1 118459ce),
   and the messages say Rev 1 of SCES-01331 or SCES-01330 is needed. The SCES-01330 zip in R2 is Rev.1 again
   (2026-11-05): extracted with extract_data.py, the port ran 900 frames headless in French from it, launcher says
   "Compatible disc: SCES-01330".
3. Debug area select crashes, checked 2026-11-04 (i386 build, 3000 frames each): AREA10 sections 0-8 all run clean;
   AREA17 section 0 segfaults in `spawnSectionObjectsAlt` (empty load list, one of the 13 `DOOR_ROOMS` of
   tools/psxemu/rooms.py that crash in the real game too). Not a port bug; only the door transition from a sibling
   room enters those. Note: `--poke` values are hex (area 10 = `A`, 17 = `11`), easy to get wrong.
4. **Free text in the Select sub-screen fonts**: done 2026-11-04 (docs/PORT.md "Free text in those fonts").
   `--subscreen-labels` (port), the "Rótulos del menú Select" field of the PC launcher, our hand-drawn letters in
   `tools/subscreen_extra_glyphs.py`, `tools/subscreen_font.py` (TTF of both fonts from the owner's disc). Checked on
   PC headless only. Not exposed in the Android launcher yet (the port option works there too).
5. Trick Village Spanish name: user decision pending.

## Japanese decomp (SLPS-01144), hand pass 3 + SYSTEM overlays (100 %, 2026-11-09)

82.04 % of the code (docs/JP.md has the method, the JP systematic differences and every leftover with what was
tried). Pending, cheapest first: (a) `tools/jp_autofix.py` near misses in docs/JP.md "tried and left"
(scheduling/register puzzles: initGameConfig, updateL8Type01Object, updateParentFollowModel, drawBalloonFrame,
weaponHitCat2Type08); (b) a better pairer for the ~190 unmatched functions of 200-1000 bytes (masked words at 0.45
already swept: `scratchpad cand2`, rest are > 30 diff lines); (c) the structural big ones (updateSceneSequence,
updateFrog, blackCrowFlyAround, masterOfTheSkiesScene, the x10 seesaw, komaPigState0); (d) 28 functions >= 2 KB
with no PAL twin, to decompile from scratch. Demo SLPM-80165 at 40.87 % after `tools/demo_sweep.py`; BETA/PCPX only
sweepable in CI. Workflow per function: `tools/jpcheck.py --strict --norm`, `tools/jp_autofix.py --install-file`,
`tools/build_report.py --version slps01144`, `tools/progress_jp.py`. Pairing by callee names
(`tools/jp_pair_callees.py`, 2026-11-05) finds twins the masked-word pairer missed (contacts, attacks, event
handlers: 17 in one pass); a report line at 97-99.9 % that `jpcheck --norm` calls IDENTICAL is a PAL symbol name
that does not exist in the JP link: run `jpcheck --strict` without `--norm` and use the JP names (raw `D_`/`func_`
for overlay data and callees); three such lines (8001AA90, 8010D854, 801275C8) stay below 100 % after that, cause open. While `build_report` runs, `JP_TARGET_DIR=<copy of build/slps01144>` lets jpcheck
use a copy of the target objects (copy them before starting the build).

### JP sweep workflow

2026-11-09: `tools/demo_pair.py` (fuzzy twins from every version's C: opcode skeleton, callees, call graph) feeds
`tools/demo_sweep.py`; `--fix` adds scratchpad renames and runs in CI with `--budget 20`; PrePre 8 game code needs
`// ASPSX 2.34` (older assembler, `li` as `ori`). Demo figures and the residual-diff classes: docs/JPDEMO.md "Fuzzy
twins + fix pass". Open: the sweep checker hides `$at` store offsets (3 false matches removed by hand).

`.github/workflows/jp-sweep.yml`: on demand (workflow_dispatch) or when a push to master touches `src/**_slps01144.c`
or the JP/demo configs. Fetches the five discs, builds the reports, runs `tools/jp_autofix.py --install` on SLPS-01144
and `tools/demo_sweep.py --jobs 4` on the demos, and opens a PR (branch `jp-sweep/auto`) only when something was
installed. First run failed on the PrePre 8 image: that disc needs `tools/prepre8.py extract`, not `isoext.py` (fixed).
Second run (2026-11-04): SLPS-01144 0, SLPM-80165 0, beta 980203 +71, PrePre 8 +47, pushed to `jp-sweep/auto`; the
PR step failed because the repo did not allow Actions to create PRs; the user enabled the setting, PR #3 was opened
by hand and merged, and the third run (green) confirmed the PR step works. Open point: the CI demo report counts 1097 of 1832 functions, the local run of 7b8a7b3 counted 1098
(one function of 1788 bytes); no demo source changed, so compare the two reports when the demo disc is at hand.
Artifacts cannot be downloaded from the cloud container (the blob host is blocked by the proxy).
Since 2026-11-04 the sweep commits straight to master (the user approves every PR by default, so PRs are skipped) (the installer criterion is `--strict --norm`, so a match is real, but the file
placement may need a header fix).

### Data naming (checked 2026-11-04, nothing to do)

`tools/jp_globals.py --write` already gives every JP data address with a PAL name that name. The 148 raw `D_` globals of
the JP-only files split as: 53 pins inside PAL blocks (GVARS/GAME/GCD/GV6/DISPLAY_BUFFERS/PLAYER_POS fields, kept as
scalar externs on purpose: a struct access changes the address CSE and breaks the match; most of those fields are
unnamed in PAL too, `b42`-style) and 95 with no PAL twin. Naming data does not change the match figure.

### JP queue (one function per scheduled run; take the first open line, work it, then update this list)

Each run: `bash tools/session_bootstrap.sh --no-wasm --no-build` (fetches `game_jp/` from `GAME_FILES_URL_JP` and builds
`build/slps01144/report.json`), then on the chosen function: draft from the PAL twin (`tools/jp_autofix.py --pair
prog:addr:size:palfile --emit DIR`, or copy the PAL C and add the `// near-twin of PAL <name>` /
`// VERSION SLPS_01144 <prog> <addr> <size>` header), iterate with `python3 tools/jpcheck.py --norm DRAFT <prog>
<jpname>` (branch targets: `--strict --norm`), install with `tools/jp_autofix.py --install-file`, rebuild the report,
`tools/progress_jp.py`, update the progress line of docs/JP.md and this list, commit (explicit paths) and push.
Stop after about 90 minutes on one function: write what was tried under "tried" and leave it for the next run.

- [ ] x10 80119D9C / x14 80118580 (copies of updateA00FarBackdropWest with w1e 8 / 13): only the register of the
      object pointer (a3 in retail) and of the screen-origin halves differ (drafts wip25/80119D9C_b.c, 80118580.c).
- [ ] x00 8010D854 (playerStepSwimSpin x00 copy, src/func_8010D854_slps01144.c): jpcheck and a full objdump diff say
      identical, the report says 99.96 % (one word): probably unit size / trailing alignment, check with objdiff-cli.
- [ ] At one register / one scheduling spot each (drafts in the session scratchpad wip25/): x03 80126EC4
      wingedForkPigDefeated (v_80126EC4_b.c: the frame read lands in v1, retail v0), x03 80123E68 pig shot
      (v_80123E68_b.c: table base in v0, retail v1), x09 8012397C pickup object (v_8012397C_a.c: frame 32 vs 40 and
      the `li 16` placement), x10/x14 updateA00FarBackdropWest copies, x01/x09/x06 updateA00FarBackdropEast copies
      (8011FFFC.c: case 0 store order, 8011C2D8 w1e 9, 8011AA34 w1e 14 and anim by subtype).
- [ ] Next pairs from the scratch pair.py run (66-140 lines, real rewrites): x10 8011ED78 wingedForkPigContact (extra
      wb4 / subtype / b9c tests), x03 80126EC4 wingedForkPigDefeated, x00 80119278 spawnA00Tornado (extra global test,
      a/y stores reordered), x14 8011631C updatePoolType67Object, x00 80122B8C drawObjectProjectedSpriteB (type 44/45
      by subtype), x14 80116058 updateA00PoolType11, the three updateA00FarBackdropEast copies.
- [ ] 64-256 B band leftovers (see docs/JP.md "tried and left"): sound family 8001E2D4/388/458/528 (s1/s2 swap),
      8001E81C, 80027D58, pad readers 80016960/80020794, flag bits 800219F0/80021A40, x01 80131240, libcard
      80064E1C/E84. The rest of the band is libgte/libapi asm (RotMatrix*, ApplyMatrix*, OuterProduct*, SquareRoot0,
      _patch_*, start, __main, *_OBJ_* blobs, 8005851C BIOS stubs, 80061488).
- [x] x04 func_80124270: matched (redBirdStep3 form, see docs/JP.md "Inline pairer").
      the `% 40` timer; cases 1-4 need rewriting. The PAL file is full of matching hacks: keep them.
- [ ] x10 func_8011B4F8 (2356 B) <- src/x001/playerStepRideSeesaw.c: the PAL file is register-pinned and the JP reads
      the seesaw record as shorts at +0x30 (`lh 48` for `lw 140`); needs a rewrite from the disassembly.
- [ ] x00 func_801249EC weaponHitCat2Type08 (256 B): structure right, the temporary lands in a0/v0 instead of v1
      (draft: scratch wip25/801249EC.c of this session; see docs/JP.md).
- [ ] slps01144 func_80024B78 (5112 B) and func_80014D1C (4096 B): handwritten GTE routines (the lit modelDraw
      family); write them as C with the GTE macros of src/modelDrawG4Lit.c, instruction by instruction.
- [ ] x04 func_80115FD0 (6472 B), x00 func_800F4CDC (5464 B), x10 func_8011DCC8 (3872 B): no PAL twin, from scratch.

## Recently done (context)
- PAL X000 at 100 % (2026-11-08): the A00_WOODEN_BRANCH_HANDLERS subtype 1/2 handlers func_80132BB8 / func_80132DD0
  (only port2/runtime/x000_handwritten.c had C for them), matched with the JP-round tricks (a `static __inline__`
  for the bend body moved the last register). Checked with tools/ncheck.py (no dosbox for matchcheck here). The
  port's hand-written copies may now be redundant: not touched. Then D0SPSUB 800E89F8 (4280 B) matched with a
  volatile asm copy of the `Pal *t` table pointer (hides its constant value from cse, which had swapped the addu
  operands). Whole game 99.68 %; left: D0SPSUB 800EFD6C (786), D0SPSUB2 800E999C (178, h must outrank the block-copy
  pointer), D0SPSUB3, and Psy-Q.

- Area names from the game's map in the area select (`port_mods.py`) and the save editor: `save_import.py`
  ROOM_NAMES_EN/ES + AREA_ROOMS (every disc area / section -> map area and room, improved Spanish applied),
  `place_name()`; the hand-made lists are gone (STATUS point 4 of 2026-11-03).

- Map graph across builds (2026-11-04, `tools/map_tables.py` + `map-tables.yml` on the GAME_FILES_URL_* secrets):
  PAL = USA = beta 1998-02-03: 65 nodes, 55 links, 115 route entries, node 41 (-269, -974, -40, in the sea west of
  Mushroom Forest) referenced by nothing. JP retail: the same first 55 nodes and links; PAL/USA/beta add the
  west-coast strip (nodes 55-63, region 10) and node 64 (region 7). PrePre Vol.8, PrePre Vol.9, Dengeki D5 and the
  SLPM-80165 demo: no node table, no links table and no retail signature anywhere on the disc (their DSPSUB.BIN is
  about 48 KB against 162 KB retail): the pause map graph did not exist yet in the 1997 demos.

- PrePre Vol.9 item / event text (2026-11-04, `jp-demo-text.yml`, disc PREPRE9, whole dump in the job log): the
  1997 demo has no sub-screen name table; its item names live in the "<item>をつかった" ("used <item>") lines of
  the WFM1 block 0 of AREA01/A008.GAM (AREA02/A003.GAM holds the same block), 37 items in retail order, and the
  event titles in block 1 (30 titles, 18 glyphs of that block not in jp_charmap.json yet). Two items are not in
  any retail build: ケガをしたコイヌ (injured puppy) and はやさレベル＋１ (speed level +1); the rest match JP retail.

- Demo item icons (2026-11-04, `tools/item_icons_find.py` + `item-icons.yml`; `--cells` looks for a demo's own cells
  on another disc): PrePre Vol.9 keeps them in TOMBA/A00012.000 (1536 px wide page, retail 15-column grid from x 512),
  41 of 46 cells pixel-identical to the retail icon of the same id; new art in cells 35 (pig snout = the demo's Baby
  Pig), 36 (orange curly key) and 44 (corkscrew). Dengeki D5 (TOMBA/SYSTEM/A00012.000) has 61 retail cells plus
  snout (24), pod (34), key (36), corkscrew (44), wine bottle (45), white powder (52), grey rocks (61): the community
  "demo exclusive" row. Its description bank (SYSTEM/A00013.000, 201 entries in retail id order) already calls 36 /
  44 / 45 the 1,000 / 10,000 / 100-million-year keys and 52 / 61 Rain Essence / Iron, so those icons are older art
  of the same slots; no demo has any text about a cork or wine. Item 64 is described there as "a valve that doesn't
  leak". The disc's own ITEM_ICON_CLUTS are not found by the retail prefix (renders use the retail row of the slot).
  Dengeki text (jp-demo-text, DENGEKI_D5): the event hint bank (A00014, 200 entries) is in retail event-id order up to
  ~124 (later ids are "???"); hints at ids 26, 83, 87, 88, 112, 115, 120, 121, 172, 173 describe things no retail text
  has (secret spell, a dinner party feast, melting something to open a path, rolling down as a snowball, a hidden
  power that stops you moving, words that make something move, winning a prize, "a forgotten thing is a memory?",
  Santa's parasol, "what's that!? bananas"). AREA00/A006 #59/#64: the 100 Year Old Wise Man offers to appraise items
  whose use you don't know (not in retail). `bash tools/disc_fetch.sh DENGEKI_D5` puts the disc in game_dengeki_d5/.
- Dengeki D5 vs PAL, code and objects (2026-11-04, disc in game_dengeki_d5/ via disc_fetch.sh, scripts kept out of the repo):
  xmatch finds 735 PAL functions in TOMBA.EXE (31 % of its code, 111 identical) and 151 in AREA00/X00.BIN (overlay
  base 0x800F4A48, from its jal targets). fn_vs_pal: the village NPC walkers are the retail ones (villagerNpcWalkPath
  81 %, villagerNpcSpringMotion 78 %), so no other patrol system; the hooded old man handler (layer 3 type 0x15 in
  this build, 2704 B) is only 48 % similar to updateHoodedOldMan. Load lists are the PrePre 8 layout (20-byte
  records after the file table at exe 0x5D31C), no APD kinds: the object placement lists live inside D003.GAM
  (= D103 = D203) at +0x481CC/+0x488DC/+0x490DC (rooms 0-2) and +0x4962C (area list), D302 +0x5CC, D503 +0x11784,
  same 16-byte records as PAL. By bytes 300 of 422 PAL AREA00 records are there; by position the only objects PAL
  does not place in the village: five layer 4 type 8 "static solid" sprites (handler at X00+0x28E60, the
  cullObjectToDrawList(360) variant of updateStaticSolid; PAL emptied that slot) at (1021,-120)/(1117,-197)/
  (1198,-251) in room 0 (sprite frames 46-48) and (2546,-476)/(2612,-370) in room 1 (frames 53-54); two layer 7
  type 0 and four type 1 effects in room 0 (y 144 / 260); one flying bug (layer 2 type 0x27) in the area list at
  (2642,-300,168); plus re-placed foliage (type 0x24), backdrop sprites (0x14) and climb props (0x0A). The handler
  tables are at exe 0x8006A3E8 (layer 4, 49 entries), 0x800679B8 (layer 2, 50) and 0x80069D64 (layer 3, 58).
  Not done: rendering those five sprites (the village SPR bank was not located; A006 is a 16-bit page).
- Hyper PlayStation Re-mix No.12 (SLPM-80167, build 1997-09-22; secret / variable GAME_FILES_URL_HYPER12, 2026-11-04):
  the missing link between PrePre 8 and the October demos. Same file layout as PrePre 8 (DATA000-004, TIM000-011,
  PLAYER.WPP, single TOMBA.EXE with the test menus, PLAY DEMO / RANKING DEMO / TO BE CONTINUED) but already with WFM
  text (DATA000: 64 dialogues + 16 event titles), the three sub-screen overlays (DSPSUB*.BIN) and a new COCKA.WPP
  (82 frames: the Kokka bird; PrePre 8's "grey bird" is this). Load lists: 20-byte PrePre 8 records at exe file
  0x681E0 (system) and 0x68604 (area 0): lists 1-3 village (DATA001, TIM004/005 + CLUT01-03), 4 the 2D interior
  (TIM007 + DATA002 collision, still there), 5 the street (DATA003: WHM 2140 then WMD at +2140, 77 KB, 1137
  polygons; PrePre 8 had 1001) and 6 the house (DATA004: WMD at +3420, 983 polygons; the pig sign of PrePre 8 is
  gone). Renders made with tools/room.py + jpdemo_rooms.parse_wmd_demo from those lists (nothing committed).
  Text: all 64 lines have a close JP retail twin except the place names and the 100 Year Old Man's two appraisal
  lines (so the cut appraisal mechanic spans Sept-Nov 1997). The hungry monkey introduces himself as サンタ (Santa):
  the Dengeki hint "Santa's stylish parasol" is the monkey, renamed Charles later. Kid at the pond: "can you swim?
  the turtle helped you this time, beyond that door you must swim" (the swimming gate of the big puddle). Event
  titles include 大きな水たまり？ (unused retail event 6), コッカタマゴの中身, オイラ泳げない…, かくれ里のヤン.
  Code: xmatch 643 PAL functions in TOMBA.EXE (25 %). Rooms 4/5 (lists 5/6): the PrePre 8 exit triggers (x >= 0x4A2
  in room 0, 0x4EC..0xA46 in room 1, x < 0x51 / 0x29 back) are in the Hyper player controller too (0x8002D81C..
  0x8002DB00), but the PrePre 8 entry-record table (x, y, mode, area, room, entry) was not found in the same shape
  and func_8002816C has no masked twin, so reachability is likely but unverified (no emulator run).
  Discs now live in game_<name>/ (tools/disc_fetch.sh): /dev/shm is wiped when the idle container restarts.
- Demo event triggers vs retail (2026-11-04): the demos' setEventStarted / setEventComplete are not masked twins of
  PAL's (Dengeki 0x8001C25C / 0x8001C37C, Hyper 0x8001D414 / 0x8001D510, found through fn_vs_pal + the l3 wise-man
  handler's calls). Ids with an immediate: Dengeki starts 2, 4, 8, 37, 166, 169, 182 and completes 2, 3, 5, 166;
  Hyper completes 2, 5. Against every PAL immediate (MAIN0 + all overlays) the one trigger retail lacks is
  **event 4 "Motocross Course" started from the village scroll controller** (Dengeki X00 0x80116730, 1456 B, 25 %
  like updateA00ScrollController): a state machine on the player position (0x1F80016A/16E) with the PrePre 8 room
  exits (x >= 1186, y >= -60; 1261..2630 / y < -149) and, high in the village (y < -1369, scroll word 0x1F800172
  == 450, x < 2188, then a flag at 0x1F8001CC), setEventStarted(4, 0, 1). Retail starts 4 in AREA03 (X003).
  Follow-up: that top zone is the hut door (dispatchAreaInitB sub 4 at 1987,-1379) and sign sub 7 (2327,-1356), both
  placed in PAL too; the trigger stops the BGM (0x8001E7D4 / 0x8001E724, SsSeqStop-like), creates a thread (task
  0x8002CF40) and only then starts event 4: it is the demo's stand-in for the climb to Phoenix's Mountain, whose
  area is not on the disc (the wise man's "this version ends here" lines; the TO BE CONTINUED card is drawn by
  0x8001B904, reached through a table, no direct caller). Not a lost location. Dengeki hint 166 names the monkey
  サンタ, confirming Santa = Charles. Event 5
  / 166 are completed in the same scenes as retail (a00DarkCloakedFigureScene, Charles talk) but by register there.
  Patrols: villagerNpcWalkPath / SpringMotion in the Dengeki are a different revision (word-aligned diff useless),
  not a constant tweak; no other movement code without a PAL counterpart was found. Map tables: none in any 1997 demo.
- Game-file downloads (2026-11-04): every workflow and the bootstrap go through `tools/ci_download.sh`, which takes
  direct / Dropbox / Drive / public MEGA links (megadl). Drive hit its quota on the PAL, JP and Dengeki files that
  day; the owner moved PAL, JP and Dengeki to MEGA links.

- psx_tomba cross-check (2026-11-05): their JP symbols gave us 29 names for generic `func_*` functions (files
  renamed, e.g. `src/CdStatus_slps01144.c`) and 54 missing function symbols paired with PAL by name; on top of the
  sweep's 85.03 % the JP report is 85.05 % (2779 of 3102 functions, +13). 23 naming disagreements kept on our side (notes/psx_tomba_jp_names.md).
  Their overall progress is below ours (SDK 88 %, main 78 %, overlays 77 %); LICENSE claims MIT but the file 404s.
  Our LICENSE is now the plain MIT text (the scope note moved to the README) so GitHub identifies it.

- Bootstrap check (2026-11-05, a full run from a restarted container): two fixes. `tools/session_bootstrap.sh`
  installed wasi-libc only when clang-18 was missing (the image ships clang-18 without it: every wasm file failed,
  exit 0); now each piece of the wasm toolchain is checked, and a failed wasm build is reported in the log. And
  `port2/wasm/patch_w2c.py` forwards a `.Lname_bitcast_invalid` stub to the function under its real wasm2c name
  when `name` is only an alias (wasi-libc's ftello / fseeko are __ftello / __fseeko: `w2c_tombi_0x5F_ftello`); the
  rebuilt build/wasm/tombi2w ran 600 headless frames. Python tests, the patch validator and the i386 smoke test pass.

- PC window fit (2026-11-05): a window dragged to a size that is not within 1/16 of an integer multiple of the
  picture now gets the aspect-keeping fractional fit instead of floating at the integer scale inside borders
  (`port_window_fit`, sdl.c; `--fit` still forces the fractional fit always). Checked under Xvfb with xdotool: 682x512
  window -> 682 px wide picture (2x), 953x1000 -> 953 wide, 700x520 -> 682 wide (still 2x, small borders).

- Panel (c8529a5): event detail page redone as the Items showcase (icon 3x on stone, title 16 px + state; Dónde =
  plate / area / room lines + "Ver en el mapa" when the marker is in the loaded room (Map tab centred on it, zoom 2);
  Quién only when it adds something, else Pista, else no window; planks Volver / Seguir / Ocultar; no route graph,
  no face, no key help on Android). One shared scroller (`Scroll`, panel.c) for the Events lists, the Items grid and
  the warp destinations: pixel drag, time-based fling, clamp, indicator, no paging; `drag@...@N` spreads a swipe
  over N vblanks. The Map tab still only shows the loaded room (events elsewhere get no map plank).
- Android arm64 support through wasm2c; SDL/JNI calls on the thread's own stack; panel canvas handed to Android
  unscaled; short/char returns sign-extended (fixed vanishing scenery at the bending branch).
- `--fps auto` turns interpolation off when the display is not faster than the game (fixed movie flicker on 60 Hz
  screens).
- Panel: Items page = showcase + scrolling icon grid (double-height action plank, 24 px label); Status = 2x2
  dashboard; Events list one thing per line with place plates; improved-translation letters (glyphShift undone),
  built accented capitals, Á/ª decoding; side map drawn once into cached tiles.
- Save import: replace (asks, backs up the card, keeps last 5 in saves/backup), "Guardar en otra ranura"; the port
  emulates only memory card 1 (old card 2 saves merged once); launcher dialogs drawn in the launcher style.
- Spanish "Objetos"/"Usar objeto" labels built from the disc's own Swedish/Spanish letters; SV/DA Select crash fixed.
- Translation: Garfio → Gancho; caged leaf-butterfly dwarf rewritten from the JP/EN originals.
- GitHub wiki (afsenovilla/Tombi-Decomp/wiki): the session cannot push to it; the user pastes pages
  (`docs/wiki/Home.md`, `Builds.md`).
