# Tombi! save format (memory card)

Derived from the matched D0SPSUB2 overlay (the save/load screen): `func_800E999C` (src/wip/d0spsub2_func_800E999C.c)
builds the save, `memCardFileListStep` (src/d0spsub2/memCardFileListStep.c) writes it with
`memCardWriteFile(path, SAVE_DATA_BUFFER, 0xC00, 1)` and loads it back, `memCardStoreChecksum` /
`memCardVerifyChecksum` are the checksum, `memCardReadDirectory` reads the list. Field names: `include/gamestate.h`
(`GameState GAME` at RAM 0x8009C960). Checked against real saves written by the native port (port2) and by reading
edited saves back in it (see docs/SAVE_EDITOR.md). Library: `tools/tombi_save.py`.

## On the card

Standard PS1 card: 128 KB, 16 blocks of 8 KB, block 0 = header + directory (frame `i` = block `i`: byte 0 = 0x51 first
block in use, u32 @4 = file size, u16 @8 = next block, 0x0A = file name (20 chars), byte 0x7F = XOR of bytes 0..0x7E).

A save takes **1 block**; the game writes 0xC00 bytes (the rest of the block is left as is). File name
`<region><code>TOMBA-<nn>`, `nn` = slot 00..14 (the number shown in the list, the icon number):

| Version | File name prefix |
|---|---|
| PAL SCES-01331 (Tombi!) | `BESCES-01331TOMBA-` (verified) |
| USA SCUS-94236 (Tomba!) | `BASCUS-94236` (verified: see "USA saves" below; 1/60 s play time, other title offsets) |
| JP SLPS-01144 | `BISLPS-01144` (recognised; not verified) |
| JP development code | `BISCPS-99999`: listed and loaded by the JP retail, never written (see below) |

## The 0xC00 bytes

| Offset | Size | Contents |
|---|---|---|
| 0x000 | 4 | `"SC"`, 0x11 (icon: 1 frame), 0x01 (1 block) |
| 0x004 | 64 | title, Shift-JIS, from a per-language template (`D_800F6A34[lang]`), digits written by the code: |
|  0x016 | 2x2 | slot number (fullwidth digits `０`..`９` = 0x824F + n) |
|  0x01E | 5x2 | play time `hhh:mm` (0x1E, 0x20, 0x22, `：` at 0x24, 0x26, 0x28); h = time / 180000, m = time / 3000 mod 60 |
|  0x030 | 2x2 | lives (`Ｐ＝nn`, GAME +0x20) |
|  0x040 | 2x2 | "Level" (GAME +0x13) |
| 0x044 | 0x1C | zero |
| 0x060 | 32 | icon CLUT (`D_800F6A2C[lang]`) |
| 0x080 | 0x180 | icon bitmap 16x16 4bpp (`D_800F6A24[lang]`) |
| 0x200 | 0xA00 | **GAME block**: 0x988 bytes copied from RAM 0x8009C960, zero to 0xBFE, checksum at 0xBFF |

**Checksum** (`memCardStoreChecksum(p, 0xA00)`): byte at save+0xBFF = XOR of the 0x9FF bytes save+0x200..+0xBFE.
The loader rejects a save whose XOR does not match. The title / icon header is not covered.

Before copying, the game sets `GAME.selectedSpawnPoint` (+0x22) = 0, a byte at +0x12 = 1, copies the player position
to +0x954 (3 ints) and the button configuration / options into +0x986, +0x984, +0x985, +0x32. On load, it copies the
0x988 bytes back to GAME, then sets `playerHealth` (+0x10) = `playerMaxHealth` (+0x11) and the button config (+0x5BC)
from the current one.

The save list (`memCardReadDirectory`) reads only GAME +0x00..+0x7F: location = `LOCATIONS[area +0x00][section
+0x02]`, AP +0x0C, time +0x08, level +0x13, lives +0x20.

## GAME fields (offsets from save+0x200, RAM 0x8009C960 + offset)

