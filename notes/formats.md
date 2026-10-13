# Disc file formats (Tombi! PAL, Spanish)

One section per file type of the disc (`game/disc/fs`, never committed). "Done" = a tool in `tools/` reads it and rebuilds it
byte for byte; "Partly" = the container is understood and a tool splits/packs it, the content is not; "Open" = only
measurements and a guess. Hexdumps are the first 64 bytes (offsets in the file). All integers are little endian.

| Type | Files | Status | Tool / notes |
|---|---|---|---|
| `*.WFM` (fonts, dialogue text) | 369 (`CFNT999?`, `MFNT0nn?` in AREA00..19) | **Done** | `tools/wfm.py` (decode/encode, 369/369 identical), `notes/assets_tombatools.md` |
| `*.GAM` (LZ container) | 816 | **Done** (container) | `tools/gam.py` (unpack 816/816; repack 816/816 with the original bytes kept; 743 by the encoder alone); the content of an unpacked GAM is the same set of resources as the `.000` files below |
| `AREAxx/*.000`, `SYSTEM/*.000` | 286 | **Partly** (container done, `--check` 286/286 identical) | `tools/area000.py`; section "`*.000`" |
| `AREAxx/X0nn.BIN`, `SYSTEM/X?NN.BIN` | 17 unique, 153 in SYSTEM (9 variants x 17, byte-identical copies) | Code overlay, data layout documented | `notes/overlays.md`, `tools/overlay_data.py`, `src/x0nn/` |
| `SYS/LD*.BIN` | 180 (20 per variant `LD0`..`LD8`: `SYS` + 19 areas) | **Partly** (record lists understood) | section below; parsed by `tools/area000.py` to find each `.000`'s kind |
| `SYSTEM/*.BIN` (`D?SPSUB`, `G?OVER`, `I?NFO`, `O?PTSUB0`) | 9 x 8 | Open (overlays at `0x800E8028`, like the area ones) | section below |
| `SYSTEM/DSPCLUT.TIM` | 1 | Open, but trivial: 6 raw CLUTs | section below |
| `SYSTEM/SUBMAP.WMD` | 1 | Open | section below |
| `SOUND/*.WVD`, `SOUND/*.SEQ` | 52 + 36 | **Yes**, extracted by `tools/audio.py` (WAV / MIDI) | section below |
| `MOVIE/*.STR` | 30 | **Yes**, decoded by `tools/str.py` / `tools/audio.py` | section below |

Everything in the disc image is reached through the file table inside `EXE/MAINn.EXE` (`notes/cdfiles.md`), whose entries are
`LoadRecord.fileId` indices; the `FILE_*` names in `include/tomba24/cdfiles.h` come from the US build and are **wrong for PAL**
(for example id 334 is `FILE_AREA00_A005_000` there and `AREA00/A000.GAM` here). Resolve names with `python3 tools/cdfiles.py dump
game/disc/fs/EXE/MAIN0.EXE`.

## WFM (done)
Font glyph bitmaps plus the dialogue/menu text of one area and language. Fully handled by `tools/wfm.py` (decode to
`dialogues.yaml` + glyph PNGs, encode back, 369/369 byte-identical). See `notes/assets_tombatools.md` for the
Spanish glyph additions (`fonts/es`). Loaded with `LOAD_KIND_WFM3`.

### WFM3 in pure Python (`tools/wfm_py.py`, 369/369 byte-identical, no Go)
Header 144 bytes (`WFM3`, u32 pad, u32 dialogue-table offset, u16 n_dialogues, u16 n_glyphs, 128 reserved), `u16` glyph offsets, glyphs
`u16 w(VRAM words), u16 h, u16 advance (=4w in all 36,046 glyphs), u16 handakuten` + 4bpp pixels (low nibble first), then `u16` dialogue offsets
(relative to the table) and u16 token streams ended by FFFE/FFFF (a few files end unterminated). Tokens: `0x8000+n` glyph n (engine: low 12 bits = id,
top nibble flags: 8 normal, C corner icon without advance, E no advance; `C04D`/`C052` are the corner "next page" arrows);
FFF2 pose n, FFF3 halt, FFF5 choice, FFF6 move dx dy, FFF7 color n, FFF8 voice tone tail, FFF9 delay n, FFFA box w h, FFFB clear (page break), FFFC wait,
FFFD newline. Glyph tables are per file (only the glyphs that file uses, 71-148 for the S files); characters are not stored, the bitmaps are identified
with `tools/wfm_charmap.json` (331 bitmap hashes incl. 81 labelled by hand from word context: accented letters of all languages, ° º « » ( ) ; + 2 3 at 24 px; the generator keeps them). The two 8 px `Z`-like bitmaps are a capital `Z` (`aef0ef9a0232`, "Zwerge") and a lowercase `z` (`b610f5095df5`, x-height, "azul", "Pez"; wfm.py had labelled it `Z`, so 16 px text typed with `Z` got a small raised z). There is no special snore glyph. The encoder only uses glyphs of the dialogue's height and borrows from another file before falling back to another height; a glyph of another height decodes as `{G:n}`. Box metrics measured on 5,802 boxed dialogues: width = widest line (sum of advances) + 16 (88 % exact, the rest has trailing spaces),
height = 16 + 13 * lines, cursor starts at (8, 6), 128 glyphs per message slot (`MESSAGE_GLYPHS[][128]`). WFM files are plain files of the disc (no
container): a changed size only needs `mkdisc.py --relayout`. `tools/text_editor.py` / `dist/windows/tombi-text.exe` build on this.


### Event title layout (CFNT999*.WFM, 24 px): spaces and line breaks are in the overlays (`tools/title_layout.py`)
The WFM of a title holds only its letters (`Pulseradelabuelo`, 16 words, no flag bits, no space glyph); in game it shows as "Pulsera del" /
"abuelo", centred, each letter on an orange tile. Gaps and line breaks are **not in the WFM**: `spawnEventTitle` (FUN_8005aa74) walks a
list of 4-byte records per title, `u8 0, s8 col, s8 row, u8 flags`, one per letter in order (dialogue word i = record i): the letter goes
to x = col * 10, y = row * 14 (relative to the title origin, centred on column 0), `flags & 0xF` indexes the timing/colour tables
`D_800804E0 / D_800804EC`, bit 7 marks the first 11 letters that appear one after another (`0x80 | i`; other letters get a zone code
1/2/3/4). Letters are 2 columns apart, a space adds 1 column, a line break moves 2 rows (28 px) down (rows -4 / -2 / 0 for 3 / 2 / 1 lines).
Lists end with a record whose first byte is 0xFF and are stored one after another, dialogue k = list k - 2 (dialogues 0 and 1, the locked
titles, and four others (52, ~90, ~126, 135) have none): 130 lists, in every language the same way, found by matching list lengths to the
word counts of the CFNT file. Where: every `X?NN.BIN` / `I?NFO*.BIN` overlay of the language (`?` = digit n of MAINn; letters
B F G I H S P W D = n 0..8, `tools/cdfiles.py`), e.g. `AREA19/I5NFO.BIN` offset 0x697C (RAM 0x800F0B1C) for Spanish; the copies are identical.
The event tables point INTO the lists, so a list can be rewritten in place but not moved or resized. `spawnEventTitle` makes one tile per record up to the 0xFF terminator and reads one glyph per record from the text (`src += 2`, no text terminator test), so a SHORTER title is done by padding: the letters keep the normal layout, the surplus records go to row 40 (560 px below the origin, off screen; `title_layout.HIDDEN_ROW`) keeping their flags, and the WFM words are padded with a copy of the first glyph. Verified in `tools/psxemu` (AREA00 event 4 run through `FUN_8005aa74` with 12 of 15 records laid out and 3 hidden: only 12 tiles drawn, 120 frames without problems). A LONGER title needs the list moved to free space and every event-table pointer updated, or the code change in docs/TODO.md; not implemented (editor: error `max. N letters`).
The tool therefore reads the layout from the overlays (text shows `Pulsera del{NL}` + `abuelo`), ignores ' ' and {NL} in 24 px WFM text
(the old blank-glyph hack is gone), accepts up to as many letters as the list (fewer: hidden tiles), and writes moved spaces / line breaks back into
every overlay copy (flags kept). Odd-width centring is not reproduced bit for bit (31 of 1170 lists start one column further left), so
only titles whose spacing changed are rewritten. `python3 tools/title_layout.py` checks all 171 CFNT files.
**Dialogues 0 and 1 of CFNT999 (no list).** In `spawnEventTitle` the title text is entry `D_800806B4[area][event]` of the title table (`src + *(u16*)(src+8)`; the table is the
same for all areas: events 0.. -> entries 2,3,4,6,8,9,0xFF,10,11,12,13,72,14,...). Entry 0 is never selected by this routine; entry 1 is used
as the fallback title when the table byte is 0xFF (events 6 and 18, which have no title of their own), still walked with a list taken from the
area tables (so a letter count fixed by that list; this list was not located: the aligner only finds lists for entries 2..). Entry 1 is the
placeholder "DUMMY CUBE" (US/PAL: `!!!DUMMYCUBE!!!`, ES `¡¡¡CUBOVACÍO!!!`, 15 letters): it is shown for events without a title, it has nothing to do with the
cubes the letters sit on. Entry 0 is "CLEAR!" (ES `¡DESPEJADA!`, 11 letters); the routine that draws it is not in MAIN0 (not found: no other
`0x7D16` object spawner or reader of the title table in MAIN0; the score "5000" is `printEventMessage`, digit objects 16 px apart centred on x = 160,
y = 160, separate from the title). Its layout was therefore not read from code: the preview assumes the usual grid for a single line (letters 2 columns apart, row 0,
centred: -(2n/2) columns, which is what every list-based one-line title has, e.g. 11 letters start at column -11) and treats spaces as 1-column gaps.
Whether the engine accepts a different letter count for #0 was not verified; keep the original count when editing it.

