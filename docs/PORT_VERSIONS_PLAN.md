# Plan: running the USA and JP retail discs (and later the demos) in the native port

Status on 2026-11-09: PAL SCES-01331/01330, USA SCUS-94236 and JP SLPS-01144 are 100 % matched from one `src/` tree.
`port2/` runs only PAL. This plan was made from reading the code; nothing in it has been built or run yet. Lines
marked *(guess)* were not checked.

## 0. Findings that shape the plan

1. **The port is PAL-only at generation time, not just at run time.**
   - `port2/tools/gen2.py:39-56` hardcodes the PAL images and layout: `game/MAIN0.EXE`, `game/AREA00/X000.BIN`,
     `X000_LOAD 0x800E8028`, overlay end `0x80140000`, heap `0x80140000..0x801FD800`, hiram `0x801FD800`.
   - `port/tools/gen_port.py` hardcodes `OVL_BASE, OVL_END` (~339) and the first area word `0x80115EA4` (403-405).
   - Every name and address comes from the PAL full link: `check_full_fresh`/`provides()` read
     `build/full/*.full.ld` (266-306), `object_refs()` reads `build/full/obj`.
   - `tools/units.py:28-35` hardcodes the PAL ranges and `game/AREAnn/X0nn.BIN`; `tools/sysovl.py:13-23` the PAL
     SYSTEM names and sha1s.
2. **USA and JP have no full link.** `tools/build_full.py:50-71` builds only the PAL programs (no `--version`);
   docs/JP.md:557 "Not linked yet ... compared by objdiff only". The objdiff reports run with
   `"functionRelocDiffs": "none"` (`tools/build_report.py:377, 546`): **"100 %" does not prove that relocation
   targets (data and callee addresses) are right**, and the port depends on exactly those.
3. **USA has no data symbols**: `config/scus94236/symbol_addrs_scus94236.txt` holds 1331 lines, all `type:func`.
   JP has them (`GAME = 0x8009E508 // PAL 0x8009C960`, written by `tools/jp_globals.py`, JP-only).
4. **How a version builds**: a version function is a unit at the version's address whose C comes from PAL files
   paired through `config/<ver>/symbol_addrs_*.txt` `PAL <mod> <addr>` comments, `config/<ver>/reuse.txt`, or
   `// VERSION <SERIAL> <prog> <addr> <size>` files (283 USA, 1108 JP), resolved by `tools/build_report.py:61-118`
   (`version_lines`, `jpdemo_sources`, `version_source`). gen_port must reuse this resolver, not `units()`.
   `gen_port._blank_versions` (801-821) keeps the PAL branch; it has to keep the target version's branch.
5. **Port fixes and hooks were written against PAL branches.** `port/fixes.txt` has 867 lines on 212 files; 41 of
   them have `VERSION_SCUS_94236` branches and 53 `VERSION_SLPS_01144` branches (drawModel*.c, isQuadOnScreen.c,
   drawHud.c, initGraphics.c, loopTitleScreen.c, d0spsub2/memCardScreenStep.c, x006/drawRaceHud.c...). A fix applies to
   every occurrence; if the version branch spells the text differently it silently lands only in the PAL branch.
   24 `src/` files carry `#ifdef PORT2` hooks (`port_play_time_tick`, spawnEventTitle.c, drawMessageBox.c,
   loadMessageGlyph.c...); only one USA-only file has one.
6. **Hand-written runtime C uses PAL addresses.**
   - `port2/runtime/main.c:142-206` `game_main` is PAL's `main` (0x800163F4). USA's `main` is matched C
     (`src/main_scus94236.c`, 0x590 B); JP's is 0x28C B.
   - `port2/runtime/main0_handwritten.c` translates PAL asm units bound **by address**
     (`gen_port.handwritten_port2_funcs()` 942-952): under USA/JP they would hijack unrelated functions → PAL-only.
   - Raw retail literals: `PORT_M0(0x8009C960u)` etc. in `shims/psyq.c:498-516`, `runtime/main.c:510-565, 744-746`,
     `debugmenu.c` (13), `panel*.c/.inc` (~90), `animlog.c` (23), `lowmem.c:161`; layout constants in
     `port_runtime.h:38-40`; `CdRead` relocation below `0x80140000` and `cd_last_id < 20` (`shims/psyq.c:1159-1165`).
