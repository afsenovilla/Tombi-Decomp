# Hidden / debug features and cut content

Method: byte scans of MAIN0.EXE, MAIN1..8.EXE and every overlay on the disc for stores to scratchpad flags (`sb/sh/sw x, 0x1B4(reg)` etc.),
the Ghidra decompile of MAIN0 (`game/MAIN0_EXE_decomp.c`) for readers, and `tools/psxemu` (poke the flag, drive the pad) for verification.

## Result in one line
There are **no button-sequence cheats** (no table of pad codes with a counter) in the pad readers of MAIN0 that were reviewed, and **no code
anywhere writes the debug flags**: they stay 0 unless set from outside (GameShark / emulator poke). All of it is dead debug code left in the retail game.

## Debug flags (scratchpad, 0x1F800000 + off). Nothing in MAIN0..MAIN8 or any overlay stores to these offsets
| flag | read by | effect when != 0 | verified |
|---|---|---|---|
| `0x1F8001B4` `debug_mode_enabled` | `introSequenceHandler` (0x80019D88, 2 reads), `displayDebugScreen` (0x8001A0F0), `debugSelectHandler` | NEW GAME opens "AREA SELECT = nn / SECTION SELECT = nn" (gameplay mode 0) instead of the village; UP/DOWN pick the row, LEFT/RIGHT change value (clamped by the section table at 0x80078790), CIRCLE/START loads that area+section. Also re-entered when Tombi dies in a debug-started game. | yes, PNG `debugselect` (title -> poke -> START; reproduced by `capture.base_state`) |
| `0x1F8001AB` | `initGameConfig` (0x8001783C) | NEW GAME gives a **debug loadout**: equipment/inventory bytes at 0x8009D1A4.. = 1B 21 22 15 16 17 20 1D 18 1C 05 06 24 14, `0x8009D2A4` = 0x0E (instead of 0x20 / 2), and ~12 progress flags (0x8009D0A9..0x8009D0C8) set to 1. | yes (poke before START: bytes read back as listed, game starts normally) |
| `0x1F8001B3` | `initGameConfig` | != 0 skips both the normal and the 1AB loadout (leaves the config as the caller set it). | no (static) |
| `0x1F8001BD` | main loop (`main`, 0x80016604) | while R2 is held (`pad & 0x200`) and a display mode flag (0x1D0/0x1D1) is on: the D-pad moves the display environment (`DAT_8009BC80/82` +-4 per frame) = screen position adjuster. | no (static) |
| `0x1F8001BE` / `0x1F8001BF` | main loop (0x80016824 / 0x80016790) | frame control through `0x1F8001F0` / `0x1F8001EE`: with 1BE: L1 (0x400) toggles pause (1EE), L2 (0x100) held steps / freezes (`1F0 ^= 0x4000`, the thread table is not run when 1F0 > 0x4000); with 1BF: L1 flips `0x8000 - 1F0`. Needs 0x1CC == 0, 0x1CE == 1 and 0x1D0/0x1D1 != 0. | partly: poke 1BE = 1 in AREA00, L2 sets `1F0` to 0x4001 (freeze); L1 toggle did not change 1EE in the test (not understood) |
| `0x1F8001BB` | area task step (0x8001A810, `FUN_8001A64C`) | != 0 stops the countdown `0x8009C980` that ends the mode (timer / auto exit of a state). | no |
| `0x1F8001B8` | `FUN_8001CA58` (0x8001CBD8) | != 0 blocks the button-mask check (`0x1F8003CA & pad`) in a menu = disables that input. | no |
| `0x1F8001BA` | 0x8002D89C | read only, purpose unknown. | no |

`fontDebugPrintf`, `SetGraphDebug(0)`, `CdSetDebug` are Psy-Q leftovers (printf into a stub); the debug screen text is drawn by the game's own font.
No other debug menu or unreachable screen found: `displayDebugScreen` / `debugSelectHandler` are the only ones named.