## GAM (done)
`"GAM\0"` header followed by an LZ77 variant (greedy matcher, min length 5, see
`notes/assets_tombatools.md` "GAM: byte-exact repack"). The game decompresses it with `lzDecompress` for load records of type
`LOAD_TYPE_LZ_TO_VRAM` (images) and `LOAD_TYPE_LZ_TO_RAM` (data). `tools/gam.py` wraps the TombaTools fork.

## `*.000` (container done, content partly)
**What it is.** A `.000` file is *the same resource a `.GAM` holds, stored without compression*. There is no common file
header: what the bytes are depends on the **kind** of the load record (`LoadRecord.kind & 0xF0`, `include/tomba24/game/cdload.h`)
that names the file in `SYS/LD0AR*.BIN` / `SYS/LD0SYS.BIN`. The records read them with `LOAD_TYPE_STAGING`/`RAW`/`RAW_4`
(`src/cdLoadTask.c`, states 3..5: CdRead straight to the destination, no `lzDecompress`). All 286 files have a record
(`tools/area000.py` finds it by scanning the LD files; all 286 resolve):

| Record kind | Files | Content | Container |
|---|---|---|---|
| `0x10` GRPX | 237 (36 AREA, 201 SYSTEM) | a raw VRAM rectangle, `LoadImage(x,y,w,h)`: background pages, tile/sprite textures, CLUTs | none: `w*h*2` bytes of 16-bit words. Size = `w*h*2` exactly for every file; 512 B files are 16x16-word CLUT blocks, 32 KB = 64x256 words, 192/384 KB = 3/6 texture pages. 4/8/16 bpp is decided by the GPU texture-page mode of the sprites using it, not by the file |
| `0x60`/`0x62` WPP | 40 (AREA only) | packed sprite frames (`WPP_DATA[n]`); n=0: 33 files of exactly `0x2E008` bytes | u32 table container (below) |
| `0x50` WMD | 2 (`AREA06/D004`, `SYSTEM/A09300`) | 3D models (`WMD_DATA[n]`) | u32 table container |
| `0x8E`/`0x8F` APD | 7 (AREA02/05/08) | "asset placement data": 2D byte grid (`0x8E`) or a table (`0x8F`, 525,856 B, header `10 00 00 00 09 00 00 00 0c 02 00 00` = N=16?, not decoded) | grid (below) / opaque |

**Table container** (WPP, WMD; the same shape as the `.WVD` header tables): `u32 N`, then N or N+1 `u32` offsets; the first
offset equals the header size (`4*(N+1)` or `4*(N+2)`), items are `[off[i], off[i+1])`. With `4*(N+2)` the last word is an end offset
(WPP: `size-1`, leaving a 1-byte tail). Example `AREA01/A005.000`: N=358, header 1,440 bytes, 358 items of 322..750 bytes,
tail 1 byte; item 0 starts `88 0a 00 30 1e 00 90 cb ...` (a flag-byte RLE, see "WPP" below). Files read with `LOAD_TYPE_RAW_4` hold **two containers back to back**; the record `arg` is the
size of the first: `AREA00/A004.000` = WPP table (`0x2E008`) + a second table N=83 (85,212 bytes); `AREA02/D004` = table N=127
(`0x62`, 51,772 B) + a second block.

**Grid** (APD `0x8E`): `u16 w, u16 h`, then `w*h` byte cells row-major, zero padding up to the buffer (`0x4008` bytes for
`AREA05/D003`: 110 x 77, data spans one more row; `AREA02/D303`: 240 x 168 = 40,320 + 4 = `0x9D84`, no padding). Cell values are small
(`0x09`, `0x06`, `0x89`, `0x80`...) and the shape follows the level: a per-tile attribute/object map. `AREA08/D002` = grid 68 x 56
(`0x4008`) + the `0x8F` block (starts like `AREA05/D004`).

**Tool.** `tools/area000.py info FILE` prints the above, `unpack FILE DIR [--png]` writes `manifest.json` + `header.bin`,
`item_NNNN.bin`..., `grid.bin`, `image.bin` and PNG views (`view16.png` RGB555, `view8.png` 8 bpp gray, `view4.png` 4 bpp gray; pure
Python), `pack DIR -o OUT` rebuilds (the table header is regenerated from the item sizes, so items may change length), and
`--check` unpacks+packs every `.000`: **identical 286/286**. Verified visually: `AREA00/B301.000` (384x256 words, x=192) read as 8 bpp
is a room interior (background pages); read as 4 bpp or 16 bpp it is noise, so the background pages are 8 bpp.

Open: the `0x8F` block and what the grid values mean. (WMD layout and the per polygon CLUT: solved, section "WMD rooms" below.)

### WPP (done: `tools/wpp.py`)
Decoder in the matched code: `FUN_800371c0(table, frame, dst, d)` (`src/FUN_800371c0.c`) + `FUN_80037454` (computed-jump run fill),
called by `FUN_80037c80` (WPP_DATA[0] = scratchpad `0x1F800348` for type 0, WPP_DATA[2] = `0x1F800350` for types 1..3; frame = `*o->anim`)
and `FUN_80132174`/`FUN_801306f0`. WPP_DATA[n] are stored by the loader (`FUN_8004f538`, kind `0x60|n`). The table is `[u32 N][u32 offsets...]`
from the table base; frame i = `[off[i], off[i+1])`; the decoder reads `n = off[i+1]-off[i]` bytes.

Item codec: a flag byte, then 8 tokens (bit 0 first): bit clear = 1 literal byte; bit set = `count, value` = `count` copies of `value`
(count 1..255). The last group is cut where the item ends. Output = `u16 W, u16 H`, then H rows of W 16-bit words, 4 bpp (low nibble = left
pixel; so 4*W x H pixels, index 0 transparent). The game prefixes `10 00 00 00 / 00 00 00 00 / 01 00 00 00 / DX,DY` (a TIM without CLUT,
4 bpp) and `FUN_800174fc` does OpenTIM/ReadTIM/LoadImage to VRAM `x = buffer*16+1, y = 4` (Tombi) or `(448,160)`.
The leading bytes of every item (`08 08 00 30 25 00`...) are therefore just flag + W,H + the first run, not a header.
Encoder: greedy, a run for any run >= 2 equal bytes (max 255), else literal; it reproduces **12,934/12,934** items of AREA01/A005 and
all items of the 40 files. Verified: 13,017 frames decode to exactly `4+2*W*H` bytes. Sizes: 358 frames of Tombi/enemies in `0x60`
files (33 files `0x2E008` bytes, 40-48 px wide, 48 tall), 83 more in the second table of `AREA00/A004`, 127 in the `0x62` files (WPP_DATA[2]).
`python3 tools/wpp.py --check` -> `WPP files identical 40/40 (13017 frames)` (`unpack` writes `frame_NNNN.4bpp` = the decoded stream, editable,
and `frame_NNNN.png`; `pack` re-encodes).

### CLUT pairing (tools/area000.py `page`, `unpack --png`; tools/wpp.py)
- A GRPX record never names a palette. VRAM layout: 256 x 32 words at `(128,480)` per area (`AREAnn/CLUTkk.GAM`, record `x=128 y=480 256x32`,
  loaded in the same load list as the room pages `Bnnn.GAM/.000`): **32 palettes of 256 colours** (row r = VRAM y `480+r`) for the 8 bpp room
  pages. `tools/area000.py page AREA00/B301 -o x.png [--row N|--all-rows]` pairs page and CLUT through the load list.
- The 16 colour sprite palettes live in the same VRAM area: the last 16 colours of rows 0..19 of every `CLUTkk.GAM` are byte-identical to
  `SYSTEM/A00001.000` (x=368, y=480..495, 16 palettes), `A00002` (368,496, 4), `A10004` (352,480), `A00005`, `A00205/6`, `A00803/5`
  (`x=320..336`...): the system lists load them first, then the area `CLUT` overwrites that strip with the same data.
- **Tombi (WPP_DATA[0])** uses the CLUT at VRAM `(0x80,0x1e0)` (`FUN_800174fc` args; `GetClut(0x80,0x1f0)` in the draw code): colours 0..15 of
  row 0 of `CLUT01.GAM` (pink hair, pink shorts); rows 1..8 are the other outfits (green, blue, purple, red, grey...), row 26/27 = pink/green
  again. Colours 16.. of a row are the next 16 colour palettes (`--x0 16`). Enemy/NPC WPP frames use other 16 colour palettes of the same block.
- `SYSTEM/DSPCLUT.TIM` (3072 B) is loaded as GRPX kind `0x11` at `(288,480)` 48 x 31 words (menu palettes); not paired with sprites.
- Which of the 32 rows a room uses (and the per tile row) is chosen by the polygon CLUT field at draw time; not found in the data yet.

Verified visually (images only in /tmp): Tombi frames 0,40,100 of `AREA00/A004` with `CLUT01` row 0 look right (pink hair/shorts, correct
skin, no stray colours); `AREA00/B301` (x=192, 768x256 px 8 bpp) with `CLUT04` row 0 is a hut interior with sane colours but the page is an atlas of vertical
strips (the room is rebuilt from strips by the tile code), not a flat picture; `AREA00/B000` is a sprite/tile atlas (colours plausible
for rows 0..15, dark for >= 24). Not done: exact room reassembly. Per sprite palettes: next section.

