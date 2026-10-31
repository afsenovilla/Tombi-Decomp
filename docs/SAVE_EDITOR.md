# Tombi Save Editor

A PKHeX-style editor for Tombi! memory card saves: `tools/tombi_save_editor.py` (tkinter), library and command line
`tools/tombi_save.py`, tests `tools/test_tombi_save.py`. Windows build: `TombiSaveEditor.exe` (artifact
`bin-windows-save-editor` of the workflow `save-editor-exe.yml`). The format is in [SAVE_FORMAT.md](SAVE_FORMAT.md).

## Use

1. **Abrir tarjeta / Open card**: an emulator card (`.mcd`, `.mcr`, `.gme`, `.vgs`, `.mem`, `.mcs`) or the native
   port's `tombi_mc0.mcd` / `tombi_mc1.mcd` (in its save folder). Pick the save in **Partida / Save**.
2. Optional, **Datos del juego / Game data**: your own Tombi! PAL disc image (`.bin`/`.cue`/`.iso`) or an extracted
   folder; then item and event names appear in the language chosen in **Nombres** (English, Español, ...). The editor
   ships no game text: without game data it shows ids. The path is remembered in `~/.tombi_save_editor.json`.
3. Tabs:
   - **Jugador / Player**: AP, max HP (vitality, 1..8, up to 16 with the golden bowl bonus), HP, lives, the title's
     level, red/green/blue exp levels and bars, bonus hearts, equipped weapon/pants (raw ids), area/section of the
     Continue (with a warning: pick a room you already reached).
   - **Objetos / Items**: tick to have an item, quantity 0..255 (the inventory list is rebuilt like the game does);
     cut/unused items are marked.
   - **Eventos / Events**: the 130 events with their title and state; double click cycles not started -> started ->
     completed; buttons set a state; "Valor" writes a raw 0..255 value (some events count steps). Cut events are
     marked.
   - **Varios / Misc**: play time (h:m:s), purified areas, current area/section, checksum status.
4. **Guardar como / Save as** writes a new file (the source is not touched); the checksum and the title digits (time,
   lives, level) are rewritten.

Command line:

```
python3 tools/tombi_save.py list CARD.mcd
python3 tools/tombi_save.py show CARD.mcd 0
python3 tools/tombi_save.py set CARD.mcd 0 ap=123456 max_health=7 event.2=255 item.27=1 -o OUT.mcd
python3 tools/tombi_save.py roundtrip CARD.mcd      # unchanged write-back is byte-identical
```

## Verified

- Load -> write without edits is byte-identical (CLI and GUI) on the port's cards; synthetic `.mcd`/`.gme` tests.
- In the native port (headless, PAL disc): a save edited to AP 123456 / lives 7 / time 5:33 is listed with those
  values on the LOAD screen, and after loading it the Status screen shows AP 123456; another with AP 777777 and max
  HP 7 shows AP 777777 and vitality 7 in game.
- Only PAL saves were checked; USA / JP saves are recognised by their product code and assumed to share the layout.
