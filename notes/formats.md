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
tail 1 byte; item 0 starts `88 0a 00 30 1e 00 90 cb ...` (nibble-packed pixels with `00 nn` zero-run escapes: a 4 bpp RLE, to be confirmed
against the sprite renderer). Files read with `LOAD_TYPE_RAW_4` hold **two containers back to back**; the record `arg` is the
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

Open: the WPP frame encoding (offsets per frame are known; the pixel codec and the frame header are not), WMD model layout, the
`0x8F` block, what the grid values mean, and the exact VRAM layout needed to pair each page with its `CLUTnn.GAM`.

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
