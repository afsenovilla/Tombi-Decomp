# Tomba! 2 (USA demo) vs Tomba! 1

Question: do Tomba! 1 and Tomba! 2: The Evil Swine Return share an engine? Source: "Tomba! 2 - The Evil Swine Return
(USA) (Demo)", data track extracted with `tools/isoext.py`. Compared against T1 PAL (`game/disc/fs`, 816 GAMs unpacked
with `tools/gam.py`), T1 JP retail (fs extracted from `game_jp/disc`), beta 980203 and PrePre 8. The JP demo
(`game_jpdemo/`) is not present in this checkout and was not compared. Measurements: `tools/tomba2_demo.py`
(`code`, `reverse`, `blocks`, `strings`, `entropy`) and `tools/xmatch.py`. No game data is in the repo.

**Short answer.** At the binary level the demo shares almost nothing with T1 beyond the Sony libraries: one 104-byte
T1 game function (`tickTaskTimers`) is present instruction for instruction, the rest of the T2 game code does not match
T1 functions, and none of T1's file formats or containers are used. This does not prove T2 was written from scratch
(heavily edited or recompiled source would also fail these tests); it only shows that the T1 code and data were not
carried over unchanged.

## Verified (measured)

### Disc / file structure
The disc is a multi-game demo disc: `SCUS_944.64` (= `PSX.EXE`) boots a menu `MAIN.EXE` + `MENU.FF`; the game is in
`TOMBA2/`.

| | Tomba! 1 (PAL) | Tomba! 2 demo |
|---|---|---|
| main exe | `EXE/MAIN0..8.EXE` | `TOMBA2/MAIN.EXE` (text 0x80018000, size 0xAD800, entry 0x80082E98) |
| overlays | `AREAnn/X0nn.BIN`, `SYSTEM/I0NFO*.BIN` etc., raw code at 0x800E8028 | `TOMBA2/BIN/{A00,SOP,OPN,GAME,DEMO,START}.BIN`; all but OPN start with a small u32 (0xA, 0xB, 8, 7, 6) and a pointer table; load addresses not determined |
| data | one file per resource: `.GAM` (LZ, `GAM\0` magic), `.WFM` fonts/text (`WFM3`), `SYS/LD*.BIN` load lists, `SOUND/*.WVD` + `.SEQ` | packed `TOMBA2/CD/TOMBA2.{IDX,DAT,IMG,SND}`; IDX = 96 KiB mostly-zero offset table; DAT starts with (u16 count, u16 sector) pairs; IMG starts with count + VRAM rectangles (x, y, w, h, size) |
| sound | WVD (custom wrapper around a VAB) + SEQ | SND contains plain `pBAV` (3) and `pQES` (10) |
| video/audio | `MOVIE/*.STR`, XA | `CD/LOGO.STR`, `CD/UNDER.XA`, `MOVIES/ENDCAP.STR` (disc menu) |
| text | WFM-encoded | plain ASCII in `TOMBA2/MAIN.EXE` (menus, item names) and `SOP.BIN` (intro text) |

Magic search over all demo files: no `GAM\0`, no `WFM`, no `LD0`/`LD1` lists, no T1-style WVD header. Only Sony
`pBAV`/`pQES` and one TIM header inside `MAIN.EXE`.

### Psy-Q
`$Id` strings: T1 PAL `sys.c 1.129 (1996-12-25)`, `bios.c 1.81`, `intr.c 1.74`; T2 `sys.c 1.140 (1998-01-12)`,
`bios.c 1.86`, `intr.c 1.75`, plus "Library Programs (c) 1993-1997". T2 uses a newer library release. The demo menu exe
has the same T2 versions.

### Code matching (`tomba2_demo.py code`)
8336 T1 functions with a size, all modules (main0, x0nn, i0nfo*, d0spsub*, g0over, o0ptsub0), masked as in `xmatch.py`
(call targets, `lui` and non-`$sp` offsets zeroed). Library = Psy-Q rows of `tools/units.py` plus libpad/libapi names.