7. **Disc layouts** (USA checked in `game_usa/fs`):

   | | PAL | USA | JP (docs/JP.md:33-52) |
   |---|---|---|---|
   | Boot exe | `EXE/MAIN0..8.EXE` | `SCUS_942.36` (sha1 81cbc79f…, 0x88000 B) | `SLPS_011.44` (07d57769…, 0x8E000 B) |
   | Area overlays | `AREAnn/X0nn.BIN` | `SYSTEM/X00..X19.BIN` | `SYSTEM/X00..X19.BIN` |
   | Overlay base | 0x800E8028 | 0x800E7388 | 0x800E9AF8 |
   | Shared X00 prefix | 0x2DE7C | 0x2DE6C | 0x2B680 |
   | SYSTEM code | `D0SPSUB*`, `I0NFO*`, `O0PTSUB0`, `G0OVER` | `DSPSUB*`, `INFO*`, `OPTSUB00`, `GOVER` | as USA, **no GOVER** |
   | Load lists | `SYS/LD0SYS.BIN`, `LD0ARnn` | `SYS/LDSYS.BIN`, `LDARnn` | *(guess: as USA)* |
   | Movies | `MOVIE/*.STR` | adds `100US.STR`, `END_US.STR` | *(not checked)* |

8. **Disc checks expect PAL `EXE/MAIN0.EXE`**: `tools/extract_data.py:29-30`, `tools/port_launcher.py:420` and
   `check_disc` (2858; `KNOWN_DISCS` 438-441 already names SCUS-94236 and SLPS-01144), `port2/android/disccheck.c:104,
   300`, `LauncherActivity.java:54-55, 550-553`, `shims/psyq.c:898-921`, `.github/workflows/port2-exe.yml:154-183`.
9. **The data image is compiled into the binary** from the builder's disc (`port2-exe.yml:6-8`, gen2 `link`): each
   version's binary needs that disc at build time (secrets `GAME_FILES_URL_USA` / `_JP` exist).
10. **Timing**: PORT.md "60 Hz mode" shows the PAL logic is the USA logic frame for frame. The USA build divides play
    time by 60 natively, so `port_play_time_tick` stays off; default 60 Hz for USA/JP; `--picture ntsc`,
    `runtime/ntsc.c`, `ntsc_apd.inc` are PAL-only.

**Shared across versions** (built with `-DVERSION_<SERIAL>`): all game C, every shim (GPU, GTE, SPU/XA, MDEC, CD,
card, pad, threads, lowmem), the renderer, widescreen / fps / texture packs, the launcher UI, gen2's relocation and
dispatch design. **Per version**: full-link symbol tables, data image and overlay maps, layout constants, the
runtime's retail addresses, `game_main`, the hand-written units (PAL only), panel tables, save time units, the fixes
in version branches.

## Stage U0: decomp prerequisites for USA (link proof and data symbols)

**Done 2026-11-09** (docs/USA.md "Full link"): all 26 files sha1 OK, `units(ver)`, `jp_globals --version`, name audit.

- **Goal**: `build/scus94236/full/` with byte-identical `SCUS_942.36`, `X00..X19.BIN` and the eight SYSTEM files
  (sha1-checked) plus `.full.ld` PROVIDE tables: the symbol source gen_port needs, and proof of every relocation.
- **Changes**: `tools/jp_globals.py --version` (USA data names into `config/scus94236/`); `tools/units.py`
  `units(ver=None)` built from the version symbol files plus `jpdemo_sources` (moved into units.py/versions.py);
  `tools/build_full.py --version` (`OUT=build/<ver>/full`, programs and sha1s from `versions.VERSIONS` and
  `config/<ver>/*.yaml`, `version_source()` flags, area prefix linked once from x00, `src/data/scus94236/`).
- **Risk: high, the main unknown.** Wrong relocation names hidden by `functionRelocDiffs: none` show up as sha1
  mismatches; each is a real port bug avoided *(guess: tens of hand fixes)*.
- **Verify**: every sha1 OK; the PAL full link unchanged. **Effort**: 3-6 sessions.

## Stage U1: generator per version ("boots and reaches gameplay headless")

**Done 2026-11-09** (docs/USA.md "Native port"): `build/port2-scus94236/tombi2` runs the USA disc headless through the
logos, the title, a new game and AREA00 gameplay (dialogue, event title, HUD, walking, enemies); report: 0 unresolved
literals, 45 stubs (PAL 148). The PAL build generates the same files and the same frames. Open: the 27 fixes.txt lines
that only reach PAL branches (widescreen culling, HUD, NTSC-picture emulation; the generator lists them) need USA
spellings (U3); the runtime features with PAL addresses (panel, debug menu, lang, animlog) are off in the USA build.
As built: `port/tools/portver.py` maps the version's files onto the PAL program names instead of a `pal_prog` rename
of every table; `gen_port.sources_version` is the version path (the PAL `sources()` is untouched).