## Title screen
`loopTitleScreen` (0x8004E41C): holding RIGHT-ish bit `0x20` in `0x1F8001F8` (held pad word) draws one extra sprite (`func_800E972C(0x38, 0xA0, 0)`), probably a
cursor/hand; not a secret. Button sequences: none (only UP/DOWN/CROSS/START handling; menu has 3 entries incl. the options).

## Event titles (CFNT999*.WFM, 136 entries 0..135)
The title text of event e is `D_800806B4[area][e]` (RAM 0x80080410, 200 bytes = 200 events, identical for all 20 area slots; the 0x800806B4 array holds 20
pointers to it). Layout record list = area-overlay table `[e]`.
- Referenced entries: 2..134 except **90 "The Black Water"** and **126 "The Rusty Steel Car"** (English; 90 / 126 are never selected by any event id;
  the other 131 distinct entries are used, with a few unused slots 0xFF in the table: events 6, 18, 45, 48, 52, 58 .. = no title, fallback entry 1
  "!!!DUMMYCUBE!!!").
- **Entry 52 "MiniatureMakingTrumpets" IS in the table (event 32)**, so it is not removed from the title table; it is the one without a layout list in the
  aligner (`tools/title_layout.py`), the alignment of lists to dialogues is by length so the "missing lists" are 4 guesses (52, ~90, ~126, 135): 90 and 126
  being unreferenced fits the "no list" for them. 135 ("......................") is past the table end (max 134) = placeholder; 0 ("CLEAR!") is the banner bitmap source only.
  Whether event 32 can ever be awarded was not checked (script op not located; `awardEventProgress` is reached from script op 0x8003AA68 `scriptOpAwardEvent`,
  which is not in a MAIN0 table, so the scripts use the overlay op table). Not resolved: tracing the per-area table at 0x8013A044 (it holds a pointer and
  a byte table, not the pointer array the decompile assumes).

## Event titles in other versions (tools/event_titles.py)  [V]
- **Beta 980203**: one 200-byte table (exe 0x6623C), byte-identical to PAL (90/126 unreferenced too).
- **JP (SLPS-01144)**: one 200-byte table per area into that area's own title list (WFM2 block 1 of AREAnn/A00x.GAM; AREA01/A008 and AREA18/A003 have a block whose dialogue-table word is only valid as u16, so tools/jp_text.py skips them). Tables: X00 0x4E0B4, X01 0x5151C, X03 0x4B850, X04 0x45E80, X06 0x358B8, X09 0x409B0, X10 0x44068, X11 0x2EE74, X13 0x2D164, X14 0x39F08, X16 0x2E638, X17 0x2E8F0; exe file offsets 0x662D0 (area 2), 0x66438 (5), 0x665A0 (8), 0x66708 (18), 0x66870 (19). PAL's 20 identical pointers are the remains of this.
- Only JP title text no table uses: 風になりたい ("I want to be the wind", AREA19 list). CLEAR!/dummy as in PAL.
- Events PAL leaves 0xFF that JP titles (JP description from SYSTEM/DSPSUB.BIN block 1 in brackets):
  6 大きな水たまり？ (Big puddle?) · 18 ドワーフの助け (Dwarves' help; area 0: 魔女の忘れもの) · 45 ＡＰスロットマシーン · 52 渡れなくなっちゃった… (Can't cross any more) ·
  59 族長のたのみごと？ · 60 コトバを吸いとろう！Ⅱ · 61 マサカリ族のヒミツ · 67 解読？らくがき岩 (Decode the graffiti rock) · 74 電気うなぎを捕まえろ！ ·
  **76 黒い水の不思議 = PAL #90 "The Black Water"** · **83 動かない鉄の車 = PAL #126 "The Rusty Steel Car"** (so both "unreferenced" PAL strings are just unwired: 76/83 are 0xFF in PAL) ·
  89 緑の占い師 (Green fortune teller) · 94 仲良くしようよ · 95 木人の村の大岩 · 96 未完の橋… · **97 = 0x61 動かない時計台 ("The clock tower that won't move", [clock with stopped hands])** ·
  98 こわれない歯車？ (Unbreakable gear?) · **99 = 0x63 材料はどこだ！？ ("Where's the material?!", [something tougher than stone])** · 100 鉄のカギ穴 (Iron keyhole) ·
  138 つかまえてごらん · 139 ３姉妹のかくれんぼ · 140 ３姉妹への手紙 · 184 おいら、悪い子？ · 186 マサカリの集落？
