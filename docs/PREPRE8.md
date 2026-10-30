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
   Re-read 2026-10-30: the strip is a **glyph sheet** (8 rows of 16x16 glyphs, not whole titles), so only glyphs can be matched; see "Event list" below.
6. **Village model** (DATA001): 1839 of its 3192 polygons are not in SLPM-80165's D003. It has separate round trees
   instead of the forest wall, a bare slope, a different house by the river and the same small red castle. DATA003
   (thatched houses, a watch tower, a river) and DATA004 (a house with a pig sign) are other early rooms.
7. **Menu and HUD art** (TIM001): a 10x10 kana font plus about 125 kanji, `ステータス アイテム EXIT`,
   `イベントマップ PAUSE`, `GET!!`, `Thank You!! Bye-Bye..`, a Tomba-with-umbrella sprite, early clouds and trees.
   **Solved 2026-10-30**: the script is in TOMBA.EXE as u16 glyph indices (no WFM, no Shift-JIS); decoder
   `tools/prepre8_text.py`. See "Text (event titles and dialogue)" below.
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
- **Load lists**: `D_800B03D8` holds 7 list pointers (corrected 2026-10-30): 0 = common (CLUT, TIM000-002, banks, PLAYER.WPP,
  DATA000), 1 = DATA001, 2 = TIM003/004 + DATA001, 3 = TIM005/006 + DATA001, 4 = TIM007-009 + DATA002 (the 2D interior),
  5 = DATA003, 6 = TIM010 + DATA004 (no empty list). `func_80023FCC(area, room, common)` loads `D_800B03D8[area]` when
  `common` is set, then `D_800B03F4[area][room]`; `D_800B03F4` has a single valid entry (area 0 -> lists 1..6 for rooms
  0..5; the word after it, 0x1F801070, is not a pointer), so **area 1 has no load list** in this build.
- **Room entry and exit**: `D_800A273C[area][room]` -> 16-byte entry records indexed by `D_800DE0FA`, read by
  `func_8002816C` (x, y, mode, ...). Mode 2 means "came through a door"; area 0 room 3's first record is mode 2.
  The next area/room/entry (`D_800DE20C`/`D_800DE20E`/`D_800DE218`) are set by `func_8002816C` and by
  `func_80034C34` (object type in the table at 0x800A2760), which hard-codes area 0, room 2, entry 1. That is the
  elder's-house exit back to the village (from the code only; not checked in an emulator).
- **Dialogue text**: found in TOMBA.EXE (not in the GAMs), see "Text (event titles and dialogue)".
- **Grey bird**: not identified. The object type table is `D_800A7364` (31 entries, 0x80036FA4 = empty). The only
  handler that reads the 0x39/0x41 sprite slot (0x1F8001BC) is type 15, `func_8003D40C` (uses sine motion and
  `isObjectInView`), so it is the best candidate, but this is unconfirmed.
- **Old event titles**: the TIM000 strip is a runtime glyph cache, filled from the event-title font (see below); the
  draw code itself is still not traced.

### Text (event titles and dialogue), found 2026-10-30
Decoder: `python3 -I tools/prepre8_text.py TOMBA.EXE [--raw]` (prints to stdout; do not commit the output).

- **Encoding**: strings are arrays of u16 glyph indices ending in 0xFFFE. Each text block is preceded by a glyph table
  of 5-byte records (u, v, w, h, flag), and the code is the record index: 10x10 kana/symbol cells (25 per row), then
  12x12 kanji (21 per row) sorted by on-reading (gojuon), then two 18x10 cells "100" and "1000". Dakuten/handakuten
  are separate glyphs after the base kana. Dialogue adds controls: 0xFFFB + 3 halfwords (window/speaker header),
  0x4019 (wait), 0xFFFD/0xFFFC (end of page/box), 0xFFF8. This is why byte/u16/Shift-JIS searches failed: no fixed
  base, kanji are a per-table subset, and text is u16.
- **Event titles**: table 0x800A99FC (file 0x9A1FC, 372 glyphs: 174 kana/symbols, 196 kanji), 167 titles at
  0x800AA140..0x800AB12A (file 0x9A940). Kana: symbols are ASCII order from code 0 (space) to `}`, then `…「」、。・、〜ー`
  and `▼←→↑↓○×△□‼AP`; hiragana 0x3E, katakana 0x75, dakuten 0xAC/0xAD, kanji 0xAE..0x171. The font page is not in
  any AREA00/SYSTEM GAM: it only exists in `SYSTEM/SYS008.000` (a 384x256 VRAM snapshot, font at x=768 in 4 bpp).
  The TIM000 16x16 strip is a cache of glyphs for the titles on screen, which is why it held only fragments.
  About 15 kanji are still unread (12x12 glyphs; shown as 〓 by the decoder).