| Offset | Type | Field | Notes / limits used by the editor |
|---|---|---|---|
| 0x000 | u16 | selectedArea | area the Continue loads (0..19) |
| 0x002 | u16 | selectedSection | section of that area |
| 0x008 | u32 | totalTimePlayed | vblanks (50 per second PAL, 180000 = 1 h; 60 per second USA / JP); title shows up to 999:59 |
| 0x00C | u32 | playerAP | AP; editor limit 9 999 999 |
| 0x010 | u8 | playerHealth | overwritten with max health on load |
| 0x011 | u8 | playerMaxHealth | vitality; `increaseMaxHealth`: up to 8, up to 16 when +0x71F != 0 |
| 0x013 | u8 | (level) | "Level" of the title |
| 0x020 | u8 | playerLives | "P=" of the title |
| 0x022 | u16 | selectedSpawnPoint | 0 in every save |
| 0x034 | 20x32 | areaNN_pickedItems | picked-up item / chest flags per area |
| 0x434 | u16x2 | nextArea, nextSection | |
| 0x438 | u8x3 | red/green/blue exp level | |
| 0x43B | u8x3 | red/green/blue exp bar | |
| 0x444 | u8[256] | **event flags** (`SCRIPT_FLAGS` 0x8009CDA4) | 0 = not started, 1..0xFE started (some count steps), 0xFF = completed (`setEventStarted` / `setEventComplete` / `getEventState`); the 130 events of the pause menu are the ids listed (+1) by SUBSCREEN_EVENT_ORDER (D0SPSUB 0x801086F0), up to 0xBF, not 0..0x81 |
| 0x544.. | | `SCRIPT_GLOBALS` / GCD area variables | per-event progress counters (named in notes/names.md) |
| 0x71F | u8 | goldenBowlState | non-zero: max health can reach 16 |
| 0x720 | u8 | bonusHealth | hearts collected at max health 8 |
| 0x744 | u8[256] | item[id] | quantity per item id |
| 0x844 | u8[256] | inventory.slots | item ids in list order (`addItemToInventory` inserts at the front) |
| 0x944 | u16 | inventory.counter | number of slots in use |
| 0x946 | u16 | inventory.sortMode | bit 15 set when an item is added |
| 0x948 | u16x3 | currentArea / Section / SpawnPoint | |
| 0x952 | u8 | playerEquips.weapon | |
| 0x953 | u8 | playerEquips.pants | |
| 0x954 | int x3 | player position at save time | |
| 0x963 | u8 | purifiedAreas | |

Item names: the user's disc, the sub-screen list's own table `ITEM_NAME_STRINGS[ITEM_NAME_INDEX[id]]` of
`SYSTEM/D<n>SPSUB.BIN` (n = language 0..8; PAL D0SPSUB 0x801096D4 / 0x8010979C), ids 0..159. Not `AREA00/MFNT000<lang>.WFM`
entry id+3: that bank is the "got item" messages and follows the ids only up to 9 (it has no Bucket, Telescope, Tear
Jar...; id 12 would read "Wood Boomerang" instead of "Tear Jar"). Event titles: `CFNT999<lang>.WFM` through the
200-byte table at MAIN0 0x80080410 (event id -> title index, 0xFF = untitled). Unused items / events: docs/CUT_CONTENT.md.

## USA saves (SCUS-94236, verified 2026-11-09)

Checked field by field against the USA C (DSPSUB2: `func_800E954C_scus94236.c` builds the save,
`func_800E7ECC_scus94236.c` = memCardFileListStep writes / loads it, `func_800E7714_scus94236.c` =
memCardReadDirectory) and the USA symbols (`config/scus94236/symbol_addrs_*.txt`, whose `PAL 0x...` notes give the
same offset from GAME for every GAME field: GAME_PLAYER_AP +0x0C, GAME_PLAYER_MAX_HEALTH +0x11, SCRIPT_FLAGS +0x444,
SCRIPT_GLOBALS +0x544, LOADED_AREA +0x948, the equips +0x952 / +0x953...). The USA link also reuses the PAL C of
addItemToInventory, increaseMaxHealth, setEventComplete... (`config/scus94236/reuse.txt`) with every sha1 OK, so the
`GameState` offsets are the same. Then written and loaded in the native port's USA build (below).

The same as PAL: the file name `BASCUS-94236TOMBA-nn`, 1 block, 0xC00 bytes written, the GAME block at +0x200 (0x988
bytes from `GAME`, USA RAM 0x8009BCC8), zero to +0xBFE, the XOR checksum at +0xBFF over +0x200..+0xBFE
(`memCardStoreChecksum(D_800E1388, 0xA00)`), zero +0x44..+0x5F, CLUT +0x60, bitmap +0x80 (0x180 bytes), the fields
set before the copy (+0x22 = 0, +0x12 = 1, player position to +0x954, +0x986 / +0x984 / +0x985 / +0x32) and after a
load (+0x10 = +0x11, +0x5BC = the current button config), the list's fields (area +0x00, section +0x02, time +0x08,
AP +0x0C, level +0x13, lives +0x20, read from GAME +0x00..+0x7F).

What differs:

