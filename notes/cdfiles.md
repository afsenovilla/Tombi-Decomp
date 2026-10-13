# File table inside the executables (PAL Spanish)

The game does not use the ISO9660 directory to load its data: it seeks to a position stored in a table inside the executable
(psx_tomba: `FILE_TABLE`, `CdControlF(CdlSeekL, table + id * 8)` in the cd queue code; in our tree `src/enqueueCdRead.c`,
`src/cdLoadTask.c`, ids in `include/tomba24/cdfiles.h`). Tool: `tools/cdfiles.py`.

## Where
- `EXE/MAIN0..8.EXE` (and `game/MAIN0.EXE`, byte-identical to `EXE/MAIN0.EXE`): the table starts at **file offset 0x6E6F0**
  = vaddr **0x8007DEF0** (load address 0x80010000, header 0x800), in all nine. The US table was at 0x800791A0 (other exe layout).
- `SCES_013.31` has **no table**: no LBA, MSF or size of any file appears in it. It only holds the strings `\EXE\MAIN0/5/6/7/8.EXE;1`
  and starts the MAINn by name (CdSearchFile), so the ISO directory entries of `EXE/MAINn.EXE` must stay valid (mkpsxiso writes them).

## Entry (8 bytes, little endian)
| Offset | Size | Meaning |
|---|---|---|
| 0 | 4 | `CdlLOC`: BCD minute, second, frame of **LBA + 150**, then 0 |
| 4 | 4 | size in bytes (ISO file size). For `MOVIE/*.STR`: **sectors x 2336** (raw payload per sector), i.e. `iso_size / 2048 * 2336` |

Example: entry 0 of MAIN0 = `36 56 38 00 | 68 1E 00 00` = 36:56:38 -> LBA 166088, 7784 bytes = `SYS/LD0SYS.BIN`.
The index is the file id used by the load records (`LoadRecord.file`). Every entry was verified: LBA and size equal
what the original image says (ISO directory, 2287 files), for all entries of all nine executables (`tools/cdfiles.py check`).

## Order and count (per executable)
Each MAINn has its **own order**; they are not the same table with other values. MAINn uses the variant `n` of the language/ending files:
the first entries are `SYS/LDnSYS.BIN`, `SYS/LDnARxx.BIN`, then `SYSTEM/*` (with `A0xxxx` for n=0, `A1xxxx` for n=1 ... where they exist,
`OnPTSUB0.BIN`, `DnSPSUB.BIN`, `XnNN.BIN`, `InNFO*.BIN`, `GnOVER.BIN`), `SOUND/*`, `MOVIE/*.STR` (22 of the 30 in MAIN0), then
`AREAxx/*` in area order. After the last entry other data follows (a long entry-like run is not a table).
- MAIN0, 1, 2, 3, 4, 6, 7, 8: **1119 entries** (MAIN0 and MAIN1 differ in 135 entries, which are the variant files).
- MAIN5: **1116 entries** (the beginning is `SYS/LD5AR02.BIN`; no LD5SYS/LD5AR00/LD5AR01).
- No file appears twice in one table. The table end is not marked: the tool takes the count from a pristine copy of the executable
  (entries match the original layout up to the entry that follows the table).

## Referenced and not referenced
Union of the nine tables: **2196 of 2287** files referenced. Not referenced by any table (91): `EXE/MAIN0..8.EXE`, `SCES_013.31`,
`SYSTEM.CNF`, `ZZZ/DUMMY.DAT` (the 27.6 MB filler), `SYS/LD5AR00.BIN`, `SYS/LD5AR01.BIN`, `SYS/LD5SYS.BIN`, `SYSTEM/X019..X819.BIN`
and `AREA19/X019..X819.BIN`, `AREA11|16|17|18/I?NFO2.BIN`, `AREA11|15|16|17|18/CLUT*.GAM` (some). They may be reached another way
(the executable names above come from the ISO directory); that has not been checked for the rest.

## Layout invariants for a rebuild
- A table only needs the new LBA/size of every file it lists; the order of the files on the disc may stay as is, mkpsxiso moves the
  following LBAs when a file grows (tested). The `<dummy>` gap after `ZZZ/DUMMY.DAT` and the sector size of the STR files are
  unchanged by it. `MOVIE/*.STR` size field: sectors x 2336 (see above).
- The executables inside the image are in the table-free part of the layout (same size), so patching them does not move anything:
  `mkdisc.py --relayout` builds once, patches, builds again and the second pass changes 0 entries.

## Open points
- Other embedded LBAs (overlays in `SYS/LD*.BIN`, `SYSTEM/*`) were not searched beyond the executables and `SCES_013.31`.
- Not run on hardware or an emulator in this session.