- **Dialogue**: tables 0x800A2BCC, 0x800A4060, 0x800A5E74, 0x800AB130 (file 0x933CC, 0x94860, 0x96674, 0x9B930,
  0x9CDC4). Three of them (0x933CC, 0x96674, 0x9B930) use the TIM001 font (kanji from code 0x95, 94 kanji, decoded) and
  carry the same 104-line area-0 script; the other two (305-glyph tables, file 0x94860 and 0x9CDC4) carry the same
  ~135-line forest/dwarf script, whose kanji page is not in the file tree (kana decode fully; kanji show as `{xx}`).
  Dialogue kana: hiragana 0x31, katakana 0x68, dakuten 0x2F/0x30. Codes 0x2B-0x2E (shown ャ/ョ) are unsure.

Event data behind the titles (checked 2026-10-30): only the **first 25 titles** (少年の旅立ち .. バロンは友だち) are
referenced. Pointer table 0x800AE594 (25 pointers) + 0x800AE5F8 = `・・・・・・・・・・・` (placeholder). The event-list
screen (0x80067EA0) builds index = page*16 + row, forces it to 0 if >= 0x18, and draws the title when the status byte
0x800DE21C[i] is non-zero, else the placeholder: the demo's list has 24 usable slots (indices 0..23). Status functions:
0x800216F4 (start), 0x800217A8 (sets 4), 0x800217F0 (complete; AP from table 0x800A2198), 0x80021914 (get). The AP
table is a placeholder: 20 entries, all (500, 2000). Per-event 10-byte records at 0x800A1B0C are uniform (0, 180, 0, 0,
n). Constant ids passed to start/set4 in the exe: 2, 3, 4, 6, 9, 10, 11, 13; others come from object/script data.
Titles 25..163 (島のヒミツその1-5, 大停電, 人魚の王を救え, 「大団円1-3」, 光の落ちるところ, コロガルゾォ〜, ...) have
**no pointer and no reference anywhere** in the exe: no area, map position, icon, AP or flag code exists for them in this
build. 5つの箱の忘れもの (index 20) and 地下ロウの大ドロボウ (22) do have a list slot, but no area/icon data either.

