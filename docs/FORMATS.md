# Tomba! / Tombi! file formats

Field-level summary of the disc formats, for people writing tools against this repo. It condenses what
[TombaEdit](https://github.com/SiergiejW/TombaEdit) (MIT, `docs/formats.md` and `docs/collision.md`) established and what
our own notes record (`notes/formats.md`, `notes/overlays.md`). The layouts below are TombaEdit's findings, cross-checked
against this repo's extractors where we have one; credit for the research on WHM, the SPR drawing rules, WSS, WFM and the
object initialisation goes to its author. See `THIRD_PARTY_NOTICES.md`.

Conventions: little endian, offsets in bytes. Routine addresses are US retail (`SCUS_942.36`) as TombaEdit gives them; the
PAL executable (`SCES_013.31`, `MAIN0.EXE`) has the same routines elsewhere (the PAL ones used by `tools/objinit.py` are
in that file). Where a statement is ours, not TombaEdit's, it says so.

## Archives and compression

* **File links.** The executable holds an array of CD records (BCD minute/second/frame); LBA = `(m*60+s)*75+f-150`. They name
  `SYS/LDSYS.BIN`, `SYS/LD0*.BIN` and the area files.
* **Load groups (LDAR).** 20-byte records: ignored u32, u16 file id, u16 type code, u32 RAM destination, then either a
  byte count + 0 or four u16 VRAM x, y, w, h. A non-zero id starts a source stream; id 0 continues the previous decoded
  source at the current offset; id `FFFF` with a zero footer ends a group. Group 0 is the base, groups 1.. are the sections.
  The type code's high nibble is the family (below), the low nibble the slot.
* **GAM.** 8-byte header (`GAM\0`, u32 decoded length), then u16 command words consumed LSB first. A 0 bit emits a literal
  byte; a 1 bit reads an 8-bit backward distance and an 8-bit count. References may overlap.
* **Table container** (WPP, WMD, WVD): `u32 N`, N or N+1 u32 offsets, items between consecutive offsets (ours: `notes/formats.md`).
* **VRAM** is 1024 x 512 16-bit words. Apply the base uploads, then the section's, in order, as replacement (zero words
  included). Colour 0 is transparent, `0x8000` is opaque black.

| Type code | Family | Notes |
| --- | --- | --- |
| 10, 11 | GRPX | raw VRAM rectangle: `w*h*2` bytes of words, no header |
| 30-3F | SPR | sprite frame table (slot = low nibble) |
| 40-45 | FOUR | per-corner WMD morph deltas |
| 50-55 | WMD | models |
| 60-63 | WPP | packed Tomba pictures |
| 70, 71 | WHM | collision |
| 80-8B | APD | placement lists |
| 8E, 8F | CLM | town picture (8FFF) and cell map (8E80/8EFF) |
| B0-B3 | SEQ/WVD | music, sample banks |
| D0, D1 | WFM | dialogue font and text |
| F0 | INS / `X` | code overlay |

Loader (routine 80021D70) files each resource pointer by family and slot in scratchpad words, 4 bytes per slot: SPR `1F8002C8`,
FOUR `1F800310`, WMD `1F800330`, WPP `1F800348`, WHM `1F800308`, APD `1F800358`.

## GRPX

