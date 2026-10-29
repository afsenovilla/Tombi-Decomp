# PrePre Vol. 8 (PCPX-96084): the oldest known Tomba build

Sony's Japanese demo compilation "PrePre Vol. 8" holds a Tomba demo in `/TOMBA` (the other directories are other
games; `MAIN.EXE` is the compilation menu). The disc and anything extracted from it stay local, like `game/`. This
page records findings only. Tool: `tools/prepre8.py` (extract `/TOMBA`, unpack GAMs, dump the exe load lists, render
pages, the player sheet and the rooms).

## Dates

| Item | Date |
|---|---|
| ISO volume (PCPX_96084) | 1997-08-05 |
| `TOMBA/TOMBA.EXE` | 1997-06-24 15:35 |
| GAM data, `PLAYER.WPP` | 1997-06-19/20 |
| `OPING.STR` / `MIST.STR` | 1997-06-17 / 06-18 |
| sound banks | 1997-06-06..11 |

The exe says `@1997 WHOOPEE CAMP Co.,Ltd.`. Everything in it is older than SLPM-80165 (docs/JPDEMO.md): older
player sprites, an older village model, no WFM text and a single executable.

## Layout

| File | Contents |
|---|---|
| `TOMBA.EXE` | PS-X EXE at 0x80010000, entry 0x80087390, 0x128000 bytes (zero bss stored in the file). Rodata 0x80010000, text 0x80016D40..0x800A10F4, data after it. No overlays: all game code is in the exe. |
| `SYSTEM/SYS000..010` | `SYS003` = WhoopeeCamp logo (8bpp), `SYS005` = title screen (8bpp landscape + logo), `SYS004/006/007` = 256-colour palettes, `SYS001/002` small palettes, `SYS008..010` 384x256 pages (blank or nearly) |
| `SYSTEM/JOCHU.VH/VB` | resident sound bank (35 samples) |
| `AREA00/TIM000..010.GAM` | raw VRAM pages (width = len/512 words, 256 rows) |
| `AREA00/CLUT00.GAM` | 640x32 words of palettes at (384,480) |
| `AREA00/DATA000.GAM` | sprite/animation tables (load kinds 0x30..0x37, 0x39, 0x40..0x45, 0x51..0x53, 0xB0) |
| `AREA00/DATA001/003/004.GAM` | room: WMD model (kind 0x50, the old SLPM-80165 packet layout read by `tools/jpdemo_rooms.py`) + WHM collision (0x70) + small tables (0x80..0x86) |
| `AREA00/DATA002.GAM` | WHM + tables only: a 2D room drawn over the pre-rendered `TIM007` |
| `AREA00/PLAYER.WPP` | Tomba's frames, the PAL WPP format (`tools/wpp.py`), 313 frames |
| `AREA00/ME, AREA1, S_AREA01 .VH/.VB` | sound banks |
| `MOVIE/OPING.STR`, `MIST.STR` | MDEC + XA, 320x224 |

The exe has a named file table (0x40-byte entries, `\TOMBA\...;1` paths at file offset 0x9FA3A, filled at boot by
`CdSearchFile`) and load lists at file offset 0xA05FC (20-byte records: file id, kind<<8|flags, destination,
x, y, w, h). These are the ancestor of PAL's `LD0*` lists. Room lists: village (DATA001 with TIM000-004),
village variant (TIM005/006), interior (TIM007-009 + DATA002), street (DATA003), house (TIM010 + DATA004).
`python3 -I tools/prepre8.py loadlist TOMBA.EXE` prints them.

## Code

`tools/xmatch.py` finds 508 PAL functions in TOMBA.EXE (101 KB, 494 of them from MAIN0, 86 byte-identical). SLPM-80165
has 751. The code is single-exe: X000-type code (objects, areas) is built into the exe, mostly in versions that
differ too much for masked matching.

## Cut and changed content

Ranked by interest. "New" means not found in PAL or SLPM-80165, by an 8-row block hash over every GAM page of both
discs, by frame/sample comparison, or by eye.

1. **Pre-rendered interior** (`TIM007`, 8bpp, CLUT00 row 20): the elder's house drawn as a 2D backdrop (cone roof,
   bookshelves, candles, cauldron over a fire, scales, stump). DATA002 is its collision. Later builds do not
   have this backdrop.
