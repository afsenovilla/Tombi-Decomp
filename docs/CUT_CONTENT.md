# Cut content: inventory and status

Spanish twin: [CONTENIDO_RECORTADO.md](CONTENIDO_RECORTADO.md).

Other builds surveyed for cut content: [PREPRE8.md](PREPRE8.md) (PrePre Vol. 8, 1997-06), [JPDEMO.md](JPDEMO.md)
(SLPM-80165) and the English preview CD-R of 1998-02-03, [BETA_980203.md](BETA_980203.md) ([español](BETA_980203_ES.md)):
early English script, a subtitled 100US movie and an instrumental opening, but no cut Civilization data.

Everything the port restores is **opt-in** (`tombi2 --cut-content`, the launcher's "Enable cut content" checkbox) and
**port-only** (`port/fixes.txt` rules or `port2/runtime` code). The matching build is untouched and stays
byte-identical (`tools/build_full.py`, every sha1 OK). Nothing from any disc is shipped: text comes from the player's
own disc (or a bundled translation we wrote), art from the player's own discs.

Status: **restored** (playable with `--cut-content`), **possible** (data and a hook exist, not done yet; cost noted),
**lost** (the data needed is not on any disc we have).

What the game data says and what is our choice are kept apart in every entry ("Ours:").

## Restored

| Find | Evidence | Port change | Ours |
|---|---|---|---|
| Boatman's three extra lines (AREA00) | MFNT000 #162-164 = (7,3..5); `a00Variant0EventScene` (X000 0x801363E4) moves state 2->3 without opening a box; JP X00 0x80134AD8 does the same. JP text in Ore! Tomba. | fixes.txt `a00Variant0EventScene` (commit 43855ca) | none (they follow his last line) |
| Baccus Village letter (AREA03) | MFNT030 #208 = sign (1,1) = `D_80079EBC[27]`; no subtype-27 sign is placed in AREA03 (PAL and JP). | fixes.txt `updateL4Type1AObject` spawns a subtype-27 sign 40 px left of the save sign (43855ca) | placement |
| Baccus notice board (AREA05 s1) | `spawnNpcSet09` (X005, table `D_80117AC8[GCD.bceed]`) does spawn the board sign: subtype 19 = MFNT050 (1,5) MISSING MOUSE (reading it starts event 46 "Where's the Baby Mouse?"), 20 = (1,7) ADVENTURER WANTED once the mouse is back (`bceed` 2; starts event 49 "A Drink for Grownups"), 35 after event 50. But it stands at (28,-144) depth 90 beside the townsfolk who mentions it, and nothing Tombi can stand on is within its hit depth (ledge above the wall at depth 144, \|dz\| 54 > 45; the ground 88 px too low): never readable, so events 46/49 never started from it. (1,6) CONGRATULATIONS has no sign-table row at all. `tools/dialogue_refs.py` missed the code-spawned sign (it reads only room lists). | fixes.txt `updateL4Type1AObject` + runtime/main.c `port_cc_sign`: the sign is moved to the ground under the painted board (28,-60, depth 0); read with Up + Square. `port_cc_sign_text` / `port_cc_sign_next`: when the mouse is back and event 49 has not started, (1,6) is shown first and (1,7) follows in the same read (then event 49 starts as in the code). | ground position; (1,6) before the first (1,7) |
| Mermaid's Singing Rock save (AREA06 s1) | MFNT060 (1,0) "The Mermaid's Singing Rock / Save DATA?" in no table row nor room; the race man (1,1): "the finish line ... is near the Mermaid's Singing Rock". AREA06 s1 (finish beach) has the record board (subtype 36) only. | `port_cc_sign` spawns a port sign (subtype 0x40+i, text from `port_cc_saves`) 48 px left of the record board; it saves like any save sign (block named "La roca de la sirena"). | placement |
| Silver / golden berries | `initItemObject` CLUT rows 503/504; `rewardHeart` pays 2 HP when `b0c & 0x7f != 0`; no drop table uses b0c 2/3. | fixes.txt `spawnItemFromEntry` + `initItemObject` (dafdbcf) | rates 1/8 silver, 1/32 gold |
| Big "?" bubble | `D_80079D34[1]` (SPR 0x34 f48); `spawnItemPickupObject` is only called with 0. | fixes.txt `topdownActorOpenDialogue` (dafdbcf) | when it shows |
| **Early title screen** (PrePre Vol. 8) | PCPX-96084 `SYSTEM/SYS005.GAM` (8 bpp, 768x256 page; x 0..319, y 0..239 = landscape with a rock spire and a river) + `SYS006.000` (its palette). Retail draws its landscape `SYSTEM/A00200` (8 bpp, 320x240) from VRAM (0,0) with the palette at (128,511), so the old picture fits the same slot. | `--early-title DIR` (needs `--cut-content`), `port2/runtime/titlebg.c`: while the title is up (`titleLogoDraw`, via `port_title_bg_mark`) the page and palette are written over A00200's. Launcher: folder field under the checkbox. | We keep the retail TOMBI! logo, menu and text: the old "オレっ！トンバ" logo pieces are on the same page (x 512..767, palette SYS007) but how they were assembled is unknown. Files are read from the user's extraction (`tools/prepre8.py extract DISC OUT`). |

Screenshot check (headless port, frame 400 of `--skip-intro`): retail landscape vs PrePre8 landscape under the retail
logo.

## Possible (not done)

| Find | Evidence | What restoring needs |
|---|---|---|
| "DIRT MOTOCROSS RACE  Let's challenge it!!" sign | MFNT000 #165 = (8,0); no call with k=8 in X000/MAIN0 (PAL or JP). PrePre8's event-title strip has モトクロスコース. | A sign that maps to (8,0) (a port-side subtype; `D_80079EBC` has none) and a place for it: **no evidence where it stood**, so placement would be ours. Cheap (same pattern as the letter). |
| AREA00 hint signs per button config | MFNT000 k1 dx 8,10,14,15,21,22 and 24..29: variants of hints PAL already shows; `D_80079E8C..EB0[BUTTON_CONFIG_INDEX]` never selects them. | Nothing new to show: the same advice exists. Not worth it. |
| Dwarf woodcutter lines (AREA02/19) | MFNT020 (2,54..56) "Dwarves... good at cutting trees", "you want a tree cut down?"; no WSS literal/variable reaches dx 54-56. Unused event 18 "The Dwarves' Help" ("Ask the lumberjack dwarf for some wood"). | A WSS script change (AREA15/02xx.WSS op 0x85) and a dwarf actor that offers it; medium. |
| AREA02 group 3/4/6 lines ("We're being pushed back!", "Big brother... hurry!") | MFNT020 (3,5..6), (4,0..2), (6,16); X002 calls only k 2, 3 (dx 0-4), 7, 9. | No scene to hang them on: medium-hard. |
| Unused items (47; 21 with a JP description) | `scratchpad`-built list `unused_items_jp.txt`: e.g. Rain Essence (52), Torn Map halves (57/58), Rubber Gloves (59), Iron (61), Letter (90), Wood (91), Shovel (97), Electric Eel (108), Black Water (109), five Candies (110-114), Funga Leather/Tree/Sap (119/122/123), Small Drum (137), Chief's Spear (140), Coal (147). Name = MFNT id+3, description = D0SPSUB block 0 id+4 (PAL English "···???"). | Giving them is one `addItemToInventory`, but nothing uses them; most belong to the cut Civilization / Masakari chains. Only a viewer would be honest. The JP descriptions could go into a text patch. |
| Unused events (68; 26 with JP info) | `unused_events_jp.txt`: e.g. 6 "A Big Puddle?" (PrePre8's strip has あれま大きな水たまり: the same idea, now confirmed in two builds), 18 Dwarves' Help, 45 AP Slot Machine, 52 "Now I Can't Cross...", 59-61 Masakari chain, 74 Catch the Electric Eel, 76 Black Water, 77 Civilization Machine, 83 Iron Car. No `setEventStarted/Complete` with those ids anywhere. | Each needs its own trigger and goal; none has code. Medium-hard each; only the titles/descriptions are cheap (text patch). |
| Event titles left 0xFF in PAL | JP per-area tables (`tools/event_titles.py`, notes/secrets.md) title 25 of them: 76 = PAL #90 "The Black Water", 83 = PAL #126 "The Rusty Steel Car" (the two unreferenced PAL strings), cut-town 0x61 動かない時計台 "The clock tower that won't move", 0x63 材料はどこだ！？ "Where's the material?!", 0x62 "Unbreakable gear?", plus 6, 18, 45, 52, 59-61, 67, 74, 89, 94-96, 98, 100, 138-140, 184, 186. Beta 980203 = PAL table. | Table patch: 76/83 reuse existing strings (cheap); others need new English strings. |
| Port: titles for the 0xFF events | Done in the port with `--cut-content`: 76->#90, 83->#126 and 22 titles translated from JP (list in notes/secrets.md; tools/port_event_titles.py). USA and beta 980203 have the PAL table. | Sub-screen event list not changed. |
| PrePre8 event titles 少年旅立ち / あれま大きな水たまり | TIM000 16x16 glyph strip (x 0..224, y 104..232). | Text only: a WFM3 event-title string (the encoder exists). Needs an event to carry it (see event 6). |
| PrePre8 elder's-house 2D interior | `TIM007` 8 bpp + CLUT00 row 20, `DATA002` (collision only). | A 2D-room path PAL does not have for that house; best as a gallery. Medium-hard. |
| PrePre8 grey bird enemy | TIM000 (~16 poses); candidate type 15, `func_8003D40C` in TOMBA.EXE. | No PAL AI matched; decoration or gallery only. Hard for gameplay. |
| PrePre8 15 sound samples | JOCHU 1, 5, 11, 12, 19-21, 23-25, 27; S_AREA01 0, 25, 28, 29. | Easy to bank; no evidence of where they played. |
| PrePre8 OPTION MODE / enemy viewer | 0x800854B4 (`OPTION MODE`, music/sound/CD/memory-card tests), 0x80083D88 (`enemy=%01d`), dead code in that build. | Port-only re-implementation; PAL has no such code. |
| GoGoCart cacti / sign (AREA06) | VRAM of `A001.GAM` (577,256), (516,256), (581,348); no SPR part nor model polygon; D004.000 has 60 models and `a06AddModelsBySection` uses all 60. | New sprite parts and placement: ours. |
| AREA17 B201 page (backpack man, strap-shirt big man, bald old man, helmet walker; side view) | **Not a cut-content finding: withdrawn.** The user reports the backpack man in the final game. The same 192 x 256 page is loaded, byte-identical, by AREA10 s0 (B002, VRAM x 384) and s4 (B403, x 640), AREA16 s5/s6 (B501, B601), AREA17 s2/s5 (B201, B501) and AREA18 s1 (B101). Our static checks find no reference to its figures: no SPR table frame of any area list whose parts bound one of them, no WPP bank holding their pixels, no 16-byte part record pointing at them. These checks miss code-built primitives and other drawing paths, so "no static reference" here does not mean unused. The retail section, SPR id and address are not known yet. The B201 page renders with a real palette: AREA11/CLUT02 row 36 (same row in AREA17/CLUT03, AREA17/CLUT06, AREA18/CLUT02; the Male Lumberjack Town Resident's row) colours its side-view man with a backpack exactly like TCRF's colour sprite (all 14 colours in that row; silhouette close, not pixel-exact, TCRF's frame differs). Our identification. | Find the drawing path (dynamic capture in AREA10 s0/s4 or AREA17 s2) before calling any frame unused. |
| AREA19's own overlay `X019.BIN` (and `X119`..`X819`) | On the disc (`SYSTEM/` and `AREA19/`, 190,180 B, sha1 `94060c10...`) but **never loaded**: none of the nine file tables lists it (notes/cdfiles.md), and AREA19's load lists name `AREA19/X002.BIN`, byte-identical to AREA02's `X002.BIN` (sha1 `646aa8b5...`). JP does the same: AREA19 ships `X02.BIN` and `X19.BIN`, its lists name only `X02`. So the purified village runs AREA02's code; X019 is a separate, dead AREA19 build. | Study what X019 does differently from X002 (its area part, from 0x80115EA4); a port switch to run it would be ours. Unknown cost. |
| Small "?" bubble | SPR 0x34 f49; no script, no pointer. | Graphics only. |

## Lost

| Find | Why |
|---|---|
| Civilization towns (AREA11/16/17/18 top-down sections) | Owned by docs/CIVILIZATION.md (`--civ-test`): code in I0NFO2 is complete, but the sections' load lists are empty and their map / SPR / palette files are on no disc. 63 lines (class B*). |
| Growing golden flower frames (AREA02 SPR 0x3B f27-32) | The rectangles they point at are empty in PAL VRAM: the art is not on the disc. |
| JP-only text of class D tied to Civilization (MFNT110/170/180: raft maker, generator, coal, Dr. Gantetsu, new town) | Their scenes are the cut top-down towns. |
| PrePre8 dialogue script | The 10x10 font is there but no script text using it was found (still open, see docs/PREPRE8.md). |

## JP-only text survey (SOLO_JAPON, 179 candidates)

JP dialogue whose PAL slot is empty or filler, aligned by control-code structure (SLPS-01144 vs PAL). Almost all is
already above: Civilization towns MFNT110/160/170/180 (116, see Lost / docs/CIVILIZATION.md), unused item
descriptions (DSPSUB#0, 21) and unused event hints (DSPSUB#1, 7) (Possible: unused items/events), AREA00 hint variants
per button config (14), boatman lines (MFNT000 JP #156-158), motocross sign (#159), dwarf woodcutter lines (AREA02/19,
6), AREA02 group lines (7), Baccus letter (AREA03 #178). New, minor:

- AREA00 JP #111 `ポスト　あけるな！` ("Mailbox: don't open!"): no PAL slot. Unplaced.
- AREA03 #174 and AREA10 #171 `ポテンシャルが　たりない` ("Not enough potential"): PAL slot is filler. Probably an
  AP/requirement message; not traced to code.

Minor: the JP disc has empty folders `AREA20`..`AREA24` (PAL has none); nothing in them, nothing loads from them.

## How to run

```
tombi2 --data data --cut-content [--early-title /path/to/prepre8_extract]
python3 -I tools/prepre8.py extract PREPRE8.bin /path/to/prepre8_extract   # once, from your own disc
```

## Event icons (the community "item icon" sheet)

The community sheet of icons is the **event list** page, not the inventory. `tools/event_icons.py` (list / sheet / compare) reads it from all six discs.
- **Icon of event k:** SPR frame `*ICONS[k]` (PAL D0SPSUB `D_800FE974`, 192 events), taken from the sub-screen sprite table (kind 0x39: PAL `AREAnn/A1nn.GAM`, 267 frames; USA/JP/beta inside `AREAnn/A003.GAM`). Each icon is one 24x16 part of the 4 bpp page at VRAM (256,0), which is PAL `SYSTEM/A00300.000` words 128..191 (USA/JP/beta `SYSTEM/A00014.000`).
- **CLUT:** column x=320 of the 48x31 block that D0SPSUB uploads to (288,480) (file +0x98B8). `subScreenDrawEventIcons` forces 8 CLUTs.
- **Menu order:** the menu shows only the 130 events in `SUBSCREEN_EVENT_ORDER` (value = event+1), and only after they are completed.
- **Item icons** are a separate page: VRAM (192,0) = A00300 words 64..127, cell `n%15*16+8, n/15*16+8`, CLUT row `ITEM_ICON_CLUTS[n]` (0x80109C88) at x=304. Cells 160-164 are empty.

**PAL/USA/JP/beta have the same 166 icon frames** (JP numbers them 17 lower). On PAL, 40 frames are never shown; the uv/CLUT of each is in `event_icons.py list pal`:
- **No event points at them (16):** 116 bell+key, 127 pig snout, 129 lightning, 133 flower, 137 stone gate, 140 key+red blob, 156 pink pig, 168 blue orb, 173 chest, 196 Tomba in water, 198 black eyes, 248 necklace. Three frames reuse the pixels of another frame with a new CLUT: 253 (115, CLUT row 495, the grey "rusty steel car"), 255 (139, grey slab) and 266 (210, green hourglass).
- **Event not in the menu order:** 107 (ev 6, Big Puddle), 115 (ev 18), 139 (ev 45 AP slot machine), 159 (ev 94), 172 (ev 32 trumpets), 183 (ev 52), 192 (ev 59), 193 (ev 184), 194 (ev 61), 197 (ev 67), 199 (ev 74 electric eel), 200 (ev 75 footprint), 201 (ev 76 Black Water bucket), 203 (ev 186), 205 (ev 143), 209 (ev 100 Iron Keyhole), 210 (ev 98), 213 (ev 96, orange gear), 214 (ev 97), 216 (ev 99), 226 (ev 140, the three sisters), 227 (ev 89 green fortune teller), 230 (ev 95), 249 (ev 122), 253 (ev 83 Rusty Steel Car).
- **In the order but never awarded:** 202, the grey gear (ev 77 "The Civilization Machine").

**Civilization-related icons:** gears 202 (ev 77) and 213 (ev 96); Iron Keyhole 209 (ev 100); Rusty Steel Car 253 (ev 83); Black Water 201 (ev 76); "Where are the materials?" 216 (ev 99). The bomb icon is used: frame 211 belongs to retail event 106.

**The community "Unused" row (22 icons):** every icon in it is in the 40 frames above, except the first one, the red-jewel ring. That ring is frame 101, the dummy icon, which retail event 0 (Grandpa's Bracelet) uses. **New, not in that row:** 107, 115, 139, 140, 159, 172, 173, 183, 192-194, 197, 199, 202, 203, 205, 210, 213, 214, 216, 230.

**Demo exclusive** (pixel data that is not on the retail page):
- **SLPM-80165** (page `SYSTEM/A00012.000`, CLUTs from `DSPCLUT.TIM` column 320): frames 79 puddle (ev 6), 87 axe (ev 18), 110 red-hat dwarf (ev 44), 111 slot machine (ev 45), 112 "II"+bowl (ev 60), 136 pig (ev 167), 144 coin (ev 32), 151 jar (ev 49).
- **PrePre8** (`SYSTEM/SYS008.000`, sprites in `AREA00/DATA000.GAM`, CLUT `CLUT00.GAM` x=608): 59 puddle (ev 6), 67 axe (ev 18), 72 green-hat dwarf (ev 14).
- **Retail:** keeps the event ids. Events 6, 18, 32, 45 and 60 got new, unshown art. Events 44, 49, 167 and 14 are used in retail with a different icon.

## Events with no PAL title (0xFF): where they are started

Full table: notes/secrets.md, "Where the 0xFF events are started / completed". Static scan of both discs (PAL = JP):
- **97 "The Stopped Clock Tower"** is started by a scene in the cut Stonetown (AREA16 s0, I0NFO2 `npcSet0ESceneStep1`) and completed by the Clock Tower
  gears object (X016 `brokenStoneGearsRepair`, using the Iron Wheel, item 0x3E).
- **98 "An Unbreakable Gear?"** is started at the same gears (X016 `brokenStoneGearsTalk`, "the stone gears are broken") and completed by the cut Iron Town
  smith when he gives the Iron Wheel (AREA17 s0, `npcSet0FSceneStep4`). That smith is the only code that gives item 0x3E.
- **76 (Black Water)** and **99 (Where's the Material!?)** are started only in the cut Iron Town (AREA17 s0) and completed nowhere. **61** is only read by the cut Stonetown dialogue.
- **The other 19** (6, 18, 45, 52, 59, 60, 67, 74, 83, 89, 94, 95, 96, 100, 138, 139, 140, 184, 186) have no code, room data or script that starts, completes or reads them.
  **89 (Green Fortune Teller)** is one of these. Its only context is the cut Wood village line "a fortune-teller lives in this hut, but he has a condition" (AREA11 s0).

## Jungle (AREA10): what is left of the Masakari / Funga content

AREA10 (overlay `X010`, text `MFNT100*`, JP `X10` + `AREA10/A003.GAM`) is the whole jungle: save names "Jungla" and
"Colina del árbol noble"; JP title list ジャングル / ジャングル奥地. There is no separate "Masakari Jungle" area.

- **Verified: the JP per-area title table of X10 (JP `X10.BIN` +0x44068) titles the cut events here:** 6 大きな水たまり？,
  59 族長のたのみごと？ (the chief's request), 60 コトバを吸いとろう！Ⅱ, 61 マサカリ族のヒミツ, 74 電気うなぎを捕まえろ！,
  184 おいら、悪い子？ ("Am I a bad kid?"), 186 マサカリの集落？, plus 138-140 (three sisters). So the Masakari chain was a jungle chain.
- **Verified: no code for 59/60/61/74/186 in PAL.** No `jal setEventStarted/Complete` (0x8005A8A8 / 0x8005A9A4) with those ids as
  immediate a0 in any X0nn, SYSTEM overlay or the exe; no `lui 0x800A` + load/store of `SCRIPT_FLAGS`+E in any language-0
  binary except the known I0NFO2 read of 61 (+0xAE00, +0xAF1C, cut AREA16 town). The other hits are in language-variant data.
- **Verified: the Funga chief's judgement scene survives as text only.** `MFNT100S` group 2 (base 211) dx 11-18 = PAL #222-#229
  (JP AREA10 A003 block 0 #188-#195). The lines: "You! Our friend! You hurt no one!", "Good boy! If I had a **Funga drum** I'd dance...
  (JP adds: that's a secret, forget it!)", "You may go!", then "You! You hurt my friends! ... I will read your fate; if you were good I'll
  forgive you. Fungaaa! Hongaaa!", "You were good, I forgive you", "You were bad! You'll pay! Fungaaa!!". dx 14 and 18 are
  the same speeches in Masakari gibberish (JP ◆★ glyphs, PAL `?????`), shown before you can understand the language. No static
  call reaches them. The 4 computed call sites of X010 resolve to group 4 (lostHatManTalk) and group 2 dx 22-36
  (swim teacher, button table `D_8012F3C8` = dx 27-32), and the 9 WSS scripts do not use them either. The fortune check probably went
  with event 184 "Am I a bad kid?" (inferred).
- Also in the bank, unreferenced: group 5 dx 15 ("you look tired, you need energy"),
  group 1 save prompts (data-driven, not cut). JP #171 ポテンシャルが　たりない ("not enough potential") is empty in PAL.
- **Sprites:** AREA10 SPR tables have 37 candidate frames, all Tomba's own swim/fight poses (code-driven, false positives).
  JP AREA10 SPR 0x33 has 3 frames not in PAL (f41, f42, f45: plain coloured bars, no art). No chief, eel or drum sprite.
- **Not checked:** a full dead-function list of X010 (the naive pointer scan is too noisy), placed-object subtypes per room.

## Folkin's rough notes — verification (non-Civilization claims)

The Civilization claims are checked in [CIVILIZATION.md](CIVILIZATION.md) ("Folkin's rough notes — verification").
These are the rest, checked only where it was cheap. **Unverified** means we did not check it.

| Claim | Verdict | Evidence |
|---|---|---|
| Green Fortune Teller completely cut | **partly** | Event 89 has its own icon (frame 227, "green fortune teller", see Event icons) but is not in `SUBSCREEN_EVENT_ORDER` and no code starts it. Retail has "The Red Fortune Teller" (CFNT #69) and "The Blue Fortune Teller" (#125). The unused item 90 is a letter "addressed to the three sisters". No sprite checked. |
| Funga Chief completely cut; punishment, drum and spear events | **partly** | Unused items: Funga Leather / Tree / Sap (119/122/123, JP "drum material"), Small Drum (137) and Chief's Spear (140). Their use-check lists are `subScreenCondFalse` (D0SPSUB `D_800FECE8`) and no code gives them. No chief sprite found. The chief's **judgement dialogue does exist** (AREA10 `MFNT100S` group 2 dx 11-18, unreferenced; see "Jungle (AREA10)" below). |
| Old Lumberjack Town Man completely cut | **confirmed** | His top-down frames are on `SYSTEM/A06100` (AREA11 subtype 0 extra frames 30..50, AREA11/CLUT02 row 36). They belong to the cut AREA11 s0 "Lumberjack Town" ([CIVILIZATION.md](CIVILIZATION.md)). |
| Puffer Fish, Whelk completely cut | unverified | |
| Masakari language learned by biting | **partly** | JP title of event 60 is コトバを吸いとろう！Ⅱ "Suck up the words! II" (JP AREA10 table), and the cut chief lines come in pairs: translated text plus a ◆★ gibberish copy (JP #191/#195, PAL `?????`). No code: 59-61 have no start/complete/read in X010 (see "Jungle (AREA10)"). |
| Navy Evil Pig Bag from the chief | unverified | |
| GoGoCar cacti and an extra track piece | **partly** | The cacti are confirmed (see "GoGoCart cacti / sign" above). The track piece was not checked. |
| Haunted Mansion metal sign model | unverified | |
| Doodle Rock textures/sprites in the Underground Forest | unverified | |
| Growing flower sprites, cursed and uncursed | **partly** | AREA02 SPR 0x3B frames 27-32 exist, but the VRAM they point at is empty on PAL (see Lost). |
| Iron door in the Iron Castle | **partly** | Event 100 "Iron Keyhole" has an icon (frame 209), is not in the menu order, and nothing starts it. No door sprite checked. |
| Weird plant in the Masakari Jungle VRAM | **not found** | AREA10 4 bpp pages of all 10 load lists rendered (tools/vram.py + find_unused_gfx.py): the unsampled regions are palms, reeds, flowers and wall textures used by backdrops/models, plus the known withdrawn B002 page. No isolated plant sprite. No palette, so a small plant could be missed. |
| "Thin" / "Fat" status text | **not found** | No such word in the PAL English/Spanish WFM text of AREA03/11/16/17/18 or CFNT999. The sub-screen graphics were not searched. |
| Mansion clock opening and gem | unverified | |
| Final Evil Pig door | unverified | |
| An older Pipe item drawn as a Valve | **confirmed** | Item 64 is "Pipe" (MFNT #67). Its icon (A00300 words 64..127, cell 64: x 72, y 72) is a round handwheel, i.e. a valve. Whether this is "older" art is not provable. Its inventory icon is grey because `ITEM_ICON_CLUTS[64]` = 1, the all-grey sub-screen row, on retail PAL and JP alike (an original quirk, not a port bug); the pickup in AREA08 is SPR_DATA[3] frame 2, a brown wooden wheel (area CLUT (304,482)). No row of the sub-screen block colours it as the tarnished brass it is, so the port draws it with a **custom palette, not an original one**: row 0 with the wheel's indices (13, 7, 12, 5, 14, light to shadow) turned into a rusted ochre ramp, rounded to 15 bits and uploaded to (304,511) (`port_valve_clut` in `port2/runtime/main.c`; `port/fixes.txt`, subScreenDrawItemIcon). The icon's frame stays exactly like every other item's: index 15 is kept white, and the frame corners / inner bevel, which share indices 13 / 12 / 5 with the wheel, are re-indexed in a corrected copy of the cell (uploaded over it) to free indices holding row 0's greys. |
| Pure 8 demo: caterpillars in the AREA00 VRAM; "Motocross Course" event starts normally | unverified | That disc is not in our set. PrePre 8's title strip does carry モトクロスコース (see the DIRT MOTOCROSS sign entry). |
| Pure 9 demo: a 10,000-year-old-man chest where retail has the 1000-year one | unverified | That disc is not in our set. |

Image (not committed): `item_icons_63_64_65_62_52_grey.png` (Flower Seeds, Pipe, Wine, Iron Wheel, Rain Essence).

## Zombie sweep (PAL)

Static sweep of MAIN0, X000..X019 (X007/X012/X015 do not exist) and the 8 SYSTEM overlays (D0SPSUB 1-3, G0OVER, I0NFO 1-3,
O0PTSUB0), scripted (throw-away scanner, not committed): every `type:func` symbol of `config/symbol_addrs_*.txt` is searched in the
raw binaries for `jal`/`j`, a raw 32-bit pointer (tables) and `lui`+`addiu/ori/lw..` pairs, then reachability is closed over the
call graph (a function only called by dead functions is dead too). Roots: any reference from data, every overlay's first
function, MAIN0 pointers into overlay space. A name counts only if **every** copy of it (the player library is linked into each
overlay) is dead. Limits: references built at run time (computed `jalr`, offsets) are invisible; "dead" = no static reference.

**Result: 575 distinct function names have no live reference; 263 are PsyQ library (libgpu/libcd/libspu/libgte...).** Of the other
312, 89 were already tagged "unreferenced / no caller" in `notes/names.md`; 223 were not. **Verdict: all of them are dead code, none
is cut content.** Nothing in this sweep is restorable as a feature.

| Class | Count | What (new = not tagged in names.md) | Confidence |
|---|---|---|---|
| Split-out `scriptOp*` cases + the older runner | 53 | MAIN0 `scriptOp*` (49: 0x80038238..0x8003AF94, `opNop`) and `runScript` 0x800391B0, `runScriptContext` 0x8003926C, `scriptRunOpcode` 0x80039150, `scriptLoadHeader` 0x80038B04: the WS-script (`"WS"` magic 0x530057, 64 script objects) opcode cases as separate functions plus an earlier copy of the runner. The live runner (the code after 0x80039338) calls `execCoreOpcode` / `execGameOpcode` directly, which inline the same cases. `scriptOpSetObjField6B.c` already says "a separate function here". | verified no refs; "copy" = read C |
| `step*` switch copies in overlays | ~60 | The compiler inlined an object's step switch into `updateX`; the stand-alone `stepX` copy stayed in the binary: `stepWindDiesEvent` (X003 0x80133938), `phoenixFavoriteCompleteScene`, `stepUnderwaterPigBagStart`, `stepBucket`, `stepPaintingSlotHintTalk`, `stepTownHallWorkerTalk`, `stepScaredGoldVolunteerTalk`, `stepSoakedPhoenixGuideTalk`, `stepTreasureChestGuardTalk`, `stepTenThousandYearElderTalk`, `juiceManJoinFightStep`, `overheadActor*Step` (I0NFO) ... Same dialogue ids as the live twin. | verified (C read for 12) |
| `check*Trigger` / `spawn*` one-shots | ~25 | `checkWindDiesEventTrigger` (X003, event 0xAA), `checkKeyHolesTrigger`, `checkBigKeyPaintingPanels`, `spawnBigKeyPiece`, `spawnMermaidForMathBeads`, `spawnHauntedPaintingObj` (X004), `spawnLostSeedKidDwarf`, `initPuppyRumorNpc`, `playPuppyBarkNearby`, `checkPuppyRumorNpcTalk` (X001), `spawnGoldenFlowerIfAffordable` (X002), `spawnBaccusVillagePigShotPieces`, `spawnFlowerForestPigShotPieces` (X014), `spawnCharlesNpcAlt` (X000, Charles, type 0x15, unless 0x8009CEBD >= 6): the same logic exists in the live `update*` that inlines them; the NPC/event they serve is in the game. | verified for the 6 read; the rest by name/shape: guess |
| Player library helpers (X000.. all areas) | ~35 | `playerTiltTowardSlope`, `playerAirControlTimed`, `setWeaponAimFromPad*`, `playerBiteEntry*`, `playerTryStartJump`, `playerGrabSpinEntry`... dead in every overlay (mostly inlined copies of the same player state machine). | verified no refs |
| MAIN0 engine remnants | ~60 | `dispatchAreaActor{Init,Update,Draw1,Draw2,Spawn}`, `dispatchAreaNpcInit`, `dispatchAreaCollisionHook{A,B}`, `dispatchAreaUnlayeredHandler`, `spawnUnlayeredObject*`, `onOverlap*`, `tryAttach*`, `modelDrawF3/G3`, `initMsgBoxA/B`, `showMessageBoxTimed`, `enqueueCdRead`, `lzDecompressToBuffer`, `getEventState`, `computeRotatedQuad`, `spawnItemLinked`, `stubCamera1..3`. Per-area dispatch wrappers and helpers whose callers were optimised away or never written. | verified no refs; role from names |
| Empty handlers | 5 | `updateL4Type08Object`, `updateL8Type07Object`, `stubNop*` (`i0nfo2`, X0nn), `stubFreeObj*`: empty bodies. | verified |
| One possible leftover (guess) | 1 | **`updateWaterCurrent` (X010 0x8012E9E0, 388 B)**: a jungle object that sets the "current" drift (`GVARS.driftH=0`, `driftY=-0x40`), sets a water level by section / purified areas (`-0xEA`, `-0x50`, `-0x3B8`) and snaps the swimming player (steps 0x3E, 2, 0xD) back to step 0x3D at that level. Its companions `a10SetPlatformY` and `a10ClampPlayerAboveY` are dead too. No caller or pointer in PAL. The live jungle water is handled in the player steps; this may be an earlier water-level object. Not matched to any placed object. **Checked (not restored):** dead in JP retail too (X10 `func_8012D004`/`8012CEEC`/`8012CF3C`, no pointer in X10.BIN) and the 1998-02-03 beta has no X10, so no build shows a call site. It is superseded: the live `playerUpdateWaterLevel` (in every player overlay) already does the same job per frame for AREA10 sections 1/5 (`w56=-0xE4`) and 3/7 (`-0x8C` purified / `-0x3C0`) and sets the same drift, `playerStepSwimUnderwater` (0x3E) surfaces at `+0x56`, and `playerHandleHazardContact` sends step 2 to swim 0x3D. The zombie's older levels (`-0xEA`, `-0x50`, `-0x3B8`) disagree with the live surface (52 px off in purified 3/7), so wiring it would snap Tombi to a second, invisible water line. Earlier water-level object, replaced by the player-side code; not wired into the port. | role: superseded |

**Not new but confirmed:** the three I0NFO2 `topdownA11/A16/A17ActorDialogueBank2` (cut Civilization towns, messages (2,4)/(2,5), (2,0)/(2,2), (2,5..10)) and `topdownA05/A08/A11/A16/A17/A19Scenery*` have no caller: the cut towns' own scene code (see Lost, docs/CIVILIZATION.md).

### 2. Object handler slots no room places
`tools/objects.py list --all-slots` (6533 records) against the MAIN0 handler tables. Unplaced slots that nothing spawns either (grep of
`allocObjectLayerN` + `->type =`): layer 4 type 8 (`updateL4Type08Object`, empty), layer 8 type 7 (empty), layer 2 type 0x30 (`updateL2Type30Object`: per-area dispatcher to the I0NFO2 town scenery of areas 5/8/11/16/17/19; those rooms are the cut towns), layer 5 types 9/0xA (`updateL5Type09Item`, `updateL5Type0AItem`: item objects; spawned through `ObjSpawn` with a variable type, so not provably unused), layer 7 types 5/10/13 and layer 2/4 types 0x19, 0x2D, 0x2E, 0x45, 0x4C, 0x41 (`overheadActorHandler`, `topdownItemSpotHandler`, `topdownTalkSpotHandler`, `tunnelActorHandler`, `tunnelTalkSpotHandler`, `tunnelStaticObjHandler`: the AREA02 top-down / tunnel sections of I0NFO / I0NFO3, placed by those overlays' own section objects). **No unplaced handler with real content.** Layer 8 has no placement record at all (its two live handlers are spawned by code).

### 3. Event ids
Nothing to add to "Events with no PAL title (0xFF)" and "Unused events": the 19 unreferenced and the 6 Civilization-linked ids stand as listed.

### 4. Dialogue
`tools/dialogue_refs.py --resolve` (computed-argument resolver, ~3 min) takes the 30 groups the literal scan flags and resolves the computed `(k, dx)` of every message call in the matched C (JP `*_slps01144.c` ports excluded): state-byte literals (`DAT_8009cebd/e/f`, ++ counters kept as "maybe"), data tables read from the binaries (`A00_TUTORIAL_JUMP_MESSAGE_IDS`, swimming teacher `D_8012F3C8` = 27..32 per button config, `D_80125E20`, `TOPDOWN_A16_ACTOR_DIALOGUE_IDS`, both talk-spot tables), the scene-sequence tables (`e[1]` / `c->arg`), the MAIN0 sign table `D_80079EBC` restricted to the subtypes the rooms really place (`objects.py`), the I0NFO/I0NFO2/I0NFO3 overlays (jal scan + C, I0NFO2 attributed to its town by function name) and `CALL/SPAWN` macros. Result for the 30: **7 groups are fully reachable** (MFNT040/080/090/100 g1, MFNT020 g5, MFNT160 g3, MFNT180 g1), 12 partly, **11 have no reachable line** (below). Undetermined: MFNT000 g7 (counter `DAT_8009cebf`; the boatman lines (7,3..5) are already listed above), the rest of MFNT000/001 g1 (`updateHoodedOldMan` sequence table not resolved), MFNT170 g5 dx 1/7/10/15/18 (ternaries on `D_8009CFA5`). Limit: WSS op 0x85 with variable args (AREA15: 0140 x13, 0200 x27, 0201 x3, 0210 x1, SCR0100 x1) is invisible, so the MFNT020 rows are "no static reference", not proof.

| New unused lines (PAL; ES / EN) | Evidence | Suggests |
|---|---|---|
| MFNT050 (1,5) "RATÓN DESAPARECIDO..." / "MISSING MOUSE...", (1,6) "¡ENHORABUENA! ..." / "CONGRATULATIONS!...", (1,7) "¡SE NECESITA AVENTURERO!..." / "ADVENTURER WANTED! The pump at the lake is broken..." | `D_80079EBC` has entries for (1,5) (subtypes 14, 19) and (1,7) (20) but no room places those subtypes; (1,6) is overridden by the button-config hint table | **restored (port, --cut-content)**: the board sign is spawned by code (`spawnNpcSet09`, subtypes 19/20) but out of reach; moved to the ground, (1,6) added before (1,7) (see Restored) |
| MFNT010 (1,4) "Bosque de setas ¿Guardar DATOS?" / "Mushroom Forest Save DATA?", (1,5) "Tobogán de hoja" / "Leaf Slider" | (1,4) is in no table entry; (1,5) only in unplaced entries; AREA01 s1 has no save sign | not restored: AREA01's five rooms each already have their own named save sign (Forest of 100 Flowers s0/s1, Wobbly Wharf, Watch Tower, Charity Square); Mushroom Forest / Leaf Slider name places with no room here (the Mushroom Forest beach of the boatman's cut lines; AREA07, which shares MFNT010/X001, is not on the disc) |
| MFNT060 (1,0) "La roca de la sirena" / "The Mermaid's Singing Rock Save DATA?"; MFNT020 (1,2) and MFNT190 (1,0..2) "Ciudad oculta / Ciudad de Enanos / Cabaña Enano Mayor" / "Hidden Village / Dwarf Village / Dwarf Elder's Hut" | AREA06 places one sign (subtype 36), AREA19 none; the (1,2) of AREA02 is placed nowhere | AREA06 rock **restored** (port, --cut-content; see Restored). Dwarf saves not restored: no room of AREA02 s0-s4 or AREA19 has any sign object to hook a spawn on (AREA02's only sign is s5's (1,3)), the elder's hut has no side-view room of its own, and AREA19 (the purified village, running AREA02's X002) does not load from the debug select in the port (fault in `queueLoadList`), so a placement could not be checked |
| MFNT020 (6,11) "¿Qué? ¿Querías preguntarme algo? ¿Un cable?..." / "What? Did you wanna ask me about something? A wire? ..not just an ordinary but an invincible wire?", (6,18) "¡Eh, hola! ¡Echa un vistazo! ¡El camino está despejado!" / "Hi, there! Take a look! The path is cleared!", (6,19) "¡Lo he acabado!! ¡Me merezco una buena palmada!" / "I finally finished it!! I deserve a big ol' pat on the back!" | `tunnelActorOpenDialogue` (I0NFO3) opens (6,0..10,12,15,17) only | digger-dwarf lines of a cut "tunnel finished" beat and a cut wire question (cf. the unbending cable chain) |
| MFNT020 (8,0..2) "¡El cable del ladrón está roto!" / "The thief's wire broke!", (8,2) "Suena como si algo se moviera..." / "It sounds like something moved..." | no call with k=8 in X002 / I0NFO / I0NFO3 | cut thief-wire scene (AREA02) |
| MFNT190 (3,7) "La cesta de comida... Has conocido a mi padre..." / "That basket of food... You must have met my father...", (3,8) "¿Te has comido mi almuerzo?" / "What? You ate my lunch?" | `topdownA19ActorOpenDialogue` (I0NFO2) uses (3,10..13) and `topdownA19ActorTalkStep` (3,4)/(3,6) | cut line pair of an AREA19 hidden-village resident |
| MFNT170 (4,0) "¿Me has encontrado otra vez, eh?..." / "Found me again, eh? Is there anything you can't find?!" | no caller | cut Iron Town NPC (Civilization) |

Placeholders, not content (`···???` / `..........???`): MFNT020 g4, g6 dx 13/14/16, MFNT110 g4 dx 8/9/12-15/17, MFNT170 g3 dx 4/6/7/9/10, MFNT030 (1,1), MFNT110/130/140 (1,0). MFNT000 (8,0) is the motocross sign already above; the save names of Stonestown / Iron Town (MFNT160/170 (1,0)) belong to the cut towns.

### 5. Files and load lists
Every file of `game/disc/fs` against the nine MAINn file tables: 2199 of 2287 are listed; 77 are not (the rest are the MAIN0..8 executables, SCES_013.31, SYSTEM.CNF):
- **`AREA19/X019..X819.BIN` and `SYSTEM/X019..X819.BIN`**: already listed (never loaded).
- **`I0NFO2.BIN` ... `I8NFO2.BIN` in AREA11, 16, 17, 18**: byte-identical to `SYSTEM/I?NFO2.BIN` (sha1 prefix checked for 0 and 8): copies of the town overlay that no table lists, since the towns load `SYSTEM/I?NFO2`. New but harmless: a leftover from when the town overlays lived in the area folders.
- **Unlisted CLUTs: `AREA11/CLUT01,04,05`, `AREA16/CLUT01-03`, `AREA17/CLUT01,07-11`, `AREA18/CLUT01` (13 files, 4457 B) and `AREA15/CLUT01-09` (9 files, 161 B)**: the 13 are byte-identical to each other (sha1 prefix `fa9c666a`), the 9 likewise (`2aca3f83`): filler / template palettes, no content. The used CLUTs of those areas (AREA11/02,03; AREA16/04-07; AREA17/02-06; AREA18/02,03) are the ones docs/CIVILIZATION.md uses.
- `ZZZ/DUMMY.DAT`: padding.
So **no unloaded graphic or sound resource** beyond what is known (X019, the Civilization sets).

### Summary
The PAL disc keeps no hidden zombie feature that the earlier inventory missed: the dead code is inlining/porting leftovers, and
the dead data is filler. The only item worth a second look is `updateWaterCurrent`; the dialogue sweep (4) is the place where a
resolver of computed `(k, dx)` could still find new lines.

## PrePre Vol. 8 code findings (2026-10-30)

Source: [PREPRE8.md](PREPRE8.md), "Functions vs PAL". All absent from PAL, JP retail and the 1998 beta.

| Find | Evidence | Status |
|---|---|---|
| `TO BE CONTINUED...` end-of-demo card | string 0x80010228, drawn at (20, 26) for 360 frames by the task at 0x80021050 when state byte 0x800DE4A3 == 2 | lost (code only; the trigger is not traced) |
| Scroll-triggered enemy wave spawner in area 1 room 0 | 0x80083D88 prints `enemy=%01d`; 8 scroll-X thresholds (0x800AF1A4), random delay table (0x800AF194); the other rooms do not spawn it | lost (enemy type not identified) |
| Camera waypoint debug view (`area_pos`, `pos_num`, `tar_x/y/z`) | 0x800268C0 | debug only |
| Test menus: SOUND OUTPUT (reverb types), MUSIC PLAY, MEMORY CARD, CDROM LOAD | dead code from 0x800854B4, no caller | debug only, not content |
| Title attract modes `PLAY DEMO`, `RANKING DEMO` | strings 0x80010174/0x80010180, called from the title state machine 0x8001ECD4 | lost (no demo data on the disc) |
