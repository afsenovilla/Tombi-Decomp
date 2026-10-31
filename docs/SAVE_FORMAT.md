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
| USA SCUS-94236 (Tomba!) | `BASCUS-94236` (recognised; layout assumed the same, not verified) |
| JP SLPS-01144 | `BISLPS-01144` (recognised; not verified) |

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
| 0x008 | u32 | totalTimePlayed | vblanks (50 per second; 180000 = 1 h); title shows up to 999:59 |
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
| 0x444 | u8[256] | **event flags** (`SCRIPT_FLAGS` 0x8009CDA4) | 0 = not started, 1..0xFE started (some count steps), 0xFF = completed (`setEventStarted` / `setEventComplete` / `getEventState`); ids 0..0x81 are the game's 130 events |
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

Item names: the user's disc, `AREA00/MFNT000<lang>.WFM` entry id+3. Event titles: `CFNT999<lang>.WFM` through the
200-byte table at MAIN0 0x80080410 (event id -> title index, 0xFF = untitled). Unused items / events: docs/CUT_CONTENT.md.

## Container formats read by tools/tombi_save.py

Raw 128 KB (`.mcd .mcr .mc .srm .ps`, the port's cards), DexDrive `.gme` (0xF40-byte header), `.vgs/.mem` (64-byte
header), single-save `.mcs/.mcb/.ps1` (directory frame + blocks). The header / trailer bytes are kept, so an unedited
card is written back byte-identical.