- The other cut-town events are titled in PAL already: 0x3E #89 "Tree of Knowledge Knows", 0x4F #98, 0x67 #100 "The Cute Witch", 0x69 #94 "I Need a Bomb".
- Port candidates: 0x61 -> new string "The Clock Tower That Won't Move" (started by Stonetown npcSet0ESceneStep1, the stopped-clock talk); 0x63 -> "Where's the Material?!" (npcSet0FSceneStep2, the smith / iron gear item 0x3E); 0x62 "An Unbreakable Gear?"; 76 -> #90, 83 -> #126 (existing PAL strings). No version gives English text for 0x61/0x63.
- Still 0xFF everywhere checked: 48, 66, 68-72, 75, 84, 91-93, 101, 106-110, 117, 122, 130, 131, 136, 137, 141-143, 147-149, 152, 154, 155, 158, 162, 163, 165, 192-199.
- USA / jpdemo / PrePre8: dumped by the report.yml jobs (event_titles.py). PrePre8's TIM title strip (docs/PREPRE8.md) adds 少年旅立ち and あれま大きな水たまり (= event 6).

## Not done
- Exhaustive proof that no overlay has a button sequence (checked: MAIN0 pad readers; overlay pad code not audited one by one).
- Unreferenced dialogue / room / item text.

## Follow-up research: event 32 / titles 90, 126 / dialogue references
Legend: **[V]** verified by reading the binaries (scripts in this section: `tools/dialogue_refs.py`; the event census was a throw-away scan of `jal` sites), **[I]** inferred.

### Event award paths  [V unless marked]
- The award op of the script VM is **game opcode 0x9a** (`execGameOpcode` -> `FUN_8005a7a0` = `awardEventProgress(arg0 = event, arg1 = 0 started / 1 complete)`), not
  `scriptOpAwardEvent` (0x8003AA68): nothing references that address (no jal, no table word in MAIN0 or the overlays), so the earlier note above
  ("reached from script op 0x8003AA68") is wrong; it is a dead psx_tomba-named stub.
- Event ids reach the three routines `awardEventProgress` (0x8005A7A0), `setEventStarted` (0x8005A8A8) and `setEventComplete` (0x8005A9A4) from: (a) MAIN0 and the area
  overlays X0nn / I?NFO* code, (b) script op 0x9a, (c) `rewardJewel` / `rewardPigBag` / `rewardMysteriousMushroom` (MAIN0, immediate ids).
- **Scripts are only 9 files**: `AREA15/{0000,0120,0130,0140,0200,0201,0210,0220,SCR0100}.WSS` (magic `W\0S\0`, u32 size, 64 x u16 entry table from +8, code at +0x88;
  `05 nargs [kind imm32 | 00 var]* opcode` loads the argument registers, then the 1-byte game op). Events awarded there (`05 02 01 id 01 flag 9a`): 1, 12, 13, 14, 17, 20, 21, 23, 25, 26, 27, 80, 86,
  87, 88, 113, 151, 168, 177, 179, 181, 187, 188, 189. **32 is not among them.** (Many of these small numbers are probably not event ids of the title table: 86/87/88/113 etc. look like
  item/other ids of the same op family; not checked.)
- Static `jal` census with an immediate in a0 (MAIN0..8, all unique X*/I* overlays): 116 distinct ids are referenced with a constant. **Event 32 is not one of them**, in no
  language. Neighbours: AREA03 completes 29, 31, 33, 35, 36 and starts 30, 31, 33, 35 (X003); 32 and 34 are the gaps. Titled events (title table byte != 0xFF) with NO constant reference
  at all: 0, 4, 9, 10, 11, **32**, 34, 41, 42, 44, 57, 77, 85, 111, 124, 133, 135, 170, 172. Event 4 is known to be awarded in game (motocross, AREA00), so these are awarded with
  **data-driven ids**: the AREA03 sites are `lh a0, 0xAC(obj)` (X003 0x8011AD10, 0x8011AD54...), the id is a field of an object filled from room object data (not
  located; no overlay writes +0xAC). Conclusion on #52: **not proven either way.**
