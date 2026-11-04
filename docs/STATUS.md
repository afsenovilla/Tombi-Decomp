# Status and open tasks (hand-off between Claude Code sessions)

Update this file whenever something lands or a task is decided. Newest first.

## Last release

v0.1.1 (build of 9cee5ef). After it, on master: vitality fruit numbers 8-16 and segment colours (2528f81), warp
destinations one-per-line (620495c), event detail page + shared touch scroller (c8529a5).

## In progress / open

1. **NTSC picture (`--picture ntsc`, opt-in, experimental)**: top-down areas (AREA02/05/08/19) draw sprites/scenery
   out of step and movies flicker. An agent found the cause for top-down rooms (USA map strips start at the visible
   tile row, PAL 24 rows above; USA camera limits table MAIN0 0x8007B680; sprite cull 0xF1) but had not committed.
   The default picture is PAL at 60 Hz, so this does not block releases.
2. **FR/DE/IT/NL (the SCES-01330 disc)**: `GAME_FILES_URL_SCES01330` (Drive link; GitHub Actions + cloud env var)
   is "Tombi! (Language set 1)" **Rev.0** (MAIN0 md5 3edd55b0, docs/BUILDS.md row 20), while the port's C is
   SCES-01331 **Rev.1** (row 24). Findings (session of 2026-11-04): the nine sub-screen font pages
   SYSTEM/A?0301.000 are byte-identical on both discs (the PAL disc already carries the FR/DE/IT/NL pages, so the
   glyph-table plan needs no second disc; only `--lang 5` uses that table anyway). 824 of 2287 files differ
   (every MAINn.EXE, every area overlay, SYS/LD*, D?SPSUB*, movies): Rev.0 vs Rev.1, not the language set. The port
   on a `tools/extract_data.py` extraction of that disc segfaults at start with `--lang 0` and `--lang 1` (lang.c
   does align MAIN1 with that disc's MAIN0: 57 chunks). A hybrid (Rev.1 data + the Rev.0 language files) is not a
   drop-in either: MAINn's file table is read by LBA and the two discs' layouts differ. Options, user decision
   pending: (a) a SCES-01330 Rev.1 dump (same MAIN0 as the port) would most likely work with lang.c as it is, as
   `--lang 1..4`; (b) a "language pack" path: files resolved by name across a second data folder plus a check that
   Rev.0 language overlays compose over the Rev.1 twins.
3. Debug area select crashes, checked 2026-11-04 (i386 build, 3000 frames each): AREA10 sections 0-8 all run clean;
   AREA17 section 0 segfaults in `spawnSectionObjectsAlt` (empty load list, one of the 13 `DOOR_ROOMS` of
   tools/psxemu/rooms.py that crash in the real game too). Not a port bug; only the door transition from a sibling
   room enters those. Note: `--poke` values are hex (area 10 = `A`, 17 = `11`), easy to get wrong.
4. `tools/port_mods.py` and the save editor keep hand-made English area names that disagree with the game's map
   (area 4 is "Haunted Mansion" on the map). Save import already uses the map names (REGIONS_EN / REGIONS_ES).
5. Trick Village Spanish name: user decision pending.

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