| | PAL | USA |
|---|---|---|
| play time unit (GAME +0x08) | 1/50 s (h = t / 180000, m = t / 3000) | **1/60 s** (h = t / 216000, m = t / 3600; also the list's `func_800EB87C`) |
| header byte 2 | 0x11 (static icon) | **0x13** (the 3-frame animated icon) |
| title template | per language (`D_800F6A34[lang]`) | one, `D_800E74F4`: `ＴＯＭＢＡ！　［００］　０００：００　Ｐ＝００　Ｌｅｖｅｌ０` |
| title digit offsets | slot +0x16, time +0x1E / +0x20 / +0x22, `：`, +0x26 / +0x28, lives +0x30, level +0x40 | slot **+0x14**, time **+0x1C / +0x1E / +0x20**, `：`, **+0x24 / +0x26**, lives **+0x2E**, level **+0x3E** (its second digit at +0x40 replaces the template's last space) |
| icon / CLUT source | `D_800F6A24[lang]` / `D_800F6A2C[lang]` | `D_800F544C` / `D_800F55CC` (one each) |

JP (SLPS-01144): the play-time draw (`func_800ED850_slps01144`) also divides by 216000 / 3600 (1/60 s); its save
builder is not decompiled, so its title offsets are not known (the tools leave a JP title as it is).

The tools: `tools/tombi_save.py` `LAYOUTS` / `PRODUCT_LAYOUT` (time base and title digits per product code;
`Save.hz`, `Save.hms()`, the 999:59 limit at the save's own rate), the save editor's play time, `tools/save_import.py`
`PRODUCT_HZ` / `PRODUCT_SLOT_OFS` (the info line, "Save to another slot") and `SaveImport.java` (the same).

**Checked in the port** (USA build, headless): a save from the save screen (`--menu 2000:4 --pad-script
1000:start,1300:cross:3x45/30,2200:cross:3,2300:left,2400:cross:3`) creates `BASCUS-94236TOMBA-00` in `--save-dir`:
header `SC 13 01`, title `ＴＯＭＢＡ！　［００］　０００：００　Ｐ＝０３　Ｌｅｖｅｌ０１`, time 871 (vblanks at 60 per second),
checksum right, zero after GAME +0x988. Edited with `tombi_save.py set CARD 0 ap=12345 lives=7 level=4 time=299100`
(1:23:05 at 60 Hz): the USA LOAD screen lists it as `01 Lv.4 :7 1:23 AP 12345 Village of All Beginnings`, and loading
it (`--pad-script 1000:right,1050:cross,1450:cross:3,1600:left,1700:cross:3`) puts AP 12345, lives 7, level 4 and the
time (counting on at 60 per second) back in GAME. Not checked: a save made on a real USA console.

## Container formats read by tools/tombi_save.py

Raw 128 KB (`.mcd .mcr .mc .srm .ps`, the port's cards), DexDrive `.gme` (0xF40-byte header), `.vgs/.mem` (64-byte
header), single-save `.mcs/.mcb/.ps1` (directory frame + blocks). The header / trailer bytes are kept, so an unedited
card is written back byte-identical.

## The JP retail also loads `BISCPS-99999` saves (2026-11-09)

JP DSPSUB2 `memCardReadDirectory` (0x800EA490, `src/d0spsub2/func_800EA490_slps01144.c`) lists a card file when its
name starts with either of two prefixes: `BISLPS-01144` (D_800E9B90) or `BISCPS-99999` (D_800E9BA0). The strings sit
side by side at `SYSTEM/DSPSUB2.BIN` +0x98 / +0xA8; the save path at +0x720 is `BISLPS-01144` + `TOMBA-%02d`, the only
name the game writes. USA (`BASCUS-94236`) and PAL (`BESCES-01331`) accept only their own code.

SCPS-99999 is a placeholder product code (the 99999 number of an unassigned Sony title, "BI" = Japan region): saves
made by an internal or pre-release Japanese build that still used it stay loadable in the retail game.

Implications, checked:
- **In the game**: such a save shows in the list and loads like any other (same 0xC00-byte layout and checksum: the
  loader reads it with the same code); saving afterwards writes a new `BISLPS-01144TOMBA-nn` file, the old file is left
  as is. The JP retail never creates a `BISCPS-99999` file, so only a card from a development build would have one.
- **Which builds used it** (every Tombi disc in the CI secrets checked, 2026-11-09):
  - **Hyper PlayStation Re-mix No.12 (SLPM-80167), Tomba demo**: its `TOMBA/SYSTEM/DSPSUB2.BIN` holds only `bu10:`,
    `bu00:`, `TOMBA-%02d` and `BISCPS-99999` (+0x04..+0x20): the demo **writes and reads** `BISCPS-99999TOMBA-nn`.
    So saves made with that demo are listed and loaded by the JP retail. Not checked: whether the demo's GAME
    layout is the retail one (a demo save could load with values the retail game reads differently), and whether
    the demo lets the player reach the save screen.
  - **Beta 1998-02-03**: `SYSTEM/DSPSUB2.BIN` is already the retail scheme (reads `BISLPS-01144` and
    `BISCPS-99999` at +0x98/+0xA8, writes `BISLPS-01144` at +0xF0).
  - **PrePre Vol. 9 (PCPX-96093) and Hyper 12 `TOMBA.EXE`**: `BISCPS-99999` sits next to `bu00:`/`bu10:` in the boot
    executable, with the debug menu's `MEMORY CARD : ACCESS TEST` (the test menu's card code; not traced further).
  - **JP demo SLPM-80165**: no DSPSUB2 and no product code anywhere: it cannot save.
  - So the second prefix is the development code the pre-release builds saved with, kept in the retail reader so
    those saves (in practice, Hyper 12 demo saves) still load.
- **Port**: `port2/shims/card.c` (`tombi_name`) and `tools/save_import.py` already treat any name containing `TOMBA` as
  a Tombi save, so such files are kept and merged. The save editor (`tools/tombi_save.py`) listed only the three retail
  codes; `BISCPS-99999` is now in its `PRODUCTS` table so those saves are listed too.
