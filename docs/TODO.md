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
- **"Fully linked" on decomp.dev.** (Done: MAIN0/X000 matched units are marked complete by tools/build_report.py; remaining: link the overlays from source and) split each object's
  `.data`/`.rodata` out of the retail binary (roadmap step 4, segment split), keeping the linked
  executables byte-identical. Long, mechanical background task; start after the struct/type work.
- **Free-form event titles (code mod).** Change `spawnEventTitle` (src/spawnEventTitle.c) so it lays out the
  letters from the text itself (centred lines, a gap per space, `{NL}` breaks, tile animation kept) instead
  of the fixed per-title layout lists in the overlays; build a modified MAIN0 and let the text editor put it
  in the ISO. Then titles can have any length. Same idea for other text limits once more code is clean.