- **Goal**: `build/port2-scus94236/tombi2` boots `data/` extracted from the USA disc through logos, title, new game
  and AREA00 headless (`--skip-movies`).
- **Generator**: `PORT_VERSION` (env / CMake, default PAL), output `build/port2-<ver>/gen`; game C with
  `-DVERSION_SCUS_94236`; `_blank_versions` keeps the target branch; symbol sources from `build/<ver>/full`; program
  lists from `versions.py` (keep PAL-style prefixes through `pal_prog` so fixes and runtime names carry over);
  `regions()` per version; area test `ovl_base + shared_prefix` (USA areas start at 0x801151F4);
  `handwritten_port2_funcs` PAL-only.
- **fixes.txt**: optional version column or `@version` sections, plus a gen-time **branch audit** (warn/fail when a
  fix only hits inactive `#if` branches); then the 41 USA files. `PORT_NTSC(...)` fixes are no-ops for USA.
- **`#ifdef PORT2` hooks**: copy the needed ones into the USA branches; play-time tick stays out.
- **Runtime**: generated `gen/port_version.h` (overlay base/end, MAIN load, load-list id range, `CdRead` bound, named
  retail addresses); replace raw `PORT_M0(0x...)` literals with symbol names; `game_main` from `src/main_scus94236.c`
  with `port_vblank()`; `port_hz = 60`, `port_ntsc` off; `psyq.c` checks `boot_sha1` and refuses another version's
  disc with a clear message.
- **Data**: `tools/extract_data.py` reads the BOOT line of `SYSTEM.CNF`, manifest `serial`/`boot`/`boot_sha1`.
- **Risk: medium** (USA overlay dispatch, load-list relocation). **Verify**: `gen/report.txt` 0 unresolved, stub
  count near PAL's; headless `--hz 60 --skip-movies` frame dumps; compare with `tools/ntsc_check.py` (SCUS_942.36 in
  `tools/psxemu`) traces and opening frames. **Effort**: 4-8 sessions.

## Stage U2: launchers, CI, distribution

**PC side and CI done 2026-11-09** (docs/PORT.md "Game library"): one launcher with a game library
(`tools/game_library.py`: boot exe + sha1 identification, `data-<serial>/` per game, the old `data/` migrated as the
first PAL entry, card art rebuilt from the player's disc through the title overlay's own logo sprite list, per-version
options, `tombi-pc-usa` for USA, JP recognised but not supported, `text_patches` hook per version);
`port2-exe.yml` builds `tombi-pc-usa(.exe)` next to the PAL game when `GAME_FILES_URL_USA` is set (disc-cache,
`build_full.py --version usa`, `PORT_VERSION=usa`) and the launchers bundle it; `versions.txt` in the bin-*-port
artifacts tells release.yml's notes which games a bundle runs. Open: Android (below), the USA exe only boots from
the launcher's data folder (the same as U1 headless; no `--disc`).

- **PC launcher** (`tools/port_launcher.py`): boot exe from `SYSTEM.CNF`, `{sha1: (serial, binary)}` table (PAL
  bec8dc5f…, USA 81cbc79f…, JP 07d57769…), `check_disc`/`data_ready` on `boot_sha1`, per-version binary; hide
  `--lang`, `--text-patch`, `--picture`, the 60 Hz switch for USA/JP.
- **Android**: `disccheck.c` reports boot sha1/serial; `LauncherActivity.java` accepted sha1s; `loader.c` dlopens
  `libtombi_<ver>.so` by manifest; one game library per version and ABI (arm64 through `port2/wasm/build.py`).
- **CI**: USA/JP matrix entries (`GAME_FILES_URL_USA` / `_JP`): `build_full --version` → gen2 → exe / .so.
- **Risk**: low; build time and APK size grow (~3x). **Verify**: `tools/test_port_launcher.py` cases, extract and play
  from the launcher headless. **Effort**: 2-3 sessions.

## Stage U3: USA feature parity

