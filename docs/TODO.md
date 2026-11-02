# Pending (after the decompilation is complete)

- **DuckStation HD texture packs.** Export from the graphics editor DuckStation-compatible texture
  replacement packs for the PAL disc (SCES-01331): textures named by DuckStation's hash convention
  (upload + palette), so users can repaint/upscale them and load them in DuckStation. A PS1 ISO cannot
  show HD textures itself, so this is not a disc mod. The texture uploads and palettes can be traced with
  tools/psxemu. Format reference: https://github.com/mstan/TombaRecomp/blob/master/docs/HD_TEXTURE_PACKS.md
  (US-only; check its licence before reusing anything).
- **Hidden cheats / debug features.** (Researched, see notes/secrets.md.) No button-sequence cheats found; the debug flags
  (scratchpad 0x1F8001B4 area select, 0x1F8001AB debug loadout, 0x1BB/0x1BD/0x1BE/0x1BF/0x1B3/0x1B8) are never written by any code, so they only
  exist for GameShark / emulator pokes. Verified in tools/psxemu: 1B4 (area select PNG), 1AB. Still open: whether event 32 (title #52
  "MiniatureMakingTrumpets", in D_800806B4 but no layout list) is ever awarded; titles 90 and 126 are not referenced by the title table;
  overlay pad code not audited for sequences.
- **"Fully linked" on decomp.dev.** (Done: matched units of MAIN0/X000, the area overlays and the SYSTEM overlays are linked from source by
  tools/build_full.py and marked complete by tools/build_report.py; remaining:) split each object's
  `.data`/`.rodata` out of the retail binary (roadmap step 4, segment split), keeping the linked
  executables byte-identical. Long, mechanical background task; start after the struct/type work.
- **Free-form event titles (code mod).** Done for the native port (docs/PORT.md "Free-form event titles": the
  port lays the letters out from the text and a layout tail the text tools write; `"freeTitles": true` patches may
  be longer than the lists). Still open for the console: a modified MAIN0 (`spawnEventTitle` reading the same tail)
  that the text editor puts in the ISO; until then the console shows the first letters of a longer title
  (`check_text_patch.py` warns). Same idea for other text limits once more code is clean.
- **Japanese version (Ore no Tomba).** Next project: a matching decomp of the Japanese release reusing the PAL work.
  Split its executable(s), then auto-match functions against the PAL sources by masked byte hashes (as done for the
  overlay duplicates) and port the C; expect most game code to transfer, with differences in the text/font system
  (kana/kanji), the single-language file layout (no MAIN0-8) and older library revisions. Needs the user's disc image
  (kept out of the repo like the PAL one). Adapting the text editor to Japanese is a separate, larger task.
- **[All platforms] High frame rates (90 / 120 / 144 FPS) at the original speed (port).** Done: `--fps
  N|auto|unlimited` (docs/PORT.md "High frame rates"): the logic stays at 50 Hz, the presents run at the display's
  refresh (auto), a cap or unlimited, the in-between ones re-drawn from the recorded draw list of the displayed frame
  with every primitive blended towards where it was in the frame before (static / moved / animated sprites / streamed
  backgrounds following the camera; new objects, scene changes, camera cuts, movies and the debug overlay not
  blended), on the software and OpenGL renderers, PC and Android, with a *Frame rate* option in both launchers (default
  50). Still open: the software renderer manages about one in-between frame per vblank on a slow CPU (a faster
  rasteriser would lift that); a pre-projection variant (blending GTE inputs per object instead of screen positions)
  would bend fast close-up rotations less but needs per-object tags through the game's draw code.
- **[All platforms] Launcher language.** Done: Android *Options > Game > Launcher language* (Auto / nine languages,
  independent of the game language, applied without a restart; docs/PORT.md "Launcher language"), and both launchers
  translated into French, German, Italian, Dutch, Portuguese, Swedish and Danish (res/values-xx,
  tools/port_launcher_lang.py), with the game's own names for menus and places.
- **Unused animations pack (community rip, 136 files).** Locate on the disc and document the animations the pack shows
  that our docs (CUT_CONTENT, CIVILIZATION) don't cover: Taro (with shocked front/back/side), the Fisherman, the Sombrero
  Man (normal, cursed, cursed shocked, shocked), the Miner and MinerSmashing, the Cowboy (normal, cursed shocked,
  shocked); and check whether these are covered: the three thieves' duck/punch/knocked-back/surprised/lock-opening/laugh
  frames, the Adventurer sawing, the Digger, the Haunted Mansion and Singing Rock mermaid animations, Yan's Dad shocked,
  Baccus residents cursed/shocked and putting on a hat, Stonetown residents smashing, the Lumberjack factory worker
  hammering (side and top-down, shocked), the cursed Lumberjack old man. The "cursed" / "shocked" sets across several
  villages look like one cut curse mechanic: give it one section. For each: sprite table, frames, scripts, palette,
  whether any code uses it. Also the Funga chief's real palette (blue body, red cape in the rip) and the chief in JP,
  the Feb 1998 beta, PrePre and the JP demo. Never commit the pack or rendered pixels.
- **Reconstructed Funga drum quest (cut content option).** The pieces that exist: the Funga Leather / Tree / Sap and
  Small Drum items with their JP descriptions ("drum material"), the chief's unreferenced judgement dialogue (JP text to
  translate; PAL shows "···???"), the jungle lines that mention the drum, and the chief's sprite (AREA10 SPR 0x37 frames
  31, 47-58, no scripts). To build: where each material is found, the chief as an NPC (animation scripts from the
  frames), the crafting scene, the event and its title. Offered as a clearly marked reconstruction under "Contenido
  recortado", not as original content.
