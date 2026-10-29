# Cut content: inventory and status

Spanish twin: [CONTENIDO_RECORTADO.md](CONTENIDO_RECORTADO.md).

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
| PrePre8 event titles 少年旅立ち / あれま大きな水たまり | TIM000 16x16 glyph strip (x 0..224, y 104..232). | Text only: a WFM3 event-title string (the encoder exists). Needs an event to carry it (see event 6). |
| PrePre8 elder's-house 2D interior | `TIM007` 8 bpp + CLUT00 row 20, `DATA002` (collision only). | A 2D-room path PAL does not have for that house; best as a gallery. Medium-hard. |
| PrePre8 grey bird enemy | TIM000 (~16 poses); candidate type 15, `func_8003D40C` in TOMBA.EXE. | No PAL AI matched; decoration or gallery only. Hard for gameplay. |
| PrePre8 15 sound samples | JOCHU 1, 5, 11, 12, 19-21, 23-25, 27; S_AREA01 0, 25, 28, 29. | Easy to bank; no evidence of where they played. |
| PrePre8 OPTION MODE / enemy viewer | 0x800854B4 (`OPTION MODE`, music/sound/CD/memory-card tests), 0x80083D88 (`enemy=%01d`), dead code in that build. | Port-only re-implementation; PAL has no such code. |
| GoGoCart cacti / sign (AREA06) | VRAM of `A001.GAM` (577,256), (516,256), (581,348); no SPR part nor model polygon; D004.000 has 60 models and `a06AddModelsBySection` uses all 60. | New sprite parts and placement: ours. |
| AREA17 B201 miner, AREA16 B501 big man (side view) | Texels with no SPR part. The B201 page renders with a real palette: AREA11/CLUT02 row 36 (same row in AREA17/CLUT03, AREA17/CLUT06, AREA18/CLUT02; the Male Lumberjack Town Resident's row) colours its side-view man with a backpack exactly like TCRF's colour sprite (all 14 colours in that row; silhouette close, not pixel-exact, TCRF's frame differs). Our identification. | SPR tables would have to be written by hand; animation unknown. |
| Small "?" bubble | SPR 0x34 f49; no script, no pointer. | Graphics only. |

## Lost

| Find | Why |
|---|---|
| Civilization towns (AREA11/16/17/18 top-down sections) | Owned by docs/CIVILIZATION.md (`--civ-test`): code in I0NFO2 is complete, but the sections' load lists are empty and their map / SPR / palette files are on no disc. 63 lines (class B*). |
| Growing golden flower frames (AREA02 SPR 0x3B f27-32) | The rectangles they point at are empty in PAL VRAM: the art is not on the disc. |
| JP-only text of class D tied to Civilization (MFNT110/170/180: raft maker, generator, coal, Dr. Gantetsu, new town) | Their scenes are the cut top-down towns. |
| PrePre8 dialogue script | The 10x10 font is there but no script text using it was found (still open, see docs/PREPRE8.md). |

## How to run

```
tombi2 --data data --cut-content [--early-title /path/to/prepre8_extract]
python3 -I tools/prepre8.py extract PREPRE8.bin /path/to/prepre8_extract   # once, from your own disc
```