| T2 file | exact masked hits | of which library | non-library | fuzzy pieces >= 48 B | best Jaccard < 30 % |
|---|---|---|---|---|---|
| `TOMBA2/MAIN.EXE` | 135 (16920 B, 24 byte-identical) | 131 (16728 B) | 4 (192 B) | 1052 | 903 |
| `A00.BIN` | 1 (36 B) | 0 | 1 (36 B, `setBox33x66`) | 390 | 388 |
| `SOP/OPN/GAME/DEMO.BIN` | 0 | - | - | 83 | 83 |
| disc-menu `MAIN.EXE` (control) | 84 | 82 | 2 (80 B) | 233 | 149 |

- The 4 non-library hits in T2 MAIN.EXE: **`tickTaskTimers` (104 B)**: same 26 instructions, only the table address
  differs (T1 0x801FD800, T2 0x801FE000; 3 entries of 0x70 bytes in both). The other three (`startAreaBgm` 32 B,
  `startAreaBgmWithReverb` 36 B, x009 `resetToB04One` 20 B) are 5–9-instruction call stubs; `startAreaBgm` also
  "matches" in the disc-menu exe (other developer), so these are not evidence.
- Fuzzy pieces with best Jaccard >= 30 %: 149 in T2 MAIN.EXE, 146 of them best-match a library function. The 3 others
  are 48–96 B (sim 77/45/30).
- Reverse (`tomba2_demo.py reverse`): 2577 distinct non-library T1 functions >= 32 B against all T2 code:
  < 30 %: 2544, 30–59 %: 26, 60–89 %: 3, >= 90 %: 4 (all four are 8–9-instruction stubs; one is `SsUtReverbOff`).
  The window split cuts at prologues only, so leaf functions are glued to neighbours (this is why `tickTaskTimers`
  shows up in `code` but not here).
- No match for T1's object system (`ObjAlloc`, pools), GTE/model helpers (`drawModel*`, `applyLighting`), text
  renderer, collision (`playerPushOutOfObject*` only as an 8-instruction thunk), task scheduler (except the timer tick).

### Top-down engine / civilization
No T2 code piece matches any I0NFO2 (top-down) function above 53 % (one 48-byte piece in A00.BIN). No strings about
towns/civ/top-down beyond normal T2 location names. Only one demo area overlay (A00) exists, so this says nothing about
the full game.

### T1 leftovers
- Raw 64-byte block hashing of every demo file against T1 PAL fs, unpacked PAL GAMs, JP fs, beta, PrePre 8: hits only in
  executables (library code), `TOMBA2.DAT` 6 blocks (arithmetic `01 00 xx xx` sequences, coincidental), `TOMBA2.SND`
  1 block (VAB attribute pattern), `A00.BIN` 1 block (generic GTE `lwc2` sequence).
- 32-byte CLUT-like windows (>= 10 distinct colours): `TOMBA2.IMG` 0, `TOMBA2.DAT` 0. Hits in A00.BIN are 32–180-byte
  compiler idioms (e.g. the GCC divide-by-zero trap sequence), not graphics.
- ASCII strings shared with T1: all are Psy-Q messages (CdRead, GPU, libsnd, TMD); none from T1 game text.
- `CD/LOGO.STR` vs T1 `MOVIE/LOGO.STR`: 14 identical 2324-byte sectors and 548 shared 64-byte blocks, all within the
  first ~115 sectors at a stride of 7 (the sector pattern of interleaved audio). Video sectors differ; file sizes
  differ (2665600 vs 2382380 B). Content not decoded.

**No T1 game assets or T1 text were found in the demo.**

## Not verified / interpretation
- The LOGO.STR overlap is probably the same company-logo jingle audio re-used; not decoded or listened to.
- `tickTaskTimers` surviving unchanged suggests at least some T1 low-level source was kept (same task-table layout).
  One function is not enough to call it the same engine.
- Low fuzzy similarity can come from rewritten code, a different compiler build or different struct layouts; these
  tests cannot tell those apart.
- `MAIN.EXE` holds item / event names of the full game (all Evil Pigs, "Dummy1..17" placeholders); not checked
  against the retail T2.