Headerless pixels for `LoadImage(x, y, w, h)` with the rectangle coming from the load record. Whether it is 4, 8 or 16 bpp is
decided by the texture-page mode of whatever draws from it, not by the file. A 256 x 32 block at (128, 480) per area holds the
palettes (first column: Tomba's outfits, one row each). 16x16-word files are CLUT blocks.

## SPR (sprite banks)

Bank = table of 4-byte entries `{u16 part count, u16 offset}` per frame (the first frame's offset is the table length). A part is
16 bytes shaped like the texture half of a `POLY_FT4`:

| Offset | Field |
| --- | --- |
| 0 | u0, v0 |
| 2 | CLUT word |
| 4 | u1, v1 |
| 6 | texture page word, **relative** |
| 8 | u2, v2 |
| 10 | width, height |
| 12 | u3, v3 |
| 14 | s8 x, y offset from the object origin |

In our sources a part is `SprFrag` (`include/tomba/anim.h`: `u, v, clut, uv1, tpage, uv2, w, h, uv3, dx, dy`).

How the object completes a part (draw routine 800487B0, ours: `FUN_80052db8`): `page = part.tpage + obj.w1e` (the word at
`+1E`, so one bank is drawn from different pages by different objects); if `obj.b0d & 1` the part's CLUT is replaced by the word
at `obj+08`; `obj.b0d >> 7` enables semi-transparency; bit 0 of `+2E` selects facing (clear: x offset and width are subtracted,
mirroring). The four UV corners must be sampled as given: mirrored/rotated parts have reversed corners. The bank itself never
says which page or CLUT it uses; only the object's init code does (see "Placement and object init").

Animation scripts: an object's `+24` points at 8-byte steps `{u16 frame, 2 words unused by the stepper, u16 control}`; the low 14
bits of the control word are the duration in frames; the top two bits: `00` next step, `80` hold, `40`/`C0` jump through the
pointer after the step (loops).

## WPP

Pictures for sprites whose pixels are uploaded on demand. A bank is `u32 count`, count+1 offsets (end last), the entries.
An entry (unpacked by 8003A614) is flag-byte RLE: each flag byte covers eight items, low bit first; a clear bit copies one byte,
a set bit reads a count and a value and repeats the value. The result is `u16 width in VRAM words, u16 height`, then 4 bpp pixels,
uploaded to Tomba's reserved page slot whenever his frame changes (8003B0D4). SPR slot 0 has one frame per picture of WPP slot 0;
SPR slot 5 pairs with WPP slot 2. WPP is not the only pixel source: many sprites use pixels resident in section VRAM. WPP slot 3
holds GAM-compressed TIMs behind a bare offset table.

## WMD (models)

`u32 object count`, then that many absolute u32 object pointers. Each object has eight batches, each starting with a u32
primitive count: 0 FT3 (24), 1 FT4 (2C), 2 GT3 (34), 3 GT4 (3C), 4 F3 (20), 5 F4 (28), 6 G3 (30), 7 G4 (38). Textured primitives:
CLUT/page words, RGB/command word(s), one `<hhhBB>` (x, y, z, u, v) per vertex. Untextured: RGB/command words, 8-byte coordinate
records, 8-byte normal records (one per colour), a final 4-byte word. Sizes: FT3 32, FT4 40, GT3 40, GT4 52 bytes (1997 builds
use the GPU-packet order and are 40/52/48/64). Quads split `0,1,2` and `3,2,1`; view transform `(x, -y, -z)`; texture modulation
neutral at 128. Each object must end exactly at the next pointer.

FOUR: routine 80027A30 subtracts `(delta * weight) >> 12` per corner (wrapping s16).

## WHM (collision)

Not the WMD mesh: independent 2D path/Y boundaries per depth slice.

Header: `u8 tag (0x11)`, `u8 planes`, `u16` (not an origin), `u16 width` (path width, world units), `u16` (not a Y offset), then
`u16` absolute byte offsets of each plane. A plane has `width/8` u16 column offsets relative to the plane; a column offset points
at `u16 count` + count 8-byte records (columns may share lists):

| Record offset | Field |
| --- | --- |
| 0 | u16 attribute/control |
| 2 | s16 base Y (negative is up) |
| 4 | s16 Y delta (relative, never an absolute end) |
| 6 low nibble | start within the column |
| 6 high nibble | width |
| 7 | contact control (1 and 2 change floor approach tolerance) |

Attribute bits: `0001` floor, `0002` ceiling, `0004` side pushes left, `0008` side pushes right, `0010` filled rectangle; bits 5-8 a
surface code, with `1000`/`2000`/`4000` selecting player-mode and contact handling. A boundary runs from `(col*8+start, baseY)`
to `(col*8+start+width, baseY+delta)`; standing height `Y = s16(baseY + trunc(delta * ((x&7) - start) / width))` (truncating
division). Lookup (80042654): path column = trunc((path - origin)/8), clamped at 0; plane = trunc((depth - origin)/90), capped by a
per-area byte. Origins come from per-area tables; objects reach path/depth through pointers at `+40` / `+44`, swapped when
entrance orientation bit 0 (`1F8001C8`) is set. Several areas select among banks by section (village, mansion, mountain
alternate route): see TombaEdit `docs/collision.md`.

## CLM (towns)

Town sections are a pre-drawn picture, not models. Slot 15 (8FFF) is a TIM, 8 bpp with a 256-colour palette. Slot 14 (8EFF/8E80)
is the cell map over it: `u16 width, u16 height` in cells, then one byte per 8 x 8 pixel cell, row-major. 0 is not walkable;
non-zero values are walkable ground/doorways; bit 7 appears around doorways (meaning not established). Placement positions in a
town are pixel positions on the picture. (Our 8E grid notes: `notes/formats.md`.) Cutaway house interiors are SPR frames drawn
with a per-frame page base and a 256-colour CLUT chosen by the INFO overlay's style table (TombaEdit lists them).

## WFM (dialogue font and text)

WFM3: 144-byte master header, u16 absolute glyph offsets, 8-byte glyph headers, 4 bpp glyphs (low nibble first), a relative
dialogue-offset table, u16 dialogue words. Glyph ids use 12 bits; sound/blink flags are kept. Control words take arguments:
FFF9 and FFF7 one word; FFFA, FFF8, FFF6 two; unknown controls cannot be skipped. WFM1 (1997 builds): the header is the first 16
bytes of WFM3 with offsets straight after it; each glyph row starts on a word; FFF4 takes no arguments; glyph word top bits mark
voiced kana (1000 base + 2000 mark, drawn on the same cell; both 4000).

## WSS (event scripts)

`W\0S\0`, u16 file size (word), 64 u16 entry points to byte 88 (entry = code offset + 1, 0 = none), then code. One-byte opcodes
with operands: variable = 1 byte (40+ = argument/result registers r0..), number = 4 bytes.

| Byte | Meaning |
| --- | --- |
| 01 | END |
| 02, 03 | MOV var, var/num |
| 04 | SWAP |
| 05 | ARGS: count, then (kind, operand) each; loads r0.. |
| 06 | RAND |
| 08-10 | JMP, JEQ, JNE, JEQ, JNE, JLT, JGT, JLE, JGE with u16 code offset |
| 11 | LOOP var, u16 offset |
| 12 | CALL entry |
| 13-15 | RET, PUSHVARS, POPVARS |
| 16, 17 | CMP |
| 18-1B | INC, DEC, NOT, NEG |
| 1C-2B | ADD, SUB, MUL, DIV, MOD, AND, OR, XOR (even byte: variable operand, odd: number) |
| 2C | WAIT frames (1 byte) |
| 80-AB | game calls (dispatcher 8003AFB4), no operands, arguments in r0..; e.g. 80 CreateObject, 81 SetObjAnim |

Scripts exist for areas 0 (`0000.WSS`), 1/7 (`SCR0100`, `0120`-`0140`), 2/19 (`0200`/`0201`, `0210`, `0220`), chosen by
routine 80039338. CreateObject classes 2/10 make layer 2 type 18 (a record of box, page base, SPR slot and animation list by
subtype in the area overlay), class 9 type 19, class 3 layer 3 type 2E, class 4 layer 4 type 1E.

## APD placement and object init

Placement record, 16 bytes (routine 8005AA98): `[0]` section, `[1]` layer (low 7 bits; bit 7 flag), `[2]` type, `[3]`, `[4]`
variants, `[5]` flag byte, `[6..11]` three s16 coordinates (x, y, z), `[12]`-`[15]` further flags (draw mode for layer 7 at `[12]`,
room at `[15] >> 2`). Records can be relative to the previous absolute one. Layers 2-5 share one object pool and handler
table per layer (indexed by type); layer 7 is the backdrop sprites (own pool, 8003438C); layer 8 models (pool of 45, 8002DB4C).

The spawner fills a zeroed object from the record: `+00` state 1, `+02` type, `+03` variant `[4]`, `+0A` `[12]`, `+0C` `[3]`,
`+0F` `[13]`, `+1C` `[1]`, `+1D` `[0]`, `+2E` `[5]`, `+6B` room, coordinates at `+10/+14/+18` as `value << 16` with `+40`/`+44`
pointing at them. The handler (first update) then sets what an object looks like: `+1E` page offset, `+08` CLUT, `+0D` flags,
`+3C` SPR bank pointer, `+24` animation, `+A0` model. Running that handler on such an object, with on-screen tests answered
"visible", `GetClut` computed, draw-list and sound calls stubbed, is how TombaEdit resolves a placement to its bank, base and
palette; `tools/objinit.py` does the same with our interpreter. Child objects are made by parents (APD modes 2/3 link a
child at `+90` to its parent).

Items: descriptor `DAT_8007B14C[DAT_8007B084[subtype]]` (ours: `tools/sprclut.py`).

Draw ordering for flat views: sprite ordering = projected depth / 4 + signed `+0F`, with four buckets in front (80048B14, 80048B4C);
screen sprites use bucket 804 + offset (8004E014, 8004E338).

Layer 7 mode 2 draws a fixed picture from the table at 8007EBEC (indexed by variant): 18-byte records `{s16 tpage, x, y, u8 u, v,
u16 w, h, s16 clut_x, clut_y, s16 blend}` ending with tpage 7FFF.
