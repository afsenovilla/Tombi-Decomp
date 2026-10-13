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
| `SOUND/*.WVD`, `SOUND/*.SEQ` | 52 + 36 | **Partly** (WVD container read from the loader; SEQ is standard Sony `pQES`) | section below |
| `MOVIE/*.STR` | 30 | Standard XA/STR, wrap as opaque | section below |

Everything in the disc image is reached through the file table inside `EXE/MAINn.EXE` (`notes/cdfiles.md`), whose entries are
`LoadRecord.fileId` indices; the `FILE_*` names in `include/tomba24/cdfiles.h` come from the US build and are **wrong for PAL**
(for example id 334 is `FILE_AREA00_A005_000` there and `AREA00/A000.GAM` here). Resolve names with `python3 tools/cdfiles.py dump
game/disc/fs/EXE/MAIN0.EXE`.

## WFM (done)
Font glyph bitmaps plus the dialogue/menu text of one area and language. Fully handled by `tools/wfm.py` (decode to
`dialogues.yaml` + glyph PNGs, encode back, 369/369 byte-identical). See `notes/assets_tombatools.md` for the
Spanish glyph additions (`fonts/es`). Loaded with `LOAD_KIND_WFM3`.

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

Open: WMD model layout, the `0x8F` block, what the grid values mean, and which CLUT row each room tile uses.

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


### SPR / VRAM: every graphic as PNG (`tools/vram.py`, `tools/extract_sprites.py`)
`python3 tools/extract_sprites.py game/disc/Tombi.bin OUT` writes `OUT/sprites` (WPP frames, `tools/wpp.py`), `OUT/spr` (SPR atlas cuts) and
`OUT/screens` (pages and VRAM snapshots); `python3 tools/vram.py --all OUT` does the last two from an extracted `game/disc/fs`.
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
- `OUT/screens/<AREA|SYSTEM>/listNN/vram.png` = the whole 1024 x 512 VRAM of the list as RGB555 (8 bpp data looks like noise there; that is the 16 bit view): AREA lists on top
  of the resident pages, SYSTEM lists alone (231 snapshots). Next to it every GRPX record: `NAME.png` for 8 bpp pages with the CLUT the list pairs them with (318;
  row 0 of a 256 x 32 block, or the palette their 8 bpp SPR parts use), `NAME_4bpp.png` / `NAME_8bpp.png` for atlases coloured with the palette most SPR parts use on them (72),
  `NAME_16bpp.png` + `NAME_4bpp_gray.png` when nothing says how a page is read (131), `NAME_palettes.png` for palette blocks (141, each colour 4 x 4 px).

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

## `MOVIE/*.STR`
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