### SPR frame definitions (load kind `0x30`..`0x3F` = `SPR_DATA[n]`, `FUN_8004f538` stores the buffer in `0x1F8002C8+4n`)
`SPR` is not a file: the records follow a `.GAM` of the same load list with file id 0 / type `0xFFFFFFFF` ("continue in the previous buffer", `arg` =
size, the loader aligns the next one to 4), e.g. `AREA00/A005.GAM` (LZ, 95,576 B) = WPP `0x63` (0x18C8) + SPR `0x30` (0x3248) + `0x35` + `0x31` + ...;
the sizes add up exactly to the unpacked size. `tools/area000.py list_blocks(ld, idx)` returns the bytes of every record; all 334 SPR blocks of
the 20 load files parse. Layout (decoded from the drawing code `FUN_80052db8`, `FUN_80055618`, `FUN_800505f0`; objects get the buffer in
`TObj.d3c`, the animation entry's first u16 = frame number):
- header: one u32 per frame `[u16 parts, u16 offset]` (offset from the block start; frame count = first offset / 4);
- a part = 16 bytes = a POLY_FT4 template: `u0 v0 clut(u16) | u1 v1 tpage(u16) | u2 v2 w h | u3 v3 dx dy` (dx,dy signed screen offset, w,h size;
  `FUN_80052db8` copies the words straight into the primitive, `tpage += TObj.w1e`, and replaces `clut` with `TObj.w08` when `b0d & 1`);
- `clut` = GPU CLUT word: `x = (c & 0x3F) * 16`, `y = c >> 6 & 0x1FF` (bit 15 is a flag of the game, not an address).
- **WPP link.** WPP_DATA[0] (`0x60`, 358 frames) <-> SPR `0x30` (358 frames); WPP `0x62` (`WPP_DATA[2]`: 83 frames in `AREA00/A004` 2nd table, 127 frames
  in `AREA02/D004`, `D304`...) <-> SPR `0x35` (83) / `0x3A` (127). Frame `i` of the WPP table is frame `i` of the SPR table (`FUN_80037c80` decodes
  frame `*o->anim` into VRAM slot `D_1F8001F4`, `FUN_800505f0` then draws the SPR parts with u offset `+ slot*64 + 4`); the pairing is by load
  list + equal frame count (unique in every list). `WPP 0x63` (16 frames) has no SPR table of its own.
- **Palette.** `FUN_800505f0` (the WPP drawer) uses `o->w08` for parts with the bit 1 of the count set, else `o+0xC4`, i.e. the object init sets it; but
  the SPR part `clut` holds the intended palette wherever it is a real VRAM address. Tombi frames carry placeholders `x=768` (rows 480/481), meaning
  "set at run time" (Tombi: `GetClut(0x80,0x1e0..)`, outfit rows). Others point into the sprite strip of the CLUT block: `(224,496)` = row 16, colours
  96..111 of `CLUTkk` (the 75 griffin/bird frames of the 2nd table of `AREA00/A004`: red body, yellow chest/beak), `(192,496)` = row 16 colours 64..79 (21 Tombi
  frames of the 1st table: hit/flash palette, white-grey; in other areas that slot of the CLUT is all zero, so it is not resolved there).
  `tools/wpp.py` (`--auto`, default) writes `manifest.json["wpp_palettes"]["frames"][frame] = {clut, row, x0, vram, spr, spr_frame, spr_part} | "default"`.
- Result: 201 of 13,017 frames get an SPR palette (all in `AREA00/A004`: 75 bird frames + 21 Tombi flash frames, plus the 21 flash frames in the other
  `0x60` files of areas whose CLUT has the slot filled); everything else is a runtime palette (Tombi) and keeps `row 0` of the first `CLUT` of the list.
  `python3 tools/wpp.py --check` still gives 40/40 identical.
- Verified visually (/tmp only): the 2nd table of `AREA00/A004` is now a red/yellow griffin (75 frames); frames 169..178 etc. are grey Tombi;
  `AREA02/D004` (127 frames), `AREA05/D005`, `AREA08/D103` are all **Tombi** (rolling, swimming, back/front, 358-frame table = the same
  character in every area) and correctly pink: the only non-Tombi WPP is the bird of `A004`; other enemies/NPCs are presumably not WPP but textures in `A*.GAM`/`D*.GAM`
  with SPR `0x31..0x3F` (parts carry their CLUT directly; not checked). The last 8 frames of the 2nd table of `A004` (a pink bat-like creature)
  have the placeholder palette (object code of the unmatched `X000` overlay sets it); they are shown with the default.
- Not done: the run-time palettes (Tombi costume rows, `GetClut(0x210, 0x1e1+n)` damage numbers = generated, not in any file), the AREA00 overlay code (not
  decompiled) that picks `o+0xC4` for the 8 frames, and pairing the non-WPP SPR blocks (`0x31..0x3F`) with their texture atlases.


### Animations (tools/anim.py)
Tombi's animation scripts, decoded from `AnimAdvance` / `AnimJump` / `AnimLoadDuration` (MAIN0) and `ObjSetAnimFromTable` (overlay).
- **Table.** All 17 overlays carry the same 90 x u32 pointer table at `0x800E8404` (file offset `0x3DC`); `PlayerSetAnimIfChanged(o, id)` stores the id in
  `player+0x2C` (`+0x2E` = id of the loaded animation), `ObjSetAnimFromTable` sets `o+0x24 = table[id]`, `AnimJump(o, 0)` loads the first duration.
  The pointers go to scripts in MAIN0's data (`0x80010748`..`0x80011800`); several ids share a script (ids 0/19/62/63/64/70, 22/27/28, 88/89) or point into
  the middle of a longer one (the same movement from a later frame: ids 20..23, 57..61, 83..88): 82 distinct scripts.
- **Entry** = 8 bytes `u16 frame | u16 box | u16 arg | u16 dur_flags`. `frame` = frame of Tombi's WPP table (`AREA00/A004.000`, 358 frames, the same
  table in every area; the SPR `0x30` parts give each frame's draw offset: `min(dx), min(dy)` of its parts relative to the object position);
  `dur_flags & 0x3FFF` = ticks the frame is shown (`o+0x2C` countdown), bits 14/15 = action when the time is up: `00` next entry (`o+0x24 += 8`),
  `0x4000` the next 4 bytes (NOT an entry; the stream goes on after them) are a pointer: jump there and load its duration (loop), `0x8000` end: keep the frame
  and return 1 ("finished") every `dur` ticks, `0xC000` jump and return 1. `AnimAdvanceWithBox` (enemy objects) also copies 4 hit box bytes
  `D_80138FD8[box*4]` into the object (`o+0x6C..`); Tombi's `box` values (0..12) are probably the same hit box index. `arg` is 0 except in the jump
  scripts 13/14/15/29 (200, 198, 192, 190, 168, 144, 136, 128: looks like a vertical speed profile) and the hurt script 9 (-14, -40: knock back speed).
  Tick length is not measured (the preview assumes 40 ms = 2 VSyncs).
- **Names** (`anim.NAMES`, from the state code in `X000_BIN_decomp.c` and the rendered strips): 36 of 82 scripts. Certain from code: 0 idle (walk speed `+0xB2` == 0),
  1/2/3 walk/jog/run (speed < 0x84 / < 0x144 / above, `FUN_800efa80`), 10/11/12 climbing idle/up/down (pad bits 0x10 up, 0x40 down in `FUN_801099b8`),
  33 crouch walk (down held). From code + strip: fall 4, crouch 5, climb_back 6, hurt 9, jump 13/14/15 (`FUN_80104f88`), turn 36, get up 48, roll 49, swimming 67/68,
  shock 80. The rest are guesses from the pose (marked `L` in the catalogue `confidence`) or keep their number. Not done: weapon/attack, bite/throw and
  swing animations (not found: probably other tables or other overlays' object code), other characters (enemy tables use `o->anim = D_xxxx[subtype]` in
  the overlays with `AnimAdvanceWithBox`; same entry format, not catalogued).

### SPR / VRAM: every graphic as PNG (`tools/vram.py`, `tools/extract_sprites.py`)
`python3 tools/extract_sprites.py game/disc/Tombi.bin OUT` writes `OUT/sprites` (WPP frames, `tools/wpp.py`), `OUT/spr` (SPR atlas cuts) and
`OUT/vram` (pages and VRAM snapshots) and `OUT/screens` (static full screens, `tools/screens.py`); `python3 tools/vram.py --all OUT` does the last two from an extracted `game/disc/fs`.
Pure Python, about 2 minutes, 95 MB of PNG (never committed).

**VRAM replay.** Every list of `SYS/LD0*.BIN` (`area000.load_lists()`: 19 area files + `LD0SYS` = 248 lists, 95 of them SYSTEM) names GRPX
records (kind `0x10`/`0x11`); `arg = y << 16 | x`, x in 16-bit words, `w x h` words, data = the file (`.GAM` unpacked, `.000` raw; size is `w*h*2`
in all 521 pages). `Vram.load` copies them into a 1024 x 512 word buffer in list order (later records overwrite: the area `CLUTnn` (128,480,256x32)
covers the lower 32 rows of the HUD/sprite pages 18..21, which is why a few HUD parts read palette data). A context is "what the game has when it
draws room *i*": resident SYS list 0 (without the start-up logo `A00006/A00007` at 0,0: HUD pages `A00000` = page 21 and `A10003` = page 20, sprite
palette strips `A00001/2`, `A10004`, `A00005`) + area list 0 + room list *i* (+ one SYS atlas list, below). Texture page `p` of a tpage word =
`x = (p & 15) * 64` words, `y = (p >> 4) * 256`, mode = bits 7-8 (0 = 4 bpp: 256 px per 64 words, 1 = 8 bpp: 128 words, 2 = 16 bpp); a CLUT word is
`x = (c & 0x3F) * 16`, `y = c >> 6 & 0x1FF`; a palette entry of word `0x0000` is transparent (GPU rule, not "index 0").

**Layout found in the lists.** A 256 x n (n <= 32) record is the CLUT of the page records right before it (`pair_cluts`: room list = pages + `CLUTnn`;
`A00200`+`A00201` = title landscape, `A00202`+`A00203` = TOMBI! logo, `A09400`+`A09401` = "Sony Computer Entertainment Europe presents", `A00006`+`A00007`
and `A00100`+`A00101` = Whoopee Camp logo, `A00400/1`+`A00402` = save/load menu: font, frames, TOMBI! tile background). 16 x n records are sprite palette
strips (16 colours x n palettes) and belong to the page before them (`A00000` <-> `A00001/2`, `A10003` <-> `A10004`). SYS lists 10..92 (X0nn/I?NFO
overlays) hold one 4 bpp atlas set each for the NPC/enemy/object tables of the area that names the same overlay: `SYSTEM/A01000.000` (384 x 256 words at
x = 64 = pages 1..6) is the atlas of the 194-frame NPC/enemy table `0x32` of AREA00 (pigs with pitchforks, the boy with the sword, the straw-hat man).