| Area | Work | Verify |
|---|---|---|
| Movies / XA | same shims; check `100US` / `END_US.STR` | intro frame dumps, `--dump-audio` |
| Saves | `card.c:164` accepts `BASCUS-94236`; save editor / `tombi_save.py` / `save_import.py` assume 1/50 s play time: per-product time base; check the USA layout ("assumed the same" in SAVE_FORMAT.md) | headless save/load round trip; a real USA save if available |
| All rooms | `sweep_areas.py` / `audit_pointers.py` with a USA room table | 96-room sweep, no findings beyond `DOOR_ROOMS` |
| fps, widescreen, HD, GL, texture packs | fixes from the U1 audit; texture-pack keys may differ | render comparisons with PAL |
| Debug menu, cut content, noclip | symbol names instead of addresses | per-flag smoke runs |
| Second-screen panel | `panel.c:153` reads `D0SPSUB.BIN` at PAL offsets; `panel_items.inc` / `panel_status.inc` / `panel_hud.c` / `panel_assets.c` use PAL addresses: per-version offset table from the USA symbols and xmatch | `--second-screen --dump-panel` |
| Text features | `lang.c`, `tpatch.c`, `subscreen_es.c`, `gfx_es.c`, `prompts.c`, `--subscreen-labels` are PAL-only: off for USA | — |

**Risk**: the panel is the largest item. **Effort**: 4-8 sessions (about half the panel).

Done 2026-11-09 (docs/USA.md "Feature parity"): fixes.txt `@version` sections and the USA spellings (widescreen, HUD,
title extras; `--launcher-exit` stays PAL-only), the USA room sweep 95/96 clean after six generator fixes, movies
(100US / END_US) and audio, widescreen / GL / `--fps` render checks. Saves, debug menu and the panel: other work items.

## Stage J1: JP retail

- Same pipeline with `slps01144` (data symbols already exist): full link, gen2, runtime header.
- JP specifics: overlay base 0x800E9AF8, shared prefix 0x2B680; no GOVER (check where game over runs); OPTSUB00 and
  DSPSUB2 have no entry table → `runtime/ovl.c` needs another identification (signature words, as for the areas);
  `game_main` from the JP C; 53 fixes.txt files with JP branches; more structural C differences (docs/JP.md).
- Text: kana/kanji are drawn by the game's own code from the disc fonts; every port text feature is off.
- Saves: `BISLPS-01144`, Shift-JIS title, play-time base *(guess: 60)*.
- Verification gap: `tools/psxemu` has no JP HLE map; build one from `config/slps01144/symbol_addrs_*.txt`, extend
  `ntsc_check.py` to `--exe SLPS_011.44`.
- **Risk**: medium-high. **Effort**: 6-12 sessions (link 3-5, port 3-7).

## Stage D: demos (later)

- Blocker: matching coverage (port2 has no interpreter): JP demo SLPM-80165 42.6 %, beta 980203 56.5 %, PrePre 8
  17.1 %. Either match the code each demo reaches (they are small: one area, or a single `TOMBA.EXE` for PrePre 8),
  or bring back a MIPS interpreter fallback from `port/` (conflicts with the native-pointer design).
- Per demo: the JP demo's own file table (docs/JPDEMO.md:27-29), PrePre 8 extraction via `tools/prepre8.py`, the
  beta has only X00. Start after USA/JP. **Effort**: 3-6 sessions per demo after matching.

## Open questions for the user

1. One binary per version (recommended: per-version game module, shared runtime source; launcher picks
   `tombi-pc-<ver>.exe` / `libtombi_<ver>.so`) or one for all (needs every game symbol prefixed per version)?
2. Load the data image from the user's disc at run time instead of compiling it in (releases free of game bytes, one
   CI build without the USA/JP discs at link time)? Bigger change; not required by this plan.
3. Which port features for USA/JP: second-screen panel, `--cut-content`, `--subscreen-labels`, 50 Hz option? (The
   improved Spanish translation is PAL-only by nature.)
4. Keep `--picture ntsc` for PAL once a real USA port exists?
5. Memory cards: one `tombi_mc0.mcd` for all versions or one per version?
6. Release USA/JP in the same release, or as "experimental" first?

## Order and milestones

U0 (link proof) → U1 (USA boots headless to AREA00) → U2 (launchers, CI) → U3 (parity) → J1 (JP) → D (demos).
Total *(guess)*: USA 13-25 sessions, JP 6-12 more, demos open-ended.

Critical files: `port2/tools/gen2.py`, `port/tools/gen_port.py`, `tools/build_full.py`, `tools/units.py`,
`port2/runtime/main.c`, `tools/versions.py`, `tools/build_report.py`, `tools/jp_globals.py`, `port/fixes.txt`,
`port2/include/port_runtime.h`, `port2/shims/psyq.c`, `tools/extract_data.py`, `tools/port_launcher.py`,
`port2/android/disccheck.c`, `port2/android/loader.c`, `port2/runtime/panel.c`, `tools/ntsc_check.py`.
