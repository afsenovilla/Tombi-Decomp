# Tombi Save Editor

A PKHeX-style editor for Tombi! memory card saves: `tools/tombi_save_editor.py` (Qt / PySide6), library and command
line `tools/tombi_save.py`, tests `tools/test_tombi_save.py` (library) and `tools/test_tombi_save_editor.py` (headless
GUI smoke test, `QT_QPA_PLATFORM=offscreen`). Windows build: `TombiSaveEditor.exe` (artifact `bin-windows-save-editor`
of the workflow `save-editor-exe.yml`). The format is in [SAVE_FORMAT.md](SAVE_FORMAT.md).

Run from source: `pip install PySide6-Essentials` then `python3 tools/tombi_save_editor.py [CARD]`.

**In the PC launcher.** `tombi-port.exe` / `tools/port_launcher.py` has *Edit save…* next to Import / Export save
(docs/PORT.md "Edit save"): the same editor opens as a window of the launcher, in the launcher's language, on the port's
`tombi_mc0.mcd` from the save folder. There *Save* keeps a timestamped `tombi_mc0.mcd.YYYYmmdd-HHMMSS.bak` and is
refused while the game runs. Hosting API: `host_in_qt(parent, card, lang, write_guard, on_closed)` (from a running Qt program); the
`Editor` class takes `lang`, `write_guard` (returns a reason not to write, or None), `stamp_bak`, `on_closed` and `hosted`
(style sheet on the window only, not the whole QApplication).

## Use

- **Open card** (or drop the file on the window): an emulator card (`.mcd`, `.mcr`, `.gme`, `.vgs`, `.mem`, `.mcs`)
  or the native port's `tombi_mc0.mcd`. The saves appear on the left as cards (slot, place, play
  time, level, AP and the save's own memory card icon); click one to edit it.
- **Game data** (optional): your own Tombi! PAL disc image (`.bin`/`.cue`/`.iso`) or extracted folder. Item and event
  names and the inventory item icons are then read from it at run time (nothing is cached or shipped); without it the
  editor shows ids. The path is remembered in `~/.tombi_save_editor.json`. Misc > game text language picks the
  language of those names (English, Español, Français, ...). The item names are the ones the game's Items list shows
  (`SYSTEM/D<n>SPSUB.BIN`, see docs/SAVE_FORMAT.md). **Item names of the improved translation** (Misc, on by default):
  the list names of `patches/es_mejorada.tombipatch.json` (bundled) replace the disc's where the patch renames them
  for that language (Spanish: Bola -> Mangual, Garfio-gancho -> Manguancho, Bumerang madera -> Bumerán de madera...).
  Opened from the PC launcher, the default follows the launcher's text patch (its own names when one is on, the
  disc's when "None").
- **Item icons** are drawn exactly as the Items list does (`subScreenDrawItemGrid` -> `subScreenDrawItemIcon(id)`,
  src/d0spsub): a 16 x 16 4 bpp sprite of the texture page at VRAM (192, 0) (`SYSTEM/A00300.000` words 64..127),
  u = id % 15 * 16 + 8, v = id / 15 * 16 + 8, CLUT row `ITEM_ICON_CLUTS[id]` (D0SPSUB 0x80109C88; the same table in
  every DnSPSUB / DnSPSUB3) of the block uploaded to (288, 480), column 304. No per-item frame or descriptor is used.
  Checked pixel for pixel against the native port's Items list (`build/port2/tombi2 --headless --lang 5`, ids poked
  into `0x8009D1A4` / count `0x8009D2A4`, Triangle) for 90 ids (0-39, 52/57-59/61/90/91/97/108/147, 60-69, 100-109,
  130-139, 150-159): all identical except 64 (Pipe), which the port recolours on purpose (port/fixes.txt).
- The language box (EN, ES, FR, DE, IT, NL, PT, SV, DA: the PC launcher's nine; en / es in the editor,
  the others in `tools/tombi_save_editor_lang.py`, a missing string falls back to English) switches the interface; the sun / moon button switches dark / light theme.
- Tabs:
  - **Overview**: play time, AP, level, lives, vitality, location, events completed, items owned, checksum.
  - **Player**: HP, max HP (1..8, up to 16 with the golden bowl), lives, golden bowl, bonus hearts, AP, title level,
    red/green/blue exp levels and bars, equipped weapon / pants (raw ids), Continue area / section / spawn point
    (with a warning: pick a room you already reached). Each value has its range; bad values are flagged inline
    (red field + message) and not written.
  - **Inventory**: a grid of item cards (icon, name, id, cut badge) with an owned box and a quantity 0..255; search,
    owned only, show / hide cut items. The inventory list is rebuilt like the game does.
  - **Events**: the 130 events (id, title, state chip, raw value, cut badge); search and filter by state; double click
    cycles not started -> started -> completed; buttons set the selected rows (or a raw 0..255 value: some events count
    steps); "Complete all shown" / "Reset all shown" act on the filtered rows.
  - **Misc**: play time (h:m:s), purified areas, current area/section, diagnostics (directory, summaries).
- Edits apply at once; the top bar shows **Unsaved changes**, **Undo** (Ctrl+Z) steps back through this session's
  edits. **Save** (Ctrl+S) overwrites the card and keeps the previous file as `CARD.bak`; **Save as** writes a new
  file. The checksum and the title digits (time, lives, level) are rewritten; HP <= max HP and the golden bowl rule are
  checked before writing. Messages appear in the status line, not in pop-ups (only closing with unsaved edits asks).

Fonts: Nunito and Lilita One (SIL Open Font License 1.1, `tools/save_editor_assets/fonts`, licences beside them;
Nunito is shipped as static Regular / Bold / ExtraBold instances of the variable font). Icons are simple inline SVG
paths drawn for the editor. The colours follow the launcher's "Selva" palette.

Command line:

```
python3 tools/tombi_save.py list CARD.mcd
python3 tools/tombi_save.py show CARD.mcd 0
python3 tools/tombi_save.py set CARD.mcd 0 ap=123456 max_health=7 event.2=255 item.27=1 -o OUT.mcd
python3 tools/tombi_save.py roundtrip CARD.mcd      # unchanged write-back is byte-identical
```

## Verified

- Load -> write without edits is byte-identical (CLI and GUI) on the port's cards; synthetic `.mcd`/`.gme` tests; the GUI
  smoke test writes an untouched card byte-identically and checks edit / undo / .bak.
- In the native port (headless, PAL disc): a save edited to AP 123456 / lives 7 / time 5:33 is listed with those
  values on the LOAD screen, and after loading it the Status screen shows AP 123456; another with AP 777777 and max
  HP 7 shows AP 777777 and vitality 7 in game.
- USA saves (2026-11-09, docs/SAVE_FORMAT.md "USA saves"): the same GAME block, but the play time counts 1/60 s and
  the title digits sit 2 bytes earlier; the editor reads both per product code (`tombi_save.LAYOUTS`: PAL 50 Hz, USA /
  JP 60 Hz). A save written by the port's USA build, edited to AP 12345 / lives 7 / level 4 / 1:23:05, is listed with
  those values on the USA LOAD screen and loads with them. JP saves: 60 Hz time, title left as it is (its layout is
  not known). Item / event names still come from a PAL disc (the USA one has other glyph numberings).