**The tpage offset.** `FUN_80052db8` does `tpage += TObj.w1e` and the area/shared code sets `w1e` per object type (`o->w1e = 0x13`, `1`, `10`, `0x14`..: shared
code in `src/FUN_8002*.c`, `src/FUN_800e*.c`; area overlays in `src/x0nn/`), so an SPR table stores pages relative to an atlas and does not say where the atlas
is (examples: `0x34` effects are used with `w1e` 0x13/0x14/0x15 = pages 19/20/21 = `A001`/`A10003`/`A00000`; `0x32` with 1 = x 64; `0x35` WPP with 7). The
only data-driven evidence is the load lists, so `vram.infer_base` chooses (context, offset 0..31) per table: all part rectangles inside loaded 4 bpp page data (pages
paired with a 256 colour CLUT are 8 bpp room backgrounds and excluded; 8 bpp/16 bpp parts accept any loaded data), then the candidate whose pages are exactly
the pages of the atlas records it touches (`fit_unused` = 0: AREA00 `0x32` = 6 pages = `A01000`), then the one that cuts the least sprite art through its rectangle borders
(`_ring_cut`). Tables byte-identical in >= 10 areas (Tombi `0x30`, `0x31`, HUD `0x3F`, effects `0x34`, `0x39`) can only use the resident pages. Frames whose rectangles
are not all inside the chosen atlas are retried with the other offsets that cover part of the table (`alt_offsets`; the manifest gives `tpage_offset` per such frame).
`manifest.json` of every table records `tpage_offset`, the SYS list used, `ties` (equally good candidates = ambiguous) and `coverage`.