Event-title list (167 entries; the order is the in-game list order). Highlights not in retail: 少年の旅立ち (Young Boy
Sets Off), 大きな水たまり? (A Large Puddle?; the strip's あれま is not in the title), APゲート, ドワーフの助け, 5つの箱の
忘れもの / 5つの箱のカギ, 地下ロウの大ドロボウ, コロガルゾォ〜 / またコロガルゾォ〜 (Tumbling Down / Again), 光の落ちる
ところ, 嵐の山の怪, そこ、どいて, 怪物?勝てるかな?, 死神の実をつぶせ, 花をさかせよう!, 霧を吹き飛ばせ, 100年老人との出会い
/ の話 / の仲間, コトバをおぼえよう!Ⅰ/Ⅱ/Ⅲ, まいごのドワーフ, バロンは友だち, 消えた長老, のぞいてごらん?!, かくれ里の
ヤン, カエル…にかえる, いよっ!穴ほり名人!, 大トリの山 - Folkin's list matches the first ~45 entries. The rest is
the full planned game: 島のヒミツその1-5, 大停電, 電気うなぎを捕まえろ!, 事件発生、犯人を探せ!, 黒い水の不思議, 文明の
キカイ, 進め!きかんしゃ, 赤/緑/青/黒の占い師, 金/銀/銅色のメダルがほしい, からくり〓の3姉妹, 1000年老人, 万年老人の
数字玉, 億年老人の願い, 人魚の王を救え, 歌を忘れた人魚, 海底通路の行き先, 本当の魔ブタ, 夢?幻?黄金郷を探せ, ミリオンの塔,
「大団円1-3」, plus item names (ヒヨコ, カエル, ドワーフの迷子, バナナ, たつまき, 花の〓).

Dialogue (summary): tutorial signs (jump into the background with ↑+×, swing on branches, charge attack, AP, save
point), place names (始まりの村, 始まりの森, 100年老人の小屋, ふるいけ), a hungry monkey who takes a banana and teaches a
dash, **Yan of the Hidden Village** (隠れ里のヤン) who is found repeatedly and gives hints, a kid asking 「キミ、泳げる
のかい?」 (swimming needed for the big puddle), the 100-year-old man asking Tomba to collect the Kokka eggs and then
sending him to the dwarf elder. The forest script: lost dwarf child, wilted flowers, dwarves who talk about the AP
"point gate", the dwarf elder, two quarrelling dwarves, a broken pot, a trapped thief (「…コマブタ…のすきを見て逃げ
出そうとしたら…」), a dwarf nurse who treats an injured dog and names it **Baron** (バロン).

### Event list (Folkin, Pure 8 = PrePre 8)
Community researcher Folkin reports that the "Pure 8" demo is this same build and published its in-game event list (from video). Titles not in retail: A Young Boy Sets Off (少年旅立ち), A Large Puddle?, AP Gate, Help from the Dwarves, The Forgotten Things Inside the Five Chests, The Key for the Five Chests, Great Thief in the Underground Prison, Tumbling Down!, Tumbling Down Again!, Where the Light Falls, Mystery of the Stormy Mountain, Get Out of the Way, Can You Beat the Monsters?, Crush the Reaper Fruit, Make the Flower Bloom!, The Unmoving Charity Priests, Blow Away the Fog, Meeting the 100 Year Old Man, Tale of the 100 Year Old Man, A Friend of the 100 Year Old Man's, Learn the Language! I, A Lost Dwarf Child, Baron is Your Friend, The Elder Disappeared, Take a Peek?!, Yan of the Hidden Village, Return the Frog to..., The Hole-Digging Master!, Master of the Skies.

TIM000 strip glyphs (rows, left to right): ボンプ岩の謎 ？り里ヤオイラ身 / ちゃん形見 10年泳げッ卵中フ / 老人と出会い霧を話仲間ドワ長 / 吹き飛ばせカエル！ / ...にかえるモトクCLEAR / ロスコーキミ、だ / あれま大きな水た / 少年旅立ちッィく.
- **Confirmed in strip**: 少年旅立ち (Young Boy Sets Off), あれま大きな水たまり (Large Puddle?), 霧を吹き飛ばせ (Blow Away the Fog), 老人と出会い (Meeting the 100 Year Old Man), ドワ(ーフの)長(老) (The Elder / dwarves), モトクロス, キミ、だあれ, カエル (frog titles).
- **Glyph-only hints (not proof)**: 話 (Tale of the 100 Year Old Man), 仲間 (A Friend of...), 里 + ヤ (Yan of the Hidden Village), 泳 (swim, unmatched).
- **Now decoded from the exe** (see Text above): all of Folkin's titles are in the title table.
- **Not in strip**: no 宝箱 (chests), 盗/泥棒 (thief), 転 (tumbling), 光/落 (light falls). Those titles come from Folkin's list only (the strip is not the full list).
- Proposed retail slots (unverified): Five Chests -> Lost and Found (area 0, 魔女の忘れもの), Great Thief -> before The Great Escape, Tumbling Down / Again -> Where did I come from? / Lava Caves, Where the Light Falls -> event 32 (trumpets) slot.

### Event ids from data, and the rooms DATA003/DATA004 (checked 2026-10-30, static only, no emulator run)

- **Dynamic event ids: nothing beyond the constants.** The two data-driven call sites carry no other id:
  - 0x80043C88 reads 8-byte records `D_800A7F7C[area][room][entry]` {u16, s16 event, u16, u16}: of the 30 records
    (area 0 and 1, 6 rooms each) only area 0 room 0 entry 0 has an event (**2**, 100年老人との出会い); all others are -1.
  - 0x800359C0 is not an object script but the **dialogue-text interpreter** (0x8003588C): codes 0xFFF7..0xFFFF go
    through the jump table 0x80010A08, where **0xFFFA id,arg = start event** (0x800216F4) and **0xFFF9 id = set 4**
    (0x800217A8). The message pointers (`D_800A5DE4[bank][i]`, all inside 0x800A2BCC..0x800A5DC0) never contain 0xFFFA
    or 0xFFF9 (a scan of file 0x92000..0x9F000 finds neither inside the dialogue): no dialogue starts an event.
  - Complete list of event ids touched in TOMBA.EXE: start 2, 3, 4, 6, 9, 10, 13 (+2 from the table above); set-4 2, 3,
    4, 6, 10, 11; event 1 (ジィちゃんの形見) is set to 1 directly at 0x800204C0; reads of the status bytes only for 2,
    6, 10. Events **20 (5つの箱の忘れもの) and 22 (地下ロウの大ドロボウ) are never started, set or read**; they are
    list titles only. The interior door object (class 4 type 21, 0x80034C34, in DATA002) handles 2, 10, 11 and 13.
  - GAM object lists hold no event ids: records are 16 bytes {chunk, class, type, 0, u16 sub, x, y, z, ...}; class 4
    uses the type table 0x800A2760, class 5 the table 0x800A7364.
- **The rooms**: room 4 = DATA003 (thatched houses, watch tower, river), room 5 = TIM010 + DATA004 (house with pig sign).
  Both have load lists, room flags 1, and their own entry records (`D_800A273C`, 16 bytes {x, y, mode, next area,
  next room, next entry, 0, 0}; `func_8002816C` copies the "next" triple to 0x800DE20C/E/218 and the room-change task
  0x80020EA4 copies those into the current area/room/entry and calls the loader). Exit graph from those records:
  village room 0 -> room 4 (DATA003) entry 0, village room 1 -> room 5 (DATA004) entry 0, DATA003 -> room 0 entry 1,
  DATA004 -> room 1 entry 1, village room 2 -> room 3 (interior), interior -> room 2 entry 2 or **area 1 room 0**
  (unloadable, see Load lists).
  The player controller (0x80028B50, state from `D_800A2758[room]` = 4, 5, 6, 4, 9, 10) has the triggers: in room 0
  (state 4) x >= 0x4A2 with y >= -0x3C starts the exit; in room 1 (state 5) 0x4EC <= x < 0xA46 with y >= -0x122 does;
  in DATA003 (state 9) x < 0x51 and in DATA004 (state 10) x < 0x29 lead back. So by the code the two rooms are
  **reachable exits of the village**, not dead data; this was not confirmed in an emulator (tools/psxemu HLE is keyed
  to PAL MAIN0 addresses and needs a disc image).
- **Objects**: DATA003's list (chunk 0x84) has only 8 class-7 records (also present in the village); no NPC or sign.
  DATA004's list (chunk 0x85) has 16 class-4 type-25 objects (0x80035530, common in the village: 66 there) and one
  class-5 type-26 sign (0x800404A8, the same place-name sign type as the village's), sub 9 -> message table
  0x800A7DDC[9] = bank 4 line 8 = **「ふるいけ」**. Village signs: sub 0 = 始まりの村 (x 83), 4 = 始まりの森 (x 1610),
  8 = 100年老人の小屋 (x 2327, high up); DATA004's (x 2638) is the only ふるいけ sign. ふるいけ is a retail place (JP
  retail text: 「この小屋のウラをまっすぐ奥に行くと ふるいけ」, ふるいけのカギ, DSPSUB2 place name), so DATA004 is an
  early form of the ふるいけ area, not a lost location. (Mapping record `sub` -> sign index is inferred from the
  village signs matching their positions.) DATA003 has no name; nothing ties it to a cut civilization.
- **No room select**: the current room (0x800DE0DA) is written only by the new-game reset (0x800199D4), the village
  section switch (0x80028D98..0x80028EB4, rooms 0..2) and the room-change task (0x80020F6C); the area word is written
  only by that task. No pad-driven area/room selector exists in the exe.
- Renders: `python3 -I tools/prepre8.py rooms UNPACKED OUT` (already covers DATA003/DATA004).


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

## Functions vs PAL (2026-10-30)

Tool: `tools/fn_vs_pal.py` (method in docs/BETA_980203.md). `game_prepre8/TOMBA.EXE` against PAL, with `--also` JP
`SLPS_011.44`, JP `X00.BIN` and the 1998-02-03 beta `SLPS_011.44`:

```
python3 -I tools/xmatch.py game_prepre8/TOMBA.EXE --csv m.csv
python3 -I tools/fn_vs_pal.py game_prepre8/TOMBA.EXE m.csv --vram 0x80010000 --text 0x80016D40 0x800A10F4 \
   --also game_jp/SLPS_011.44 game_jp/SYSTEM/X00.BIN game_beta980203/SLPS_011.44 --top 1000
```

| Class | Pieces | Bytes |
|---|---|---|
| Masked twin in PAL (xmatch; 494 MAIN0, 14 X000/X001/X003; 86 byte-identical) | 508 | 103664 |
| Gap pieces >= 48 B (all) | 680 | 461636 |
| sim >= 60 (a revised PAL function) | 59 | 17780 |
| sim 30-59 | 124 | 56084 |
| sim < 30, "no counterpart" | 497 | 387772 |
| exact masked twin in JP retail / beta | 20 | 2092 (all PsyQ library: `CdMix`, `StartCARD`, `SsStart2`, ...) |

Reading. Nothing in the game code is an exact twin of JP retail or the beta: no function in this exe reappears
unchanged in a later build except SDK library code. The 497 "none" pieces (388 KB, 463 of them below 0x80087390) are
not 388 KB of cut features. TOMBA.EXE holds the code that PAL splits into MAIN0 plus the X000/X001 overlays, and this
early version was written differently. Evidence (sizes, not names): the "best PAL" name the tool prints is only the
nearest by shingles, but the sizes agree with the PAL function of the same job: the area-0 boatman/event scene
0x800841F0 is 1412 B (PAL `a00Variant0EventScene` 1704 B), the dark cloaked figure scene 0x80084934 is 1648 B (PAL
1676 B), rolling barrel 0x8007DF3C 1492 B (2548), rope chain 0x8003D40C 2628 B (2388), `weapon0Swing`-type 0x800560AC
6984 B (6188), spin-on-object 0x800462B0 5744 B (5740), pig enemy states 0x80070514..0x80078E84 (the PAL Koma Pig
set), chick/egg/treasure chest/zipline/solid block 0x80079698..0x8007DE78 (sim 40-70, named by PAL). So the village
(AREA00) actors of PAL already existed in June 1997. This is a size-and-structure judgement (guess for each
individual function; the pig, chick and chest groups are matched by sim 40-70 and are more solid).

### Biggest "no counterpart" functions, what they are

1. **Model vertex drawers 0x800294E8 (18360 B) and 0x8002DCA0 (8516 B).** *Verified as renderers*: 70 and 45 GTE
   (COP2) ops, 212 and 131 `lwc2/swc2`, no calls, a switch on the scratchpad model type word (0x1F800128 = 0/1/2).
   PAL's `drawModelVtx` (1976 B) and `modelDrawG4Lit` (608 B) are the same jobs compiled as loops; here they are fully
   unrolled. Not content.
2. **Player move code** 0x800560AC (6984 B weapon swing), 0x800462B0 (5744 B), 0x8005B3F8 (3640 B player sprite drawer
   with a debug read-out, see below), 0x800493F4 / 0x80047D44 / 0x8004B8A8 / 0x8004DCE0 ...: earlier forms of
   `weapon0Swing`, `playerStepSpinOnObject`, `drawPlayerSprite*`, swing/cling/flip/bitten states. They call
   `AnimJump`, `MulCos`, `MulNegSin` like PAL. *Guess by structure*: the same moves. One thing to note: all 5 pose
   "step" functions exist, so the pig-carrying pose and no swimming set (item 2 above) is data, not a missing code path
   (swimming is not handled by any function found; not checked in depth).
3. **Area-0 scripted scenes** 0x800841F0 and 0x80084934 (+ 0x800853A8 state object). They run a state byte at
   `D_800F4FDC/4FDD/4FDE` (6/1/0, 6/4/5, ...) and call 0x80035ADC (a message/box opener, args 2,0xD0,0xAC) 3-4 times: boatman
   and cloaked-figure events. *Verified as event scripts; which line is said not read.* PAL has both. No new event.
4. **Area-1 wave spawner 0x80083D88 (type 3, prints `enemy=%01d`).** *Verified from asm*: state 0 sets a timer
   from a random 0..7 index into the u16 table 0x800AF194; state 1 waits until the scroll X (0x1F80028A) passes one of 8
   thresholds in the s16 table 0x800AF1A4, then allocates an object (0x8001A36C) with kind 2, repeats up to 8 times. So
   it is a scroll-triggered enemy wave in area 1 room 0 (the only room that spawns it). *Guess*: what the enemy is
   (the pig/bird sprites of TIM000 are the candidates). PAL has no such spawner and no `enemy=` string.
5. **`TO BE CONTINUED...` end screen, 0x80021050 task.** *Verified*: when the state byte at 0x800DE4A3 is 2 the task
   draws the string at (20, 26) with 0x8001E2D4 (text draw), clears 0x800DE4A1/A2 and sets a 0x168 (360 frame)
   timer. Neither PAL, JP retail nor the beta contain the string. It is the end-of-demo card (guess: shown after the last
   room of the demo; what triggers state 2 not traced). `GAME OVER` is drawn by the same task, and exists in JP.
6. **Dead system/test tree** (0x800854B4..0x80087xxx, 0x8008585C MEMORY CARD TEST 2676 B, 0x800862C8 CDROM LOAD TEST, 0x800867E8
   SOUND OUTPUT TEST 1948 B, 0x80086F7C MUSIC PLAY TEST). See "Debug features". New here: the SOUND test selects reverb
   types (`ROOM`, `STUDIO(small/large)`, `HALL`, `SPACE ECHO`, `DELAY`, `PIPE ECHO`, `OFF`; depth with L1/R1, pitch with
   left/right, voice/tone/type numbers), the MUSIC test prints `BGM : %3d` and volumes, the MEMORY CARD test lists
   `bu00:`/`bu10:` cards with `IOE (FORMATTED)`, `NEW (UNFORMATTED)`, `TIMOUT (NO CARD)`, `ERROR (ERROR)` and dumps blocks
   (`(%2d) %02X:%02X:%02X [%6d] %s`). *Verified (strings and calls)*.
7. **Own CD file system**, strings `CD_newmedia: ... cd_read(PVD)`, `CD_cachefile: searching...`, `CdSearchFile: searching
   %s...`, `%s: path level (%d) error`, `CD001`: the exe reads the ISO PVD and path table itself and caches the
   directory to fill the 0x40-byte file table. No later build has these strings. Not content.

### Debug text in TOMBA.EXE that is not in PAL (all absent from JP retail, the beta and every PAL MAINn)

All are `drawDebugText` read-outs (0x8001E2D4 text + 0x800878B4 sprintf), stripped later. Only `hit pos`/`flag=` (JP and
beta) and `GAME OVER` (JP) survive in other builds.

| Where | Strings | Likely use |
|---|---|---|
| 0x800268C0 (camera/scroll) | `scrl_x/y/z=%05d`, `pl_x/y/z=%05d`, `anglX/anglY=%04d`, `area_pos`, `n_area_pos`, `pos_num`, `n_pos_num`, `rot_flag`, `tar_x/y/z=%05d`, `x=`, `z=` | camera with area-position waypoints (`area_pos`/`pos_num`) and a rotating target. The waypoint idea has a PAL echo only in the scroll controller; here it is a debug view. |
| 0x8005B3F8 (player sprite) | `plX:%4d plY:%4d`, `rot:%4d spd:%4d`, `spx:%4d spy:%4d` | player position, rotation, speed |
| 0x800656B0 | `MX:%2d MY:%2d` | message glyph / cursor cell |
| 0x8007029C (Koma Pig anim advance) | `char=%02d` | pig character index |
| 0x80079034 (`updateKomaPig` dispatcher) | `x=%05d y=%05d r0..r3=%01d`, `spdx=%05x spdy=%05x` | pig position, 4 sub-states, speed |
| 0x8007DF3C (rolling barrel) | `addx=%04x`, `x=%05d y=%05d` | barrel velocity |
| 0x80083D88 | `enemy=%01d` | wave counter |
| 0x800854BC | `IRQ=%d` | OPTION MODE |
| 0x80086F84 | `BGM    : %3d` | MUSIC PLAY TEST |

Also: `PLAY DEMO` and `RANKING DEMO` (title attract modes; PAL has no such text), `PSDEMO` (0x800C8074, checked at boot
0x8008735C/0x80087414, the PlayStation demo-disc marker), `@1997 WHOOPEE CAMP Co.,Ltd.`, and the 39 `\TOMBA\...;1` paths.

### Verdict

No new mechanic, area or NPC turns up in the function comparison: the exe is a smaller, earlier form of the same
engine, and its actors (pigs, chicks, eggs, chest, rolling barrel, zipline, boatman, cloaked figure) are the PAL area-0
set. What is new is the wave spawner, the `TO BE CONTINUED...` card, the debug read-outs and the test tree. Open:
trace what sets 0x800DE4A3 = 2 (TO BE CONTINUED), and identify the area-1 wave enemy by emulating room 1-0.
