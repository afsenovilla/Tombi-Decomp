# Status and open tasks (hand-off between Claude Code sessions)

Update this file whenever something lands or a task is decided. Newest first.

## Last release

v0.1.2 (build of b4d6406): vitality fruit numbers 8-16, warp destinations one per line, event detail page redone,
shared touch scroller for every long list, Windows tolower link fix.

## In progress / open

1. **NTSC picture (`--picture ntsc`, opt-in, experimental)**: top-down areas (AREA02/05/08/19) draw sprites/scenery
   out of step and movies flicker. An agent found the cause for top-down rooms (USA map strips start at the visible
   tile row, PAL 24 rows above; USA camera limits table MAIN0 0x8007B680; sprite cull 0xF1) but had not committed.
   The default picture is PAL at 60 Hz, so this does not block releases.
2. **FR/DE/IT/NL**: done on 2026-11-04 (no second disc needed). The SCES-01330 Rev.1 dump in `GAME_FILES_URL_SCES01330`
   is byte-identical to SCES-01331 Rev.1 except SYSTEM.CNF and the boot file name, so every PAL disc carries all
   nine MAINn.EXE and `--lang 1..4` already ran (lang.c). Now exposed: port usage, PC launcher (list, flags,
   launcher-language default), Android launcher (`LANG_N` appended). Checked on PC headless: in-game text in the
   four languages, panel names from `D<n>SPSUB`. Not checked: the device, the Android build. Open: the panel's own
   chrome is English for 1..4 (only en/es exist, `T()` in panel.c); the earlier Rev.0 dump crashed the port (its
   code differs from Rev.1), so an SCES-01330 Rev.0 player would need the Rev.1 code: not supported.
3. Debug area select crashes, checked 2026-11-04 (i386 build, 3000 frames each): AREA10 sections 0-8 all run clean;
   AREA17 section 0 segfaults in `spawnSectionObjectsAlt` (empty load list, one of the 13 `DOOR_ROOMS` of
   tools/psxemu/rooms.py that crash in the real game too). Not a port bug; only the door transition from a sibling
   room enters those. Note: `--poke` values are hex (area 10 = `A`, 17 = `11`), easy to get wrong.
4. **Free text in the Select sub-screen fonts**: done 2026-11-04 (docs/PORT.md "Free text in those fonts").
   `--subscreen-labels` (port), the "Rótulos del menú Select" field of the PC launcher, our hand-drawn letters in
   `tools/subscreen_extra_glyphs.py`, `tools/subscreen_font.py` (TTF of both fonts from the owner's disc). Checked on
   PC headless only. Not exposed in the Android launcher yet (the port option works there too).
5. Trick Village Spanish name: user decision pending.

## Japanese decomp (SLPS-01144), hand pass 3

82.04 % of the code (docs/JP.md has the method, the JP systematic differences and every leftover with what was
tried). Pending, cheapest first: (a) `tools/jp_autofix.py` near misses in docs/JP.md "tried and left"
(scheduling/register puzzles: initGameConfig, updateL8Type01Object, updateParentFollowModel, drawBalloonFrame,
weaponHitCat2Type08); (b) a better pairer for the ~190 unmatched functions of 200-1000 bytes (masked words at 0.45
already swept: `scratchpad cand2`, rest are > 30 diff lines); (c) the structural big ones (updateSceneSequence,
updateFrog, blackCrowFlyAround, masterOfTheSkiesScene, the x10 seesaw, komaPigState0); (d) 28 functions >= 2 KB
with no PAL twin, to decompile from scratch. Demo SLPM-80165 at 40.87 % after `tools/demo_sweep.py`; BETA/PCPX only
sweepable in CI. Workflow per function: `tools/jpcheck.py --strict --norm`, `tools/jp_autofix.py --install-file`,
`tools/build_report.py --version slps01144`, `tools/progress_jp.py`.

### JP sweep workflow

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

### JP queue (one function per scheduled run; take the first open line, work it, then update this list)

Each run: `bash tools/session_bootstrap.sh --no-wasm --no-build` (fetches `game_jp/` from `GAME_FILES_URL_JP` and builds
`build/slps01144/report.json`), then on the chosen function: draft from the PAL twin (`tools/jp_autofix.py --pair
prog:addr:size:palfile --emit DIR`, or copy the PAL C and add the `// near-twin of PAL <name>` /
`// VERSION SLPS_01144 <prog> <addr> <size>` header), iterate with `python3 tools/jpcheck.py --norm DRAFT <prog>
<jpname>` (branch targets: `--strict --norm`), install with `tools/jp_autofix.py --install-file`, rebuild the report,
`tools/progress_jp.py`, update the progress line of docs/JP.md and this list, commit (explicit paths) and push.
Stop after about 90 minutes on one function: write what was tried under "tried" and leave it for the next run.

- [ ] x00 func_80101F30 (1620 B): new JP player step called from case 0x39 of playerNormalModeDispatch; nearest PAL
      playerStepWalkIntoDepth (0.22 callee overlap). Decompile from scratch using the PAL step files as idiom reference.
- [ ] x04 func_80124270 (2904 B) <- src/x004/blackCrowFlyAround.c (334 diff lines): JP drops the `w22` counters and
      the `% 40` timer; cases 1-4 need rewriting. The PAL file is full of matching hacks: keep them.
- [ ] x03 func_80128CCC (2432 B) <- src/x003/masterOfTheSkiesScene.c (205 lines): case bodies reordered, extra
      player-position checks.
- [ ] x02 func_8011794C (2304 B) <- src/x002/updateSceneSequence.c (34 lines but structural: 23 script opcodes
      instead of 18, extra init in case 0).
- [ ] x10 func_8011B4F8 (2356 B) <- src/x001/playerStepRideSeesaw.c (138 lines, another record layout).
- [ ] x04 func_80121504 / x03 func_80122250 (1.8 KB each) <- komaPigLeap / updateA00L2Type2C siblings (~100 lines).
- [ ] x00 func_801249EC weaponHitCat2Type08 (256 B, 2 lines left: where the shared tail starts; see docs/JP.md).
- [ ] slps01144 func_80024B78 (5112 B) and func_80014D1C (4096 B): handwritten GTE routines (the lit modelDraw
      family); write them as C with the GTE macros of src/modelDrawG4Lit.c, instruction by instruction.
- [ ] x04 func_80115FD0 (6472 B), x00 func_800F4CDC (5464 B), x10 func_8011DCC8 (3872 B): no PAL twin, from scratch.

## Recently done (context)

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
  whose use you don't know (not in retail). To hunt beyond text (patrols, handlers) the disc has to be in the
  container: GAME_FILES_URL_DENGEKI_D5 is a CI secret only, not an environment variable.
- Game-file downloads (2026-11-04): every workflow and the bootstrap go through `tools/ci_download.sh`, which takes
  direct / Dropbox / Drive / public MEGA links (megadl). Drive hit its quota on the PAL, JP and Dengeki files that
  day; the owner moved PAL, JP and Dengeki to MEGA links.

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