**WPP link.** SPR tables whose frame count equals a WPP table of the list (`0x30`/358 Tombi, `0x35`/83 griffin+bat of `A004`, `0x3A`/127 of `D004`) are not cut
from VRAM: the part `(u, v, w, h)` is cut from the decoded WPP frame of the same number (the WPP slot offset `slot*64+4` is the game's, the frame itself starts at u = 0),
placeholder palettes `x >= 512` (Tombi: `GetClut(0x80, 0x1e0..)`) map to `x = 128`, same row. Tombi's two parts per frame use rows 480/481 of the CLUT (hair/skin and shorts).

**Output.**
- `OUT/spr/<AREA|SYSTEM>/listNN/sprKK/frame_NNNN_partK.png` (indexed PNG, 16/256 colours, tRNS for transparent words; 16 bpp parts RGBA), `frame_NNNN.png`
  (parts composed at `dx,dy` in table order, RGBA) and `manifest.json` (per part x/y/w/h/clut/tpage/uv, per frame origin = position of (0,0) in the PNG).
  Only lists that carry SPR tables exist (125 lists, 334 tables, 42,793 frames, 89,932 parts). A table that is byte-identical and renders identically to an earlier one
  (Tombi in 34 lists, resident tables) holds only a `manifest.json` with `"same_as"` (101 tables). Parts with no visible pixel are not written (`"blank": true`, 2,993 of
  the cut ones); palette slots that are empty in the list's `CLUT` (filled at run time or only in the CLUT of another room) are taken from another `CLUTnn` of the area, else
  a grey ramp (`palette_note` in the manifest, 6,954 parts).
- `OUT/vram/<AREA|SYSTEM>/listNN/vram.png` = the whole 1024 x 512 VRAM of the list as RGB555 (8 bpp data looks like noise there; that is the 16 bit view): AREA lists on top
  of the resident pages, SYSTEM lists alone (231 snapshots). Next to it every GRPX record: `NAME.png` for 8 bpp pages with the CLUT the list pairs them with (318;
  row 0 of a 256 x 32 block, or the palette their 8 bpp SPR parts use), `NAME_4bpp.png` / `NAME_8bpp.png` for atlases coloured with the palette most SPR parts use on them (72),
  `NAME_16bpp.png` + `NAME_4bpp_gray.png` when nothing says how a page is read (131), `NAME_palettes.png` for palette blocks (141, each colour 4 x 4 px).

**Static full screens (`tools/screens.py` -> `OUT/screens/*.png`).** All are 8 bpp pages of `LD0SYS` with the 256 x 1 CLUT record next to them (`arg` of the CLUT = (128, 509..511)).
The record width is the LoadImage pitch, not the picture width: `A00200` (title landscape) is 256 words x 256 rows = 512 px pitch, but the picture is 320 x 240 px (160 words; the rest is
zero), `A09400` ("Sony Computer Entertainment Europe presents") is 320 words x 512 rows = 640 x 512 px (hi-res PAL frame), `A00100`/`A00006` (Whoopee Camp logo, identical files) is 384 words
x 256 (LoadImage rect 384 x 256 words at (0,0)); the non-zero data is exactly x 9..563, y 13..125 of the 768 x 256 px page (checked: nothing beyond, no continuation in the next page), so the 576 x 128 crop contains the whole logo. `A00202`+`A00203` (palette at (128,510) vs (128,511) of the landscape) is the TOMBI! logo page, 256 x 256 px, cut into sprites by code (the right end of the `!` is clipped by the 256 px page edge at x 247 in the data: the page `A00202` is exactly one 8 bpp texture page, `A00204` next to it is 4 bpp font data, not a continuation; the blue piece at the bottom left, x 8..39 / y 132..222, is probably the remaining part, stitched by the sprite table, not in the file layout); `A00204` is a 4 bpp font/HUD
sheet (256 x 256 px) with 11 palettes (`A00205` 3 + `A00206` 8 strips of 16 colours) = one tile per palette in `title_font_sheet.png`. The menu/map pages (`A00400/1`, `A00500/1`, `A00701`) are texture
atlases (8 bpp, each page keeps its own CLUT record; the font in them is 4 bpp with `DSPCLUT.TIM`), not screens, and are written as such. 15 -> 24 bit: `c << 3 | c >> 2` per channel (31 -> 255), STP (bit 15) ignored,
word `0x0000` transparent only in the sprite/atlas outputs (`title_logo_page`), opaque in the full screens. Unresolved: the language variants (`A?0003`, `A?0301`, `A?0601`, `A?0703` of `LD1..3SYS`), the 4 bpp pages of
SYSTEM lists 3, 6, 9 (menus) have no palette in their list (run-time/`DSPCLUT`) and stay as 4 bpp gray + 16 bit views in `OUT/vram`.

**Verified visually (PNGs in /tmp only).** Start-up logo (`SYSTEM/list01` `A00100`: pink/yellow "Whoopee Camp"), `SYSTEM/list94` ("Sony Computer Entertainment Europe presents"),
title screen (`SYSTEM/list02`: `A00200` landscape with volcanoes/forest/river, `A00202` TOMBI! logo with the pink-haired Tombi), save/load menu (`SYSTEM/list04`), enemies: the evil pig
with pitchfork (AREA00 `spr32`, offset 1 + SYS list 10, all poses coherent, atlas `A01000_4bpp.png` correctly coloured) and the griffin (WPP `spr35`, red/yellow); NPCs: the boy with the sword
and the straw-hat man (same table, last frames); HUD: AREA00 `spr3F` (digits 0..15 in beige/orange, the font `abcde..`, circles) and `spr34` (explosions, feathers, jewels, "GET!!" text);
AREA05 `list02` `spr3B` (pink hat creatures, sombrero bandits, offset 1 + `A04000`) and `spr35` (treasure chests, buckets); Tombi `spr30` (358 frames, pink hair, green shorts).
**Limits.** (1) The per object `w1e` is inferred, not read from the overlay data tables: tables used with several offsets (`0x33`, `0x34`, `0x39`, `0x3F`) get the best offset per table and an
alternative per frame, so some frames show another atlas' art (stripes/noise: 8 bpp data read as 4 bpp, or an atlas that is not in the candidate contexts); `ties` in the manifest lists the
other candidates. Frames with 8 bpp parts in a 4 bpp context (`(1,x)` tpages) are cut as 8 bpp from whatever is loaded. (2) Candidate contexts are the list, the area list 0 and the SYS lists
that name the same overlay as the list; SYS pages loaded by other code (3D room textures `I0NFO2`, menus) are not tried. (3) Room pages (`B*.GAM` with `CLUTnn`) are exported with row 0 of the
CLUT (or the row their 8 bpp SPR parts use): which of the 32 rows a room tile uses is chosen per polygon at draw time (not in any file), so room backgrounds can be wrongly coloured; B-pages
that hold 8 bpp sprite sheets (AREA00 `B000`) are shown with row 0 and look wrongly coloured. (4) Runtime-generated graphics (damage numbers `GetClut(0x210, 0x1e1+n)`, fades, the WPP slots) and the
3D models (WMD, `SUBMAP.WMD` textures are the `A07xx` pages, uncut) are not reconstructed; only `LD0*` lists (the language variants `LD1..LD8` use other file ids but the same layout) are walked.
(5) Semi-transparent parts (tpage bits 5-6) are drawn opaque. (6) No names for characters: the tables only have ids (the pig/boy/straw-hat man labels above are descriptions of what the PNGs show).

## `AREAxx/X0nn.BIN` (overlays)
Headerless RAM image loaded at `0x800E8028` by load kind `0xF0`. Fully documented in `notes/overlays.md`: header/callback pointer area,
entry stubs, shared prefix of 188,028 bytes, per-area code, `.rodata` jump tables between functions, `.data` tail. Tool:
`tools/overlay_data.py` (`summary`, `dump`, `header`, `strings`). No text inside.

## `SYS/LD*.BIN` (load record lists)
Dumps: 
`SYS/LD0SYS.BIN` (7784 bytes)
```
0000  05 00 00 00 14 00 ff 10 00 10 14 80 40 01 00 01
0010  40 00 00 01 01 00 00 00 15 00 ff 10 00 10 14 80
0020  70 01 e0 01 10 00 10 00 00 00 00 00 16 00 ff 10
0030  00 10 14 80 70 01 f0 01 10 00 04 00 00 00 00 00
```

`SYS/LD0AR00.BIN` (2192 bytes)
```
0000  06 00 00 00 4e 01 ff 10 00 10 14 80 00 00 80 00
0010  40 00 80 00 01 00 00 00 4f 01 ff 10 00 10 14 80
0020  c0 00 00 01 40 00 00 01 01 00 00 00 50 01 ff 10
0030  00 10 14 80 40 02 00 00 c0 01 00 01 01 00 00 00
```

Layout (read from `src/cdLoadTask.c` and verified by parsing all `LD0*`): `u32 n` (a small count: 5 in `LD0SYS`, 6 in
`LD0AR00`, 11 in `LD0AR05`, 24 in `LD0AR19`; meaning not pinned down), then packed 20-byte `LoadRecord`s:
`s16 fileId` (index of the MAINn file table), `u8 slot` (0xFF none, <0x80 stores the destination, >=0x80 reuses it), `u8 kind`
(`LOAD_KIND | index`), `u32 addr` (0 = after the previous record), `u32 arg` (size, or `y<<16|x` for GRPX), `s16 w, h`, `u32 type`
(`LoadType`, or `0xFFFFFFFF` = a sub-piece of the file before it). Lists end with `fileId = -1`. `LD0SYS` is loaded at `0x80097FA8`
and holds the lists of the system data (`AREA_LOAD_LISTS` points into it) and the SYSTEM `.000` records; `LD0ARnn` holds the lists of
area `nn` (area list + one list per section/room). `LD1..LD8` are the language/ending variants (fileIds differ per MAINn).
This is the source of truth for what every `.000` is; `tools/area000.py` reuses it.

## `SYSTEM/*.BIN` (D?SPSUB, G?OVER, I?NFO*, O?PTSUB0)
Code overlays of the same family as `X0nn.BIN`: `u32 id`, a pointer table into `0x800E8xxx`, then MIPS code; one set per
language/variant `n` = 0..8 (the `n` digit; `G?OVER` has 3,244 bytes in all nine, some byte-identical). Observed ids:
`0x31` `D?SPSUB` (~165 KB, 64+ pointers; the sub-screen/menu code), `0x32` `D?SPSUB2` (~63 KB: table of small offsets, then data
including the string `BESCES-01331` = the memory card save id: save/load screen data), `0x33` `D?SPSUB3` (~100 KB, same
pointer-table shape as SPSUB), `0x35` `G?OVER` (game over, 6 pointers then code), `0x36`/`0x37`/`0x38` `I?NFO`, `I?NFO2`, `I?NFO3`
(54 KB / 137 KB / 39 KB, info/inventory screens and their data), `O?PTSUB0` (13.4 KB, options). The area overlays use id `0x1D`.
Presumably loaded by `LOAD_KIND_INS`/`LDSYS` records like the area ones (not yet traced). `SYSTEM/X0nn..X8nn.BIN` (153 files) are byte-identical
copies of the area overlays and of their `X1nn..X8nn` variants.

First 64 bytes:

`SYSTEM/D0SPSUB.BIN` (168236 bytes)
```
0000  31 00 00 00 30 96 0e 80 90 99 0e 80 90 99 0e 80
0010  90 99 0e 80 90 99 0e 80 30 8a 0e 80 bc 97 0e 80
0020  8c 8a 0e 80 90 99 0e 80 90 99 0e 80 90 99 0e 80
0030  90 99 0e 80 90 99 0e 80 90 99 0e 80 90 99 0e 80
```

`SYSTEM/D0SPSUB2.BIN` (63208 bytes)
```
0000  32 00 00 00 82 58 00 00 82 57 00 00 82 56 00 00
0010  82 55 00 00 82 54 00 00 82 53 00 00 82 52 00 00
0020  82 51 00 00 82 50 00 00 82 4f 00 00 42 45 53 43
0030  45 53 2d 30 31 33 33 31 00 00 00 00 42 45 53 43
```

`SYSTEM/D0SPSUB3.BIN` (101216 bytes)
```
0000  33 00 00 00 88 95 0e 80 e8 98 0e 80 e8 98 0e 80
0010  e8 98 0e 80 e8 98 0e 80 88 89 0e 80 14 97 0e 80
0020  e4 89 0e 80 e8 98 0e 80 e8 98 0e 80 e8 98 0e 80
0030  e8 98 0e 80 e8 98 0e 80 e8 98 0e 80 e8 98 0e 80
```

`SYSTEM/G0OVER.BIN` (3244 bytes)
```
0000  35 00 00 00 dc 84 0e 80 24 85 0e 80 50 85 0e 80
0010  90 85 0e 80 00 86 0e 80 b0 86 0e 80 e0 ff bd 27
0020  10 00 b0 af 80 1f 10 3c e2 00 10 26 14 00 b1 af
0030  de ff 11 26 e0 fd 02 24 18 00 bf af 00 00 00 a6
```

`SYSTEM/I0NFO.BIN` (54452 bytes)
```
0000  36 00 00 00 f4 88 0e 80 f4 88 0e 80 e4 88 0e 80
0010  f4 88 0e 80 f4 88 0e 80 f4 88 0e 80 ec 88 0e 80
0020  f4 88 0e 80 2c 89 0e 80 b4 89 0e 80 ec 89 0e 80
0030  24 8a 0e 80 2c 89 0e 80 00 00 00 00 08 8b 0e 80
```

`SYSTEM/I0NFO2.BIN` (137436 bytes)
```
0000  37 00 00 00 58 97 0e 80 68 97 0e 80 78 97 0e 80
0010  88 97 0e 80 98 97 0e 80 88 99 0e 80 a8 97 0e 80
0020  88 99 0e 80 e0 97 0e 80 f0 97 0e 80 00 98 0e 80
0030  10 98 0e 80 20 98 0e 80 30 98 0e 80 40 98 0e 80
```

`SYSTEM/I0NFO3.BIN` (38984 bytes)
```
0000  38 00 00 00 68 92 0e 80 68 92 0e 80 58 92 0e 80
0010  68 92 0e 80 68 92 0e 80 68 92 0e 80 60 92 0e 80
0020  68 92 0e 80 a0 92 0e 80 28 93 0e 80 60 93 0e 80
0030  98 93 0e 80 a0 92 0e 80 00 00 00 00 7c 94 0e 80
```

`SYSTEM/O0PTSUB0.BIN` (13420 bytes)
```
0000  34 00 00 00 c8 82 0e 80 24 83 0e 80 90 83 0e 80
0010  5c 84 0e 80 b4 84 0e 80 54 85 0e 80 8c 92 0e 80
0020  dc 92 0e 80 2c 93 0e 80 7c 93 0e 80 cc 93 0e 80
0030  fc 93 0e 80 60 9c 0e 80 84 9a 0e 80 60 9c 0e 80
```

## `SYSTEM/DSPCLUT.TIM`
3,072 bytes = **6 x 512-byte palettes** (6 CLUTs of 256 RGB555 entries); despite the name there is no TIM header (a TIM starts
`10 00 00 00`). Best guess: display/status CLUTs loaded as raw words. First 64 bytes:
`SYSTEM/DSPCLUT.TIM` (3072 bytes)
```
0000  00 00 f9 67 f2 3a 3f 5f b5 6e 19 73 d9 4a ff 7f
0010  39 42 14 32 b2 29 5c 42 2a 21 72 35 ca 14 00 00
0020  00 00 4f 03 e0 02 41 02 5a 6b 29 25 2d 05 b1 05
0030  37 0a 9a 0a 1f 0f ff 43 ce 39 b5 56 63 0c ff 7f
```

## `SYSTEM/SUBMAP.WMD`
43,064 bytes. `u32 1, u32 8, u32 422` then GPU-packet-like records (`c8 7f 85 00 | 80 80 80 25 | bc 00 14 00 b9 ff 10 6f ...`: a
texture word, `r g b cmd` with cmd `0x25`/`0x2D`/`0x2F` = textured triangles/quads, then s16 x/y/z). Best guess: a 3D
model of the sub-map (map screen) in the same WMD family as `AREA06/D004.000`; 422 = number of primitives.
`SYSTEM/SUBMAP.WMD` (43064 bytes)
```
0000  01 00 00 00 08 00 00 00 a6 01 00 00 c8 7f 85 00
0010  80 80 80 25 bc 00 14 00 b9 ff 10 6f da 00 1a 00
0020  c1 ff 1f 6b da 00 1b 00 ba ff 1f 6f c8 7f 85 00
0030  80 80 80 25 f7 00 22 00 ba ff 2f 6f ee 00 1e 00
```

## `SOUND/*.WVD` and `SOUND/*.SEQ`
`.WVD` = bank file (52 files, 13 KB..226 KB). Header (from `src/cdLoadTask.c` case 5, verified on six files): `u32 bodyTable`
(always `0xC`), `u32 headerTable`, `u32 file size`; at `bodyTable` and `headerTable` a table `u32 offsets` whose first word is
`4*(N+1)` (N banks, 1 or 2 observed) giving the sample bodies (SPU ADPCM, `VB`) and the standard Sony **`pBAV` VAB headers**; the
loader uploads each body to the SPU (`SPU_BANK_ADDRS[h]`) and registers the VAB with `SsVabFakeHead`/`SsVabFakeBody`. `.SEQ` =
standard Sony `pQES` sequence files (36 files, 9 KB..; `S_AR000.SEQ` = music of area 000). Dump:
`SOUND/SND0000.WVD` (193892 bytes)
```
0000  0c 00 00 00 18 d5 02 00 64 f5 02 00 0c 00 00 00
0010  bc c3 01 00 0c d5 02 00 00 00 00 00 00 00 00 00
0020  00 00 00 00 00 00 00 00 32 04 00 10 0f ee ce dd
0030  21 e1 cb fd 2f 44 11 64 32 00 d1 e9 bd bd ab c8
```

`SOUND/S_AR000.SEQ` (9328 bytes)
```
0000  70 51 45 53 00 00 00 01 01 e0 0a 85 f7 04 02 00
0010  c4 04 00 04 00 04 00 04 00 04 00 04 00 04 00 04
0020  00 04 00 c1 01 00 c3 03 00 c6 00 00 c5 05 01 94
0030  37 6a 00 54 73 00 91 33 67 00 93 33 66 00 96 4b
```

### Decoding (`tools/audio.py`, verified on all 52 + 36 files)
* WVD: `u32 bodyTable (12), headerTable, size`. Both tables are `u32` offsets, entry 0 = `4*(N+1)`, entries 1..N = start of bank i-1 relative to the
  table (body table: sample bodies, the last entry is the end of the bodies = start of the header table; header table: the last header runs to the end of
  the file). Each header is a `pBAV` VAB: 32 byte header (`ver,id,size,rsvd16,ps,ts,vs,mvol,pan,a1,a2`), 128 program attrs x 16 B, `ps` x 16 tones x 32 B
  (`prior,mode,vol,pan,center,shift,min,max,vib..,pbmin,pbmax,2 rsvd, adsr1,adsr2,prog,vag,4 rsvd`), then `u16[256]` VAG sizes in 8-byte units. Entry k
  is the size of VAG k (1-based as stored in `tone.vag`; entry 0 = 0); the VAGs are stored back to back without header and their sizes add up exactly
  to the body size. 835 samples in 52 banks. Sample rate is not stored (SPU plays root note at 44100 Hz: WAVs are written at 44100, adjust with the tones' `center/shift`).
* ADPCM block (16 B): `shift = b0 & 15, filter = b0 >> 4` (coefs 0,60/-0,115/-52,98/-55,122/-60 over 64), flags byte (1 = end, 2 = repeat, 4 = loop start), 14 bytes of
  nibbles, low first. Loop start/end go into a WAV `smpl` chunk.
* SEQ: `pQES`, BE `version(1)`, `u16 resolution (0x1E0)`, `u24 tempo (us/quarter)`, `u8 num, u8 log2 den`, then plain MIDI events with delta times and running
  status up to `FF 2F 00`; wrapping them in `MThd`/`MTrk` (type 0) is the whole conversion. All 36 parse to the byte; lengths are whole bars.

### Which bank plays which SEQ, and the software sequencer
The load lists (`src/cdLoadTask.c`, the `FILE_SOUND_*` entries) load every `S_*.SEQ` right after its `SND*.WVD`; the names differ only in the `_` / `X`
variants. Table in `tools/audio.py` (`SEQ_BANK`): `S_TITLE`->`SND_TTL`, `S_SYS00`->`SND_SYS`, `S_END`->`SND_END`, `S_ARnnn`->`SND0nnn`, except
`S_AR010`->`SND001_`, `S_AR010X`->`SND001_X`, `S_AR100`->`SND010_`, `S_AR100X`->`SND010_X`, `S_AR040X`->`SND004_X`, `S_AR1802`->`SND0182`,
`S_AR140` -> `SND0140` (the game loads it with `SND0140`..`SND0146`; only the second bank differs). Bank 0 of the WVD is the music bank: the program
numbers of all 36 SEQs exist in it (program numbers = index into the 128 program attrs, tones are stored compactly for the programs with tones > 0);
bank 1 (if any) is a shared sound effect bank. The SEQs have no tempo events and no pitch bend; they use program change, CC7 (volume), CC6/CC99.
Loop: **CC99 = 20 (with CC6 = 127) is the loop start and CC99 = 30 the loop end** (Sony libsnd convention); 33 of 36 songs have it.
`render_song()` (numpy) mixes the notes: tones chosen by key range (all matching layers play), pitch `2^((key - center + shift/128 + bend)/12)` against the
44.1 kHz sample rate, linear interpolation, VAG loop flags, SPU ADSR from `adsr1`/`adsr2` (attack linear/exp, decay to sustain level, sustain mode, release;
closed form of the SPU step/shift counters), gain = velocity x CC7 x CC11 x program vol x tone vol x VAB master vol, pan = CC10 + tone/program pan offsets
(constant power). Not modelled: reverb, the 24 voice limit, vibrato/portamento, CC changes during a note. The loop body is played twice in total, the last 4 s fade
out (+1.5 s of release tails), peak normalised to 0.89. All 36 render in about 27 s. Checks: duration = tick length at the tempo (+1.5 s), onsets of the output
line up with the MIDI note-on times (onset flux at the MIDI onsets 3.6..5x the average, 1.5..2x when shifted by 0.25 s).

## `MOVIE/*.STR`
Raw layout (read from the image, `isoext.py` output mixes sizes): per 2352 byte sector subheader `file=1, channel=1`; video sectors are Form 1 (submode
`0x48`, 2048 B), audio sectors Form 2 (submode `0x64`, coding `0x01` = 4 bit stereo 37800 Hz), 7 video : 1 audio. Video payload = 32 byte header + 2016 B:
`60 01 01 80`, `u16 chunk, u16 nchunks, u32 frame, u32 demuxsize, u16 w, u16 h`, then `u16 nwords, 0x3800, u16 qscale, u16 version` (the BS header also repeats
at 0x20). Version 2 (e.g. LOGO, BOY) and 3 (100_?, ...) both occur; movies are 320x224, 110..1415 frames. Bitstream = LE 16 bit words read MSB first; 16x16
macroblocks column by column, blocks Cr, Cb, Y0..Y3; v2 block = 10 bit signed DC (coefficient 2*DC) , v3 = DC difference (MPEG-1 DC size VLC, Y shared) from 128,
then MPEG-1 intra run/level VLC (escape `000001` + 6 bit run + 10 bit signed level, EOB `10`), dequant `level*qscale*matrix>>3` (escape: `(x-1)|1`), 8x8 IDCT, +128,
YCbCr -> RGB (1.402 / 0.3437 / 0.7143 / 1.772). The pure Python decoder matches ffmpeg's mdec at 49 dB PSNR (v2 and v3). XA ADPCM audio decoded by
`audio.py` is bit-identical to ffmpeg's `adpcm_xa` (all 30 movies, 717 s in total, none silent). Old summary:
Standard PlayStation XA/STR video (30 files, 2.3 MB..29.5 MB; `100_?` per language). The extracted files hold the 2324-byte form 2
sector payloads, so they start at the video frame header: `60 01 01 80` (`0x0160`, `0x8001` = STR video sector), chunk 0 of 9, frame 1,
demux size `0x09E4`, **320 x 224**, `0x3800` = BS v2 frames (MDEC). In the file table the size is stored as sectors x 2336
(`notes/cdfiles.md`). Play/convert with jPSXdec or ffmpeg; no work needed here. Dump:
`MOVIE/LOGO.STR` (2382380 bytes)
```
0000  60 01 01 80 00 00 09 00 01 00 00 00 e4 09 00 00
0010  40 01 e0 00 a0 06 00 38 01 00 02 00 00 00 00 00
0020  a0 06 00 38 01 00 02 00 20 00 7f 02 f2 27 27 7f
0030  00 f2 02 20 27 7f 7f f2 f2 27 20 00 7f 02 f2 27
```

**AVI export (`str.py avi`, `extract_sprites` step `videos`, no ffmpeg needed).** Frame rate: XA is read at 150 sectors/s (double speed, 37800 Hz stereo
needs it) and a movie spends the same number of sectors (video + 1 audio per 7 video) on every frame, so `fps = 150 / round(sectors / frames)`: all 30 movies have
10 sectors/frame = 15 fps (2520 audio samples per frame), so audio (`n_audio_sectors * 2016` samples) and video stay in sync (100_?: 466 frames = 31.07 s video,
31.20 s audio). The AVI is RIFF `AVI ` / `LIST hdrl` (avih, strl vids `MJPG` scale 10 rate 150, strl auds PCM 16 bit stereo 37800) / `LIST movi` (per frame one `00dc`
JPEG + one `01wb` chunk with that frame's audio; the audio left over goes after the last frame) / `idx1` (offsets from the `movi` fourcc). Verified by parsing it back
(frame and chunk counts, every idx1 entry points at its chunk) and with ffprobe/ffmpeg (mjpeg 320x224 15 fps 466 frames + pcm_s16le, decodes clean).
JPEG encoding is Pillow's (the decoder hands over YCbCr planes, chroma 4:2:0, quality 90, so no RGB conversion in Python); without Pillow the AVI is skipped with a log
line. Speed: `decode_ycc` caches the IDCT basis image of each (coefficient, level) pair, so a block is a C-level `sum(zip(*terms))` + one clamp-table lookup, the bit
reader is inlined; 0.115 s -> 0.045 s/frame (v2) and ~0.065 s/frame (v3, more coefficients), about 0.07 s/frame with JPEG: ~10 min for all 8,400 frames.

## Mods (tools/mod.py)

`mods/<name>/mod.json` lists `patchs` (`file` = PNG in the mod folder, `targets` = `pattern` (fnmatch on the path under `game/disc/fs`, e.g. `AREA00/B301.000`) and an optional `frame` for WPP sprite files); `mods/load_order.json` `{"order": [...]}` sets the order (later wins). PNG to data: 8 bpp page = indexed 2*w x h, 4 bpp = indexed (<= 16 colours) 4*w x h, 16 bpp/CLUT = RGBA w x h (5 bit linear, alpha 0 = word 0, opaque black = 0x8000); WPP frame = indexed 4*W x H. Patched size must equal the original. `.GAM` targets go through `tools/gam.py`, WPP through `wpp.py`; `mod.py build` runs `mkdisc.py build --relayout`. The mod layout idea comes from TombaPatcher (US-only; no code reused).

Colour notes from comparing with TombaPatcher: it decodes 5->8 bit with a non-linear curve and shows STP as half alpha, and draws pages without the load-list CLUT pairing (grey ramp); ours uses the linear 5 bit expansion (hardware) and the VRAM replay pairing. `area000.py page` now picks the 256 colour CLUT record after the page like `vram.pair_cluts`. Room pages still need the per-tile CLUT row from the room data (row 0 is only a default).


## WMD rooms (room backgrounds; `tools/room.py`, `area000.py room AREA00:1 out.png`, `extract_sprites` -> `rooms/`)
The room background is NOT a tilemap over the `Bnnn` page: it is the kind `0x50` WMD model of the room load list (`AREA00/D001.GAM`, ...),
textured polygons that sample the 4 bpp / 8 bpp room pages (`Bnnn`) and carry their own GPU words, so the CLUT row is per polygon in the data.
- u32 table container, N sectors (vertical slices of the room, ascending x). Sector = 4 groups `u32 count` + packets (flat tri, flat quad,
  gouraud tri, gouraud quad), then 16 zero bytes.
- Packet: `u16 clut, u16 tpage, r g b cmd` (cmd 0x24/0x25 tri, 0x2c/0x2d quad, 0x34 gouraud tri...: bit 3 = quad, bit 4 = gouraud, which inserts
  2 (tri) / 3 (quad) extra `r g b cmd` words), then 3/4 vertices `s16 x, y, z; u8 u, v` (8 B).
- `clut` = real GPU word (x = (c&63)*16, y = c>>6; room palettes are 16 colour rows of `CLUTnn` at y 480..511), `tpage` = real GPU word
  (page x = (t&15)*64, y = (t>>4&1)*256, mode bits 7-8; rooms use 4 bpp, no offset needed). That is why B000 looked wrong with any single row.
- Renderer: orthographic (x right, y down), polygons sorted by mean z far to near, word 0 transparent. Verified visually: AREA00 D001 (village,
  hills, river), AREA03 D004, AREA10 D303: natural colours. The in-game camera/perspective is not reproduced.
- 3D export (`room.py --glb AREA00:1 out.glb`, `--glb-all DIR`; `extract_sprites` -> `rooms3d/`): binary glTF 2.0, one mesh, vertices not shared
  (uv differs per polygon). Atlas = one 256x256 tile per distinct (tpage, clut) (4/8 bpp decoded with that CLUT, word 0 -> alpha 0), 8 tiles per row,
  embedded PNG, uv = (tile origin + u + .5) / atlas size; alphaMode MASK, doubleSided, NEAREST. Quad -> tris (0,1,2) (1,3,2). `COLOR_0` = packet
  rgb (0x80 = 1.0, so doubled, clamped) per vertex (flat = first colour). Position = (x, -y, -z) * 0.01 (y down -> up; z negated keeps handedness).
  15 bpp polys (tpage mode 2) have no palette and are mapped to a transparent texel. Verified: AREA00:1 / AREA10:1 parse back (accessor counts,
  min/max, uv in 0..1) and reproject upright and textured from a 35 degree yaw.
- All 66 room models of the 17 areas render (`--all` and `--glb-all`: 66/66; the earlier 27 are byte-identical). There was no second packet
  variant: the container is `[u32 N][u32 off * N]` with NO terminator (sector i ends at off[i+1], the last at the file end). The old parser read
  N+1 offsets, so with N = 1 (small models: AREA04/12 D4xx..DIxx, AREA13 D102, some AREA14) it took the first sector's `u32 count` as the end
  offset, and for the last sector of larger models (AREA03 D105 etc.) it cut the sector short. The small models are single objects (plants,
  props), not room slices.

### Far background (sky / distant scenery): investigated, NOT resolved
Findings (no tool written; nothing extracted):
- Room load lists hold only: `Bnnn` GRPX pages + `CLUTnn`, the kind `0x50` WMD file and its sub-pieces. The decompressed `Dxxx.GAM` is the WMD (`arg` bytes)
  followed by the sub-pieces named `SYS/LD0SYS.BIN` (type -1): `0x51` = table of 79 small object models (rgb `3c` flat polys, not a background),
  `0x40..0x45` ("FOUR", 2088/4172 B records), `0x80..0x82` APD, `0x70` WHM. None looked like a background image or layer.
- The `Bnnn` pages with no WMD polygon on them (AREA00 `B000`, AREA03 `B000..B002`, AREA10 `B000/B001`...) are 8 bpp enemy/NPC sprite sheets
  (viewed with the room CLUT, rows of animation frames), NOT the sky. Pages shared with WMD polygons are the 4 bpp room textures.
- The far scenery art does exist in VRAM: in AREA00 the area page `A002` (x 576.., y 0..255, 4 bpp) holds cloud banks, mountain/horizon strips and
  gradient bands next to effect sprites, i.e. the sky is composed of sprite/tile parts. The placement data (which strips, scroll factors, sky fill colour)
  was not found: `src/` has no matched background drawing code (only `scriptOpLoadBackground`, which stores a value and calls `func_800EBD5C`).
- Next step: trace `func_800EBD5C` / the overlay (`X0nn.BIN`) draw routine that links POLY_FT4/SPRT before the WMD in the OT, or break on the
  GPU primitives in an emulator trace, to get the strip table + CLUT rows; then composite behind `room.py` output.

**`A00701` (worldmap_texture).** LoadImage rect 256 words x 256 rows at word 192 (= 512 x 256 px, straddles 8 bpp texture-page boundaries at words 256 and 384), a single 8 bpp CLUT (`A00702`); there are no mixed bpp regions or extra CLUT rows. Decoded as a continuous 512 x 256 image it shows the island map in the left ~350 px and unrelated tiles (cloud/sky strip, forest/terrain pieces, red markers) at the right/bottom: it is a texture atlas whose pieces the map code places by UV, so `worldmap_texture.png` is the best static decode (pending the map sprite/UV tables).

**Title logo / world map: not in SPR tables (searched).** No SPR table of any SYS load list exists (SYS lists hold only GRPX/CLUT records), and the area SPR tables contain no 8 bpp parts that reference the A00202 (tpage 0x84, word x 256) or A00701 (words 192..447) regions except coincidental area sprites (AR02/05/08/09/19 tpages 0x84/0xC4/0xE4 etc., different page data per area). The title screen is drawn by MAIN0 `loopTitleScreen` (0x8004e41c) via overlay routines at 0x800e972c (logo, called with 0x30,0xd0,1), 0x800e9d08 (menu) and 0x800e8a94, which are not in MAIN0/`X000_BIN` decomps (overlay code not yet disassembled). `D_80076E40` in `src/titleSequenceTask.c` is only the 3 landscape strips (16 bpp tpages 0x80/0x82/0x84), not the logo. The (u,v,w,h,x,y) rects for the TOMBI! logo and world map pieces most likely live in that overlay's code/rodata: next step is to disassemble the overlay at 0x800e972c.

## "NOW LOADING" graphic and "TRY AGAIN" prompt (verified visually)

**NOW LOADING** = `SYSTEM/A00900.000` (SYSTEM load list 9, `FUN_8004fa80(9,1)`; 64 words x 72 rows = 256 x 72 px, 4 bpp, VRAM (0,0)): big "Now / Loading" letters at top-left (about 0..110 x 0..64) plus two pig sprites on a coloured block. It has **no CLUT record**: the palettes live in the file itself, in the last row (y 71, as 64 words): three 16-colour palettes at row words 16, 32, 48 (file word offset `71*64 + 16*k`, k = 1..3): k=1 colourful letters / blue block (default in `tools/screens.py`), k=2 green/yellow, k=3 pink. **Ground truth:** `UiSpriteDef` 0 = `{0,0,112,71, clut (16,71)}` names exactly the first embedded palette (k=1, row 71 word 16), so that is the one the game uses. Rendering NOW LOADING with the TRY AGAIN CLUT (352,509) gives the same letter colours but muddy pigs/block (tried, rejected). `tools/screens.py now_loading.png` and the gfx entry `now_loading` show k=1 (crop 0,0,256,64). To mod: `{"file": "page.png", "targets": [{"pattern": "SYSTEM/A00900.000"}]}`, indexed 4 bpp PNG 256 x 72 (keep row 71 = palettes untouched, or edit the palette words there).

**TRY AGAIN** (not NOW LOADING; an earlier note was wrong): the `UiSpriteDef` table at `0x80078808` (12 bytes `{u16 u,v,w,h; s16 clutX,clutY}`, entries 1 and 2 valid) cuts "TRY" (184,192 40x16, drawn at (104,96)) and "AGAIN" (184,208 56x16, at (160,96)) from `SYSTEM/A10003.GAM` (VRAM (256,256) = tpage 0x14; 0x24 = (256,0) abr 1 on `GetGraphType` 1/2) via `FUN_800212b4` (`drawNowLoading` in src, misnamed). The CLUT (352,509) is row 29 (words 224..239) of the 256 x 32 area CLUT strips (`AREAnn/CLUTxx.GAM` at (128,480); identical in all areas), NOT `A00802` (that one is only loaded by list 8 and has no transparent index 0). With that palette the letters read TRY / AGAIN in red/yellow/green/pink/blue. `tools/screens.py try_again.png`, gfx entry `try_again`. Mod: target `SYSTEM/A10003.GAM` (256 x 256 4 bpp page, keep the data inside the two rects).

## Graphics editor (tools/gfx_core.py, exe tombi-gfx.exe)

- **UiSpriteDef table (MAIN0 0x80078808, file offset 0x69008):** decoded entry by entry until the fields stop being plausible: only **3 entries** exist, `0 = {u0 v0 w112 h71 clut(16,71)}` (placeholder, no known user), `1 = NOW {184,192,40,16 clut(352,509)}`, `2 = LOADING {184,208,56,16 clut(352,509)}`; the bytes after them are other data (so there is no bigger "UI sprite" table at this address; the HUD art is drawn from SPR tables). CLUT (352,509) is located by VRAM address inside the load-list CLUT records (`PalRef` = file, first word, colour count): `A00802.000` word 224.
- **Page bit depth** (`vram.page_bpp`): 16 for palette blocks (`is_clut_rec`), 8 when `pair_cluts` pairs the page with a 256 colour CLUT record of its list (title/logo/menu screens in SYSTEM, room pages with `CLUTnn`), else 4. `tools/mod.py` uses it now (before, SYSTEM 8 bpp screens were taken for 4 bpp because their CLUT is not called `CLUT*`).
- **STP bit in mod PNGs:** a 16 bit PNG patch cannot carry bit 15 (72117 of the 366 k non-zero CLUT words have it). `mod.png_words(..., orig)` keeps the STP bit of the word it replaces (opaque black = 0x8000, transparent = 0), so a CLUT exported from the unchanged file patches back byte-identical and an edited colour keeps its slot's STP.
- **GAM writing without Go:** `tools/gam_py.py` encodes with the body rules of notes/assets_tombatools.md (greedy, min 5, window 255, no self-overlap) but not the zero-pad end rules; unchanged data keeps the original bytes. `mod.py` uses it, so mods apply on Windows. Changed streams are valid (decoded back by `area000.gam_unpack`) and may differ from the original tool in the last ~260 bytes.
- **WPP frame edits** must re-encode to exactly the original file size (the load record `arg` and the tables are not rewritten); otherwise the editor refuses.

## Room backdrops (SYSTEM lists 20, 22, 23, 42, 48, 51-54, 62, 63, 77-79, 81-85, 88, 90) and palette inference

These SYSTEM lists have no CLUT record and the pages are **8 bpp**: pages at VRAM word x 0x80, 0x100, 0x180... (128 words = 256 px each); the first two pages stitched hold a pre-rendered 320 x 240 room picture, the rest are object/animation textures of that room (list 51: palace hall with arches). The 256 colour palette is row 31 (list 63 and 90: row 30) of one of the `CLUTnn.GAM` files of the area (list 51 -> `AREA09/CLUT03`, 20 -> `AREA02/CLUT03`, ...); `tools/palinfer.py backdrop()` finds it by smoothness (adjacent-pixel colour distance) among rows 24..31 of the area's CLUT files and the gfx editor lists them under Screens / Room backdrops (`backdrop_L<n>`). Names: 20 cell with wooden bars, 22 forest clearing, 23 stone door with a face, 42 seashore with a giant clam, 48 forest pier by a lake, 51 palace hall with arches, 52 hall with red carpet, 53 cave staircase, 54 brick staircase, 62 wooden pen, 63 cave with brick tower, 77 clock tower, 78 bell tower, 79 gear room, 81 copper castle, 82 hall with arches, 83/84 hall with shield doors, 85 boiler room, 88 workshop interior, 90 mushroom hut in blossoms. Lists 50 and 60 also start with a backdrop (house on a floating island / wizard study) but are not detected yet (first page too sparse for the density test). Lists 83, 86, 89, 91 are not maps: 86/89/91 are object and room texture atlases (89 cages/huts/orbs, 91 props, eggs, smoke), 8 bpp with a run time palette.

Note: `FUN_800212b4` (src name `drawNowLoading`) is really the TRY AGAIN prompt drawer (would be `drawTryAgain`); src/ not renamed here.

`tools/palinfer.py` (4 bpp pages without CLUT): methods embedded (A00900 row 71), sprites (SPR parts inside the page vote for the CLUT), match (WPP tiles, off), score (smoothness).

### Backdrop stitching and missing CLUT block (verified on all 21 backdrops)
- The 320 px room picture is page 0 (256 px) **plus the first 64 px of page 1, drawn at the LEFT edge**; `Core._backdrop_surface` rebuilds the sheet as `[strip of page 1][page 0][rest of page 1][other pages]` (Surface layers accept a source window `(name, x0, w, h, sx, sw)`). Page 0 of L90 is already 512 px wide: no strip.
- L22 (forest): its CLUT row (AREA02/CLUT05 row 31) has 16 empty words (indices 176..191); no CLUT of the disc holds them (filled at run time), so `palinfer._fill_missing` estimates the used ones from neighbouring pixels (not original data). Backdrops render opaque.
- Editor palette list: inferred / list CLUTs / embedded / resident / every disc CLUT file; the choice is stored per page in `mod.json` (`tombi_gfx.palettes`).

## MIPS capture (tools/psxemu)

Composed screens are captured by **running the game**: `tools/psxemu` is a pure Python R3000A interpreter (basic blocks compiled to
Python, ~4 M instr/s; GTE after psx-spx; no load delay slots / interrupts / caches) with HLE of the BIOS and Psy-Q libraries by address
(`notes/names_main0.csv` + `functions_main0.csv`):

- BIOS B0 OpenTh/ChangeTh/CloseTh = the task system's threads; OpenEvent(RCntCNT3, func) = the VBlank handler, fired every 25 000
  instructions (and by VSync).
- libcd: CdControl(Setloc) / CdRead read 2048 byte sectors of `game/disc/Tombi.bin`; the game's own `cdLoadTask` (load lists,
  lzDecompress, LoadImage, overlays at 0x800E8028) runs natively.
- libgpu: LoadImage/StoreImage/MoveImage/ClearImage/PutDrawEnv (isbg clear)/DrawOTag... -> `psxemu/gpu.py` (numpy VRAM, SPRT/TILE/
  POLY F/FT/G/GT 3/4, lines, E1..E6, semi transparency, CLUT 4/8 bpp, modulation). Every frame is rasterized (the dialogue font is
  rendered into VRAM and used as a texture, so skipping frames breaks text).
- stubs: libsnd/libspu/MDEC/card/pad libs; the FMV task 0x8001D6A4 sets MOVIE_PLAY_STATE (0x1F8001CC) = 0 and exits.
- pad: PadInitDirect buffer 0x8009F7F0 (status 0, id 0x41, buttons active low) written by `Machine.set_pad`.
- `Machine.save_state/load_state` pickle RAM, registers, threads and VRAM.

`tools/psxemu/capture.py title|area00|worldmap OUT.png`, `tools/screens.py OUT --composed title|all`:
- **title** (frame ~906, ~31 M instructions, ~10 s): DRAWENV 320 x 256 at (384,256)/(704,256). Landscape = 2 SPRT 16 bpp (tpage 0x80
  at x 0 256 x 240, tpage 0x82 at x 256 64 x 240, from y 8); **TOMBI! logo = 2 SPRT 8 bpp, tpage 0x84 (VRAM word 256), CLUT (128,510)**:
  (16,24) uv (0,8) 240 x 112 and (256,24) uv (8,112) 48 x 112 (the "!" + head piece). Text = 16 x 8 SPRT (tpage 6, CLUT row 480+);
  the NEW GAME / LOAD drum = POLY_FT4 strips (cmd 0x2C) squeezed toward its edges. The logo drops in: it is complete after ~150 frames.
- **area00**: START on the title, 500 frames: first room of the Village of All Beginnings (sky/far background + 3D + dialogue), ~480 prims.
- **worldmap**: the dialogue clicked through with CROSS, SELECT -> menu (Item/Event/Status/Map) -> DOWN x3 -> CROSS (SYS list 7 =
  A00701 map atlas); exact frame timings in `capture.TALK/TO_MAP` (deterministic). ~8 min.
- **rooms** (`capture.py rooms OUT [--area N] [--section N] [--jobs 4] [--force]`, `screens.py OUT --composed rooms`): every area / room as
  `room_<AA>_<n>_composed.png` (HUD / dialogue slots hidden) and `room_<AA>_<n>_sky.png` (far background alone; only for rooms that have one).
  - **Debug area select (the game's own)**: `displayDebugScreen` / `debugSelectHandler` (gameplay mode 0): with `scratchpad.debug_mode_enabled`
    (**0x1F8001B4**) = 1 at NEW GAME the game shows "AREA SELECT / SECTION SELECT" instead of starting; CIRCLE / START loads the pair through the
    normal loader (`areaTransition` flag 0 -> full area list). We set the flag before START, save the machine at that menu (`rooms_base.state`)
    and per room poke `GAME.selectedArea` (**0x8009C960**, u16) / `selectedSection` (**0x8009C962**) and press CIRCLE (spawn point
    `selectedSpawnPoint` 0x8009C982 stays 0). Sections per area: u16 table at **0x80078790** (index = area; 0x8007878C = highest area, 23); the
    areas on disc are 0..6, 8..11, 13, 14, 16..19 (7 = purified Dwarf Forest, 12, 15 do not exist): 109 rooms in all. Room n = load list n + 1 of
    `LD0ARnn` (list 0 = area list). Door / warp transitions use gameplay mode 7 (`areaTransitionHandler`): `gameConfig.nextArea / nextSection /
    nextSpawnPoint` = GAME + **0x434 / 0x436 / 0x440**, then task mode := 7, step 0 (not used: see failures).
  - **Game task** (fixed) **0x801FD800**: +0x4A = 1 gameplay, +0x4C = mode (0 debug select, 1 / 2 / 4 / 5 / 6 = area modes of `areaMode*Handler`, 7 =
    transition), +0x4E = step (1 = running); `0x1F8001CF` = room loaded. A room is "stable" 60 frames after mode in (1,2,4,5,6) and step 1 (~120-140 frames
    after CIRCLE; after ~100-200 more frames Tombi can die and the debug select comes back, so capture early).
  - **Layers**: `Machine.frames[i] = (drawenv, packets, dispenv, ot_marks, ot_head)`; the depth of a primitive = ordering table slot (head - mark) / 4, 0 =
    farthest. The OT has 808 slots (ClearOTagR). **Slots 0..~10 = sky / far background** (area overlay: textured sprite rows `0xE1` + `0x65` and
    poly strips, or the whole pre-rendered room picture in the static rooms); **slots ~530..800 = the 3D WMD room polys, objects, Tombi and effects**
    z-sorted together; **slots 805..807 = HUD, dialogue box and text**. Rooms with no far layer (e.g. Phoenix Mountain) have nothing in slots < 100
    and get no `_sky.png`. `capture.render_frame(m, keep)` re-rasterizes a recorded frame with a primitive filter.
  - **Speed**: only the final frame is rasterized (`m.render = False` while loading; VRAM textures come from LoadImage anyway); ~12 frames / s ->
    ~15 s per room, all 96 rooms in ~3 min with 4 processes (one machine per process, same base state).
  - **Not captured (13 rooms)**: rooms whose load list is empty (`LD0AR11` sections 0, 3, 4; `AR16` 0..2; `AR17` 0, 6..10; `AR18` 0) inherit the area
    overlay and resources of the room the player comes from (X011 etc.), and the game itself hangs on an empty list (`cdLoadTask` waits for a
    queued file, LOAD_COMPLETE never set) unless the area changed. Straight from the debug select the overlay is missing and the interpreter runs
    into zeroed RAM; entering them through a mode 7 transition from a sibling room also ends in a bad object pointer (objects of the previous area).
