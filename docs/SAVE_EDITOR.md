# Tombi Save Editor

A PKHeX-style editor for Tombi! memory card saves: `tools/tombi_save_editor.py` (Qt / PySide6), library and command
line `tools/tombi_save.py`, tests `tools/test_tombi_save.py` (library) and `tools/test_tombi_save_editor.py` (headless
GUI smoke test, `QT_QPA_PLATFORM=offscreen`). Windows build: `TombiSaveEditor.exe` (artifact `bin-windows-save-editor`
of the workflow `save-editor-exe.yml`). The format is in [SAVE_FORMAT.md](SAVE_FORMAT.md).

Run from source: `pip install PySide6-Essentials` then `python3 tools/tombi_save_editor.py [CARD]`.

## Use

- **Open card** (or drop the file on the window): an emulator card (`.mcd`, `.mcr`, `.gme`, `.vgs`, `.mem`, `.mcs`)
  or the native port's `tombi_mc0.mcd` / `tombi_mc1.mcd`. The saves appear on the left as cards (slot, place, play
  time, level, AP and the save's own memory card icon); click one to edit it.
- **Game data** (optional): your own Tombi! PAL disc image (`.bin`/`.cue`/`.iso`) or extracted folder. Item and event
  names and the inventory item icons are then read from it at run time (nothing is cached or shipped); without it the
  editor shows ids. The path is remembered in `~/.tombi_save_editor.json`. Misc > game text language picks the
  language of those names (English, Español, Français, ...).
- **EN / ES** switches the interface language; the sun / moon button switches dark / light theme.
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
- Only PAL saves were checked; USA / JP saves are recognised by their product code and assumed to share the layout.
