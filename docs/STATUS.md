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
2. **SCES-01330 disc (FR/DE/IT/NL)**: secret `GAME_FILES_URL_SCES01330` exists (GitHub Actions + cloud env var). Plan:
   download in a fresh session, extend `tools/subscreen_glyphs.json` (tab/leaf font glyph table, coordinates only)
   with those four languages as a separate per-disc table.
3. Known crashes, both builds, from the debug area select: AREA17 section 0 and an AREA10 room. Not investigated.
4. `tools/port_mods.py` and the save editor keep hand-made English area names that disagree with the game's map
   (area 4 is "Haunted Mansion" on the map). Save import already uses the map names (REGIONS_EN / REGIONS_ES).
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

## Recently done (context)

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