- Event 32 has real AP values: `EVENT_STARTED_AP_TABLE[32] = 2` (AP_TABLE 1000) and `EVENT_COMPLETE_AP_TABLE[32] = 3` (2000)  [V]. The events that really are dead (title byte 0xFF) all have AP 0/0
  (6, 18, 45, 48, 52, 59-61, ...). So event 32 is a **fully designed event** (title + AP), unlike event 52 (note: event 52 is a different thing: no title). [I] It is probably awardable
  through an AREA03 object whose id comes from room data (its neighbours 31/33 are AREA03 events and the US name list puts MINIATUREMAKINGTRUMPETS between them), or it is a cut event whose
  trigger object was removed from the room data. To settle it: find the object spawn records that fill obj+0xAC (X003 handlers 0x8011AD10 / 0x8011AD54) or run AREA03 in `tools/psxemu` with a watch on 0x8005A8A8/0x8005A9A4.
- Title table: `D_800806B4` (20 identical pointers) -> 200 bytes at 0x80080410 [V, same in MAIN0..8]. Only `spawnEventTitle` reads it (code scan for lui/addiu 0x800806B4 in MAIN0..8 and every overlay: single reader).
  Title entries 0..135 never selected: 0, 1 (fallback for 0xFF), **90, 126** and 135 [V]. So **90 "The Black Water" and 126 "The Rusty Steel Car" are unreferenced** (no event id maps to them and
  no other code reads CFNT999 dialogues, since the table reader is unique) [V]; entry 52 is selected only by event 32 [V]. That they were titles for cut events [I] (their neighbours in the table are in
  event order 89 -> 0xFF, 91 -> 0xFF: the events 89 / 91 exist but have no title, AP 0 [V]; 90/126 titles belong to the 0xFF slots 89/91 or 122/124 by order [I]).