2. **Different Tomba**: all 313 player frames differ from PAL (358) and SLPM-80165 (379). It is an earlier render with
   different shading and hair, a pig-carrying pose, and no swimming set.
3. **Early title screen** (`SYS005`): a landscape with a rock spire and a river, plus a "オレっ！トンパ" logo with a
   different layout from retail. There is also a WhoopeeCamp logo page (`SYS003`).
4. **Grey bird/dragon enemy** (TIM000, about 16 poses: flying, swooping, perched). It is not in any later page.
5. **Event-title strip** (TIM000): pre-rendered 16x16 glyphs for the event names, which retail renders from WFM
   text. Titles that can be rebuilt from it: ポンプ岩の謎 (retail ポンプ岩のナゾ), ジィちゃんの形見, １００年老人と出会い,
   霧を吹き飛ばせ！, モトクロスコース, キミ、だあれ？, 卵の中身 (retail コッカタマゴの中身), ドワーフの長老. Two exist only here:
   **少年旅立ち** ("The boy sets out") and **あれま大きな水たまり(?)** ("Oh my, what a big puddle").
6. **Village model** (DATA001): 1839 of its 3192 polygons are not in SLPM-80165's D003. It has separate round trees
   instead of the forest wall, a bare slope, a different house by the river and the same small red castle. DATA003
   (thatched houses, a watch tower, a river) and DATA004 (a house with a pig sign) are other early rooms.
7. **Menu and HUD art** (TIM001): a 10x10 kana font plus about 125 kanji, `ステータス アイテム EXIT`,
   `イベントマップ PAUSE`, `GET!!`, `Thank You!! Bye-Bye..`, a Tomba-with-umbrella sprite, early clouds and trees.
   The script text that uses this font was not found: there are no WFM or Shift-JIS strings, and searches with
   byte, u16 and cell-index encodings failed. This is still open.
8. **Movies**: `OPING.STR` is a short opening, 626 frames (about 42 s) against 1395 frames for PAL `OP_INST` and
   SLPM-80165 `OP_UTA`. It is a cut of the same animation (landscape, feet in the grass, Tomba, the red bird, Tomba
   shouting, a pig). `MIST.STR` has 311 frames against 266 in PAL. `ffmpeg -f psxstr` plays the `.STR.raw` output
   of `tools/prepre8.py extract`.
9. **Sound**: 15 samples are new (JOCHU 1, 5, 11, 12 (2.4 s), 19-21, 23-25, 27; S_AREA01 0, 25, 28, 29). The others are
   in PAL `SND_INI`/`SND0000`/`SND0044X`... and SLPM-80165 `SND002_`. No SEQ file is on the disc; one `pQES`
   sequence sits inside DATA000.

## Civilization top-down data (checked 2026-10-29)

No trace. The disc has only `AREA00` (no AREA11/16/17/18 directories or names: the exe file table lists only the
39 files above), and no load-list record or GAM sub-chunk uses kind 0x3B. Kinds present: 0x30-0x37, 0x39, 0x40-0x45,
0x50-0x53, 0x60/0x61, 0x70, 0x80-0x86, 0x90-0x92, 0xA0-0xA2, 0xB0. The loader 0x800240BC files each chunk by high
nibble into a slot table indexed by the low nibble: 0x2x -> 0x1F800150, 0x3x -> 0x1F800198 (8 slots up to
0x1F8001B8), 0x4x -> 0x1F8001B8, 0x5x -> 0x1F8001C8, 0x6x -> 0x1F800190, 0x7x -> 0x1F8001D8, 0x8x -> D_800DE4B0,
0x9x -> D_800DE4D0, 0xAx -> D_800DE750, 0xBx -> D_800DE074. A 0x3B chunk would land on the 0x43 slot, so this build
has no room for the PAL 0x3B sprite tables (0x39 itself shares the 0x41 slot, read from the slot arithmetic only).
No TIM/CLUT page holds a top-down or isometric map: the pages are the village, interior and HUD art listed above.

## Rooms, exits and other leads (2026-10-29)

