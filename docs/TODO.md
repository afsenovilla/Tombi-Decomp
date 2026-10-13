# Pending (after the decompilation is complete)

- **DuckStation HD texture packs.** Export from the graphics editor DuckStation-compatible texture
  replacement packs for the PAL disc (SCES-01331): textures named by DuckStation's hash convention
  (upload + palette), so users can repaint/upscale them and load them in DuckStation. A PS1 ISO cannot
  show HD textures itself, so this is not a disc mod. The texture uploads and palettes can be traced with
  tools/psxemu. Format reference: https://github.com/mstan/TombaRecomp/blob/master/docs/HD_TEXTURE_PACKS.md
  (US-only; check its licence before reusing anything).
- **Hidden cheats / debug features.** Search the decompiled code for button-sequence checks (pad input
  compared against sequence tables), writers of debug flags such as scratchpad `0x1F8001B4` (enables the
  debug AREA SELECT / SECTION SELECT on NEW GAME), unused debug menus and strings; verify any finding in
  tools/psxemu.
- **"Fully linked" on decomp.dev.** Mark complete units in the objdiff report and split each object's
  `.data`/`.rodata` out of the retail binary (roadmap step 4, segment split), keeping the linked
  executables byte-identical. Long, mechanical background task; start after the struct/type work.
- **Free-form event titles (code mod).** Change `spawnEventTitle` (src/spawnEventTitle.c) so it lays out the
  letters from the text itself (centred lines, a gap per space, `{NL}` breaks, tile animation kept) instead
  of the fixed per-title layout lists in the overlays; build a modified MAIN0 and let the text editor put it
  in the ISO. Then titles can have any length. Same idea for other text limits once more code is clean.