### WFM3 header bytes 16..143 are a group table  [V]
Not "reserved": 64 x u16 bases. `openMessageBox(k, dx, ...)` (src/openMessageBox.c) shows dialogue `*(u16*)(wfm + 0x10 + 2k) + dx`; `showMessageBox*(k, dx, x, y)` pass them through, script op 0x85 (`scriptOpShowMessageBox`
cmd 4 / 8) takes k = arg2, dx = arg3 (WSS: `05 04 01 obj 01 4 01 k 01 dx 01 ... 85`). Group 0 (base 0) = system / items / info text (207-208 entries); groups 1.. are small blocks, e.g. MFNT030S: bases 0, 207, 211, 227, 232, 234, 237, 249, 254
(group 1 = save-point name + a filler, 2 = a character's conversation states...). `k = 13` is called with dx 2 from the overlays (base 0: dialogue 2).

### Unreferenced dialogue  [I, weak]
`tools/dialogue_refs.py` lists, per MFNT0nnS file and group, the dialogues for which no static caller (jal with immediate k/dx in the area overlay, or WSS op 0x85) exists. It over-reports massively: whole groups
of ordinary NPC speech (MFNT020S group 6, the digger's story, 20 lines; MFNT000S groups 7, 9) show up as "no reference" because NPC objects read (k, dx) from room data / state variables (7-12 call sites per area have
computed arguments, many more NPCs are dispatched by a table). So **no cut text could be established**. Cheap hints only (not proof): filler lines that repeat or are punctuation only,
e.g. MFNT020S group 4 `·····???` x3 and group 8 dx 1 `·····???`, MFNT030S group 1 dx 1 `..........???`, MFNT030S group 3 dx 0 and 2 both `...¡Funga!`; MFNT000S group 8 `···`. Save-point
name dialogues (group 1 of most areas: "Montaña tormentosa", "Laberinto subterráneo", "Ciudad de Enanos" followed by `¿Guardar DATOS?`) are never called by an immediate either (the save point object passes the area/section).
Needed for a real answer: decode the NPC object spawn data of the room files (which holds k and dx).

## Room object records: where the data-driven events live  [V = verified in code/emulator, I = inferred]
Format and tool: `notes/formats.md` "Object placement records", `tools/objects.py` (`events`, `list --event N`). The task premise "event 32 is a field of the room record
(obj+0xAC)" is only half true: **+0xAC is written by the object handler, not by the spawner**; the record only supplies the object type and subtype.
- AREA03 `func_8011A874` (and AREA04/12 `func_8011B7C4`) is layer 4 type 7 (`dispatchAreaInitB`): `o->wac = <const by subtype>` then `setEventStarted/Complete(o->wac)`
  (the `lh a0, 0xAC(obj)` sites X003 0x8011AD10/0x8011AD54). AREA03: subtype 0 -> **41**, 4 -> **170**, 8 -> 55; AREA04/12: 0 -> 41, 4 -> 51, 8 -> 55, 10 -> **42**. [V]
- Other non-constant award sites are `lh a0, 2(s)` = `e[1]` of scene sequences ("event cutscene" objects, layer 2) and the AREA14 controller table. [V]

Placement of the 19 events whose award had no immediate (`objects.py events`, own-slot records only):
| event | where | how |
|---|---|---|
| 0 Grandpa's bracelet | AREA13 s1 (type 0x56 scene sequence, X013 op 7 arg 0 at the end of the sequence) | sequence [I: arg 0 read as event 0] |
| 9, 10 | AREA00 s3 (area-wide record, layer 2 type 0x34, X000 sequences) | 9 started (ops 9) / 9 and 10 completed; also 1, 12 |
| 11, 57 (and 58) | AREA04 and AREA12 s15 (type 0x35, X004 sequences) | 11 completed (flag check), 57 started+completed, 58 started |
| 34, 85, 111, 124 | AREA14 s3, s5, s1, s2 (controller types 0x3E, 0x40, 0x3C, 0x3D) | `D_801256A8[section]`, on clearing the room |
| 41 | AREA03 s1 and s5 (type 7 subtype 0), AREA04/12 s0 and s1 | door/switch objects |
| 42 | AREA04/12 s0 and s1 (type 7 subtype 10) | " |
| 170 | AREA03 s1 and s5 (type 7 subtype 4) | " |
| 133, 135 (and 134) | AREA06 s2 (type 0x51, X006 sequences: start via op 10 / complete via op 11) | sequence |
| **4, 32, 44, 77, 172** | **no record/handler found that can award them** | see below |
Also awarded through the same objects (not in the 19): AREA14 30/54/80/160, AREA06 121, AREA02 144/156, X001 164/180, AREA10 150, AREA09 189.

**Event 32 ("MiniatureMakingTrumpets", title #52): no room object awards it.** It is not an immediate, not a sequence/controller/door id in any overlay,
not in the 9 WSS scripts, and no record field carries an event id. Remaining blind spots (so "not proven impossible"): the layer-8 type-1 object
`func_8002B0BC` (`setEventStarted(o->wd2)`, spawned only by X001 code, field filled elsewhere) and overlay functions with no C source yet.
Together with its full AP table entries (2000/1000) the best reading is a **cut event** whose trigger (neighbours 31/33 in AREA03) is gone: AREA03 s1/s5 only holds the doors 41/170/55.
Same verdict for 4 (motocross course; AREA06 awards 133..135/121 via its sequences), 44, 77, 172.

Dialogue references: NPC `(k, dx)` are literals inside the handlers (per state: `m = 8; n = 0..4` ...), NOT record data; room data only selects the variant (subtype). `tools/dialogue_refs.py`
now joins the matched C sources (+~100 pairs: e.g. X003 41 instead of 30, X010 47 instead of 26) with the static call scan; per-group "unreferenced" lists got shorter but remain
a candidate list (handlers without C source and `D_xxx[state]` indexed dialogues are still invisible). `objects.py dialogues` lists the pairs per room (only for handlers with a src file).