- **Room table**: `D_800A1AA4[area]` -> six u16 per room (area 0 = 1,1,1,2,1,1; area 1 = 1,1,1,1,1,4), copied to
  `D_800E2B44` before the loader `func_80023FCC` runs. Area 0 room 3 is the only room flagged 2: the 2D room (the
  elder's house over `TIM007`, collision `DATA002`). Area 1 room 5 is the only room flagged 4.
- **Load lists**: `D_800B03D8` holds 7 list pointers: 0 = common (CLUT, TIM000-002, banks, PLAYER.WPP, DATA000),
  1 = DATA001 + TIM003/004, 2 = DATA001 + TIM005/006, 3 = TIM007-009 + DATA002 (the 2D interior), 4 = DATA003,
  5 = TIM010 + DATA004, 6 = empty. `func_80023FCC(list, ...)` loads one through `func_800240BC`.
- **Room entry and exit**: `D_800A273C[area][room]` -> 16-byte entry records indexed by `D_800DE0FA`, read by
  `func_8002816C` (x, y, mode, ...). Mode 2 means "came through a door"; area 0 room 3's first record is mode 2.
  The next area/room/entry (`D_800DE20C`/`D_800DE20E`/`D_800DE218`) are set by `func_8002816C` and by
  `func_80034C34` (object type in the table at 0x800A2760), which hard-codes area 0, room 2, entry 1. That is the
  elder's-house exit back to the village (from the code only; not checked in an emulator).
- **Dialogue text**: still not found. The room GAMs carry only models, collision and object tables (kinds 0x70,
  0x80-0x86), the DATA000 0xB0 chunk is the `pQES` sequence, and no other chunk is text. The demo may have no
  dialogue beyond the pre-rendered event titles and menu art.
- **Grey bird**: not identified. The object type table is `D_800A7364` (31 entries, 0x80036FA4 = empty). The only
  handler that reads the 0x39/0x41 sprite slot (0x1F8001BC) is type 15, `func_8003D40C` (uses sine motion and
  `isObjectInView`), so it is the best candidate, but this is unconfirmed.
- **Old event titles**: the code that draws the TIM000 title strip has not been traced.

## Debug features

- 0x8008299C is NOT a mode dispatcher: it is the unlayered-object spawner. It walks the per-room list
  D_800AF144[area=D_800DE0D8][room=D_800DE0DA] (area 0 -> 0x800AF114, area 1 -> 0x800AF12C; 6 rooms each) of 4-byte
  records {type, arg, 0, run_now} ended by 0xFF, allocates an object (allocObjectUnlayered), sets obj[2]=type,
  obj[3]=arg, and calls D_800AF14C[type] at once when run_now != 0. The same table drives updateObjectsUnlayered.
  Types: 0 = 0x80082BA4 (state object, PoolFree), 1 = 0x80083998, 2 = 0x80083CD4 (initSectionSpawnerAlt, PAL match),
  3 = 0x80083D88 (prints `enemy=%01d`; pointer also at 0x800AF158), 4 = 0x800853A8 (2-variant state object, tables
  0x800AF0B4..0x800AF0D0: variant 0 = 84030/841D0/841F0/84768, variant 1 = 84774/84868/84934/84E48).
  Room lists: area0 rooms 0-2 = {0 run, 1, 4 arg1, 4 arg2}; rooms 3,4 = {2 run, 1}; room 5 = {2 run, 1, 4 arg0};
  area1 room 0 = {0 run, 1, 3}; rooms 1-5 = {2 run, 1}. So the `enemy=` object spawns only in area 1 room 0.
- OPTION MODE root is 0x800854B4 (prints `IRQ=%d`, jump table 0x80015B88, strings table 0x800AF1BC..0x800AF1DC:
  `OPTION MODE`, `[O] : EXECUTE`, `[X] : EXIT`, `MUSIC : PLAY TEST`, `SOUND : OUTPUT TEST`, `CDROM : LOAD TEST`,
  `MEMORY CARD : ACCESS TEST`). Children: 0x80085650 menu, 0x80085854 MEMORY CARD TEST, 0x800862C8 CDROM LOAD TEST,
  0x800867E0 SOUND OUTPUT TEST, 0x80086F7C MUSIC PLAY TEST. No pointer, jal or %hi/%lo pair to 0x800854B4 exists in
  TOMBA.EXE: the OPTION MODE root is dead code in this build (port feasibility: needs a new hook).
- Title: state machine 0x8001ECD4 (jump table 0x800100E0), menu strings table 0x800A1A78 = NEW START / LOAD START /
  OPTION; attract modes 0x8001F9D8 (`PLAY DEMO`) and 0x8001FACC (`RANKING DEMO`) are called from 0x8001ECD4.

## Insertion into the PAL port

| Find | Feasibility |
|---|---|
| Interior backdrop | Medium-hard. Needs a 2D-room path (an 8bpp page + CLUT, a WHM) that PAL does not use for this house. Easiest as a gallery or extra screen in the port's launcher. |
| Old Tomba sprites | Medium. Same WPP format, but the frame numbering differs (313 vs 358) and animation tables index by frame, so it needs a mapping table or a costume switch with remapped animations. |
| Title screen | Easy as a port option: two 8bpp pages + palettes, drawn instead of the PAL title background. |
| Bird/dragon | Hard for gameplay (no AI code was matched); easy as a decoration or gallery item. |
| Event titles (2 lines only here) | Easy: add WFM3 event-title strings (the WFM3 encoder exists), translated. |
| Village/rooms | Medium. Convert the old WMD packets to PAL WMD (`docs/JPDEMO.md`); cosmetic only. |
| Short opening, MIST | Easy for the port (play the STR instead), but these are cuts of existing videos. |
| Sound samples | Easy to bank; there is no evidence where they were used. |
| OPTION/test menus | Port-only re-implementation (the PAL exe has no such code). |

## Matching target

PCPX-96084 is a third non-PAL version (`tools/versions.py`: key `pcpx96084`, alias `prepre8`, serial `PCPX_96084`,
program `tomba` = TOMBA.EXE, game dir `game_prepre8/`, git-ignored). It is set up the same way as SLPM-80165:

- `config/pcpx96084/tomba.yaml` is the splat config (header, rodata 0x800, text 0x7540, data 0x918F4..0x128000).
- `config/pcpx96084/symbol_addrs_tomba.txt` is generated by `python3 -I tools/xmatch.py game_prepre8/TOMBA.EXE --csv
  build/pcpx96084/xmatch.csv` + `python3 -I tools/jpdemo_syms.py build/pcpx96084/xmatch.csv --version pcpx96084`
  (PAL names). xmatch pairs with PAL functions only, so SLPM-80165-only functions are not named here.
- `config/pcpx96084/symbol_addrs_manual.txt` is the task-table and scratchpad block copied from SLPM-80165.
- `python3 tools/build_report.py --version pcpx96084 [--objdiff PATH]` writes `build/pcpx96084/report.json`. C is
  reused only automatically, through the symbol-file pairing. First run: 1424 functions, 441 matched (31.0 %),
  94060 of 565892 code bytes (16.6 %), 475 base objects. 2026-10-29: 472 matched (33.1 %), 96976 code bytes (17.1 %). The gain comes from two things:
  `config/pcpx96084/spad_remap.txt` (PAL scratchpad address -> PrePre 8 address, applied by `tools/build_report.py`
  to the PAL scratchpad symbols for this version only), and `#if defined(VERSION_PCPX_96084)` lines in shared C for
  literal addresses and small differences (task table at 0x801FC000, `TASK_C` is the global `D_800DE0B0`,
  `isObjectInView` screen origin at 0x1F80028A/0x1F800392, 0x1F8002A6 in place of 0x1F80019E, category 8 drawn from
  the PAL category 7 list, 0xE4-byte layer-1 objects, `setObjectSpeedPolar2` fields at +0x7C, `startAreaBgm(1)`).
  `closeTask` and `startTask` stay at 99.6 %: the only difference is the operand order of the `addu` that the
  assembler emits for a symbol+register load, which C cannot change.
- CI job `report-prepre8` (`.github/workflows/report.yml`) downloads the disc image from the
  **`GAME_FILES_URL_PREPRE8`** secret (raw data track .bin or a zip with TOMBA.EXE; Drive links handled like the
  demo job), extracts only `/TOMBA` with `tools/prepre8.py extract`, checks the TOMBA.EXE sha1
  (032d6f371b31085098b86aebbce465d30599293a) and uploads `PCPX_960.84_report`. It is skipped when the secret is
  missing.

## Palettes (tools/clut_catalog.py, 2026-10-29)

`CLUT00` gives 269 distinct 16-colour rows with `SYS001/002/004/006/007` and the two TIMs in `TOMBA.EXE`; 62 of them are on
no retail disc (59 in `CLUT00`, `SYS006`, the exe TIMs), and 35 are shared only with JP SLPS-01144. None of them fits the
Civilization scenery pages or the Female Stonetown Resident better than the retail rows (docs/CIVILIZATION.md, "Palette
catalog").
