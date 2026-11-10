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
| Unused items (47; 21 with a JP description) | `scratchpad`-built list `unused_items_jp.txt`: e.g. Rain Essence (52), Torn Map halves (57/58), Rubber Gloves (59), Iron (61), Letter (90), Wood (91), Shovel (97), Electric Eel (108), Black Water (109), five Candies (110-114), Funga Leather/Tree/Sap (119/122/123), Small Drum (137), Chief's Spear (140), Coal (147). Name = D0SPSUB `ITEM_NAME_STRINGS[id]` (MFNT id+3 only for ids < 10), description = D0SPSUB block 0 id+4 (PAL English "···???"). | Giving them is one `addItemToInventory`, but nothing uses them; most belong to the cut Civilization / Masakari chains. Only a viewer would be honest. The JP descriptions could go into a text patch. |
| Unused events (68; 26 with JP info) | `unused_events_jp.txt`: e.g. 6 "A Big Puddle?" (PrePre8's strip has あれま大きな水たまり: the same idea, now confirmed in two builds), 18 Dwarves' Help, 45 AP Slot Machine, 52 "Now I Can't Cross...", 59-61 Masakari chain, 74 Catch the Electric Eel, 76 Black Water, 77 Civilization Machine, 83 Iron Car. No `setEventStarted/Complete` with those ids anywhere. | Each needs its own trigger and goal; none has code. Medium-hard each; only the titles/descriptions are cheap (text patch). |
| Event titles left 0xFF in PAL | JP per-area tables (`tools/event_titles.py`, notes/secrets.md) title 25 of them: 76 = PAL #90 "The Black Water", 83 = PAL #126 "The Rusty Steel Car" (the two unreferenced PAL strings), cut-town 0x61 動かない時計台 "The clock tower that won't move", 0x63 材料はどこだ！？ "Where's the material?!", 0x62 "Unbreakable gear?", plus 6, 18, 45, 52, 59-61, 67, 74, 89, 94-96, 98, 100, 138-140, 184, 186. Beta 980203 = PAL table. | Table patch: 76/83 reuse existing strings (cheap); others need new English strings. |
| Port: titles for the 0xFF events | Done in the port with `--cut-content`: 76->#90, 83->#126 and 22 titles translated from JP (list in notes/secrets.md; tools/port_event_titles.py). USA and beta 980203 have the PAL table. | Sub-screen event list not changed. |
| PrePre8 event titles 少年旅立ち / あれま大きな水たまり | TIM000 16x16 glyph strip (x 0..224, y 104..232). | Text only: a WFM3 event-title string (the encoder exists). Needs an event to carry it (see event 6). In TOMBA.EXE only the first 25 titles have a pointer (list slots 0..23); the rest (島のヒミツ, 大停電, 人魚の王を救え, 大団円...) are bare strings with no area/icon/AP/flag data (PREPRE8.md "Event data behind the titles"). |
| PrePre8 elder's-house 2D interior | `TIM007` 8 bpp + CLUT00 row 20, `DATA002` (collision only). | A 2D-room path PAL does not have for that house; best as a gallery. Medium-hard. |
| PrePre8 grey bird enemy | TIM000 (~16 poses); candidate type 15, `func_8003D40C` in TOMBA.EXE. | No PAL AI matched; decoration or gallery only. Hard for gameplay. |
| PrePre8 15 sound samples | JOCHU 1, 5, 11, 12, 19-21, 23-25, 27; S_AREA01 0, 25, 28, 29. | Easy to bank; no evidence of where they played. |
| PrePre8 OPTION MODE / enemy viewer | 0x800854B4 (`OPTION MODE`, music/sound/CD/memory-card tests), 0x80083D88 (`enemy=%01d`), dead code in that build. | Port-only re-implementation; PAL has no such code. |
| PrePre9 Tombi animations retail dropped (lift, pull, punches, somersault, carry-walk...) | PCPX-96093 `TOMBA/AREA01/A007.000` (379 frames, 106 not in retail) + 20 orphan scripts in `TOMBA/TOMBA.EXE` 0x80011A50..0x800125E8 with their tick counts (section "PrePre Vol. 9" below). | Gallery / port-only: the frames and SPR parts are on the demo disc (read at run time like the Pre8 title), the scripts give the timing, but no state code and no object they act on survive in Pre9 (placements, pages and code checked: nothing demo-only), so a mechanic cannot be rebuilt. |
| GoGoCart cacti / sign (AREA06) | VRAM of `A001.GAM` (577,256), (516,256), (581,348); no SPR part nor model polygon; D004.000 has 60 models and `a06AddModelsBySection` uses all 60. | New sprite parts and placement: ours. |
| AREA17 B201 page (backpack man, strap-shirt big man, bald old man, helmet walker; side view) | **Not a cut-content finding: withdrawn.** The user reports the backpack man in the final game. The same 192 x 256 page is loaded, byte-identical, by AREA10 s0 (B002, VRAM x 384) and s4 (B403, x 640), AREA16 s5/s6 (B501, B601), AREA17 s2/s5 (B201, B501) and AREA18 s1 (B101). Our static checks find no reference to its figures: no SPR table frame of any area list whose parts bound one of them, no WPP bank holding their pixels, no 16-byte part record pointing at them. These checks miss code-built primitives and other drawing paths, so "no static reference" here does not mean unused. The retail section, SPR id and address are not known yet. The B201 page renders with a real palette: AREA11/CLUT02 row 36 (same row in AREA17/CLUT03, AREA17/CLUT06, AREA18/CLUT02; the Male Lumberjack Town Resident's row) colours its side-view man with a backpack exactly like TCRF's colour sprite (all 14 colours in that row; silhouette close, not pixel-exact, TCRF's frame differs). Our identification. | Find the drawing path (dynamic capture in AREA10 s0/s4 or AREA17 s2) before calling any frame unused. |
| AREA19's own overlay `X019.BIN` (and `X119`..`X819`) | On the disc (`SYSTEM/` and `AREA19/`, 190,180 B, sha1 `94060c10...`) but **never loaded**: none of the nine file tables lists it (notes/cdfiles.md), and AREA19's load lists name `AREA19/X002.BIN`, byte-identical to AREA02's `X002.BIN` (sha1 `646aa8b5...`). JP does the same: AREA19 ships `X02.BIN` and `X19.BIN`, its lists name only `X02`. So the purified village runs AREA02's code; X019 is a separate, dead AREA19 build. | Study what X019 does differently from X002 (its area part, from 0x80115EA4); a port switch to run it would be ours. Unknown cost. |
| Small "?" bubble | SPR 0x34 f49; no script, no pointer. | Graphics only. |

## Lost

| Find | Why |
|---|---|
| **Top-down Charity Wings glide** (JP demo SLPM-80165 / PrePre 9): Tomba frames 77-79 with the arm raised + the feather item sprite | No script of the demo plays it; rebuilt as a reconstruction (section "Lost: the top-down Charity Wings glide" below, `tools/wings_glide.py`; in the port `--wings-glide JPDEMO_DIR --wings-pre9 PRE9_DIR` (its own switch), the Tomba frames from the JP demo and the feather from PrePre 9, docs/PORT.md): the spin order and timing are ours, the pixels and positions are the disc's. |
| Civilization towns (AREA11/16/17/18 top-down sections) | Owned by docs/CIVILIZATION.md (`--civ-test`): code in I0NFO2 is complete, but the sections' load lists are empty and their map / SPR / palette files are on no disc. 63 lines (class B*). |
| Growing golden flower frames (AREA02 SPR 0x3B f27-32) | The rectangles they point at are empty in PAL VRAM: the art is not on the disc. |
| Tunnel glyph sheet: Maya-style numerals 1-9 (dots and bars), shell/eye 0, "=" 10, ten boxed 24x24 symbols (sun, bird, moon, framed star, bow, crossed arrows, gear, ring of small rings, grid, mound) | `AREA02/B301.000` (GRPX 256x256 words at VRAM (768,0), AREA02 section 3 = top-down tunnels), 4 bpp at (832,72) 240x72 (page 13, u 0..239, v 72..143), CLUT `AREA02/CLUT04` row 22 col 7 = (240,502), word 0x7D8F. Same pixels in US and PAL (identical file) and JP (different file, same block); not in any `SYSTEM/` reload copy, not in the PrePre8 data, beta disc not checked (not local). No static reference found: the CLUT word occurs nowhere in AREA02 code/data, and no part of the tunnel SPR banks (D305) samples v 72..143. **Checked in the port: never drawn.** `--dump-draws` over 13 headless runs from the debug select into AREA02 section 3 (walks covering player x 982..1677, y -80..-729 of the tunnel network, the exit into AREA01 section 3, and `D_800A60A3` poked to 1/3/4 to force the alternative backdrop sets of `tunnelSelectBackdropPieces`): 5508 distinct textured primitives in section 3, none sample page 13 at v 72..143 (page-13 use there is only v 0..56 and 146..158) and none use CLUT (240,502). `tunnelSelectBackdropPieces` (I0NFO3) only picks backdrop piece pointers (table entries 0..20 by camera x, or entry 0x5C/0x60 for `D_800A60A3`), so it cannot reach the block by itself. **Full sweep with `--noclip`** (port debug flag, docs/PORT.md): five headless serpentine walks over the whole section (player x 90..1750, the clamp of the section, i.e. past the x<0x99 exits; y -157..-1567, past the map's -0x540 and the lowest exit at -0x4FE; rows 118 px apart, camera x 199..1400+ so all 21 backdrop strips, camera y down from its top clamp -52), one per backdrop value `D_800A60A3` = 0..4 forced inside `tunnelSelectBackdropPieces` (a `--poke-every` of 0x800A60A3 does not stick: the hook read 0 every frame, so the earlier 1/3/4 runs most likely drew the default set): 2318 distinct textured primitives with the default set (0 and 4), 202 with 1/2 (entry 0x5C), 212 with 3 (entry 0x60); page 13 again only at v 0..55 and 146..157, no CLUT (240,502). With `--noclip` the trigger tiles and exits are off, so tunnel event scenes are not part of this sweep (the earlier 13 runs covered the reachable ones). **Backdrop data decoded:** the RAM block `tunnelSelectBackdropPieces` reads (0x1DC40 bytes, header of 24 piece offsets 0x64..0x1DC40) is a list of textured polys (CLUT word, tpage word, colour+GP0 code 0x24/0x2C, then x,y,z,u,v per vertex): 882 polys, tpages 0x0C/0x0D/0x0E/0x4D, 16 CLUTs (0x784E..0x7DCF), none 0x7D8F; page 13 only at v 0..31 and 144..159. **Static re-check:** 0x7D8F as a 16-bit word is absent from MAIN0 and every overlay except one false hit (X010 0x37608 is `j 0x8011F63C`); the tpage-13 4 bpp + UV pattern (hi 0x000D/2D/4D/6D, v 72..143, u<240) and GP0 0xE1 page-13 words hit only MIPS instructions in MAIN0/X001/X006; the AREA02 file hits (B001/B101/B200 GRPX pixels, D004/D304 compressed streams) are not references. No reference found. Unused art of a cut symbol/number puzzle; layout, code and the Japanese hint text (a cut "graffiti floor" contraption with dot-and-bar base-20 numerals): see "Tunnel glyph sheet (AREA02/B301): layout, code and text" below. |
| JP-only text of class D tied to Civilization (MFNT110/170/180: raft maker, generator, coal, Dr. Gantetsu, new town) | Their scenes are the cut top-down towns. |
| PrePre8 dialogue script | Found 2026-10-30 in TOMBA.EXE (u16 glyph indices, `tools/prepre8_text.py`): area-0 tutorial/Yan/100-year-old-man script and a forest/dwarf script (lost dwarf child, Baron named by a dwarf nurse, trapped thief). Forest kanji page missing. See docs/PREPRE8.md. |

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
  JP AREA10 SPR 0x33 has 3 frames not in PAL (f41, f42, f45: plain coloured bars, no art). No separate drum sprite found. **Correction: the Funga chief does have a sprite.** AREA10 load lists 1 and 5 carry SPR table 0x37 (`SPR_DATA[7]`, from `SYS/LD0SYS.BIN`); its frames 31 and 47-58 (53 is a mis-cut atlas block) are the chief: big flame-like headdress, cape, sitting / standing-up / talking / walking poses, 56x64 px. The pixels live on the SYS reload page `SYSTEM/A05700.000` (SYS load list 57, loaded together with `X010.BIN`; the Funga villagers of frames 59+ share it). The extractor shows them with CLUT 0x780A (brown/yellow); the real palette is set at run time (a community rip shows a blue body, red cape and orange robe). No X010 code reads `SPR_DATA[7]` (no `lw 0x2E4`, no AREA10 handler in `tools/animobj.py` uses table 0x37) and no X010 animation script runs only those frames, so the art is loaded but never drawn (PAL; JP/beta not checked). **Correction: the eel was too broad.** An unreferenced 3-frame
  script survives: JP X10 cell 0x8012DEB8 → 0x8012E0E0 = frames 33, 34, 35 of SPR slot 3, 8 ticks each, the last
  one held (0x8008), no loop; PAL X010 has the same 24 bytes at 0x80131F1C (cell 0x80131CE8). SiergiejW decoded the
  frames as a 32x16 eel with its own palette (texture in `AREA10/B003.GAM`); we checked the script and its lack of
  pointers in both builds, not the pixels (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **Items vs the eel (JP SLPS, same on PAL where checked):** item 108 (Electric Eel) picks world descriptor 0 (table
  0x80077BE4 → pointers 0x80077CAC; descriptor 0 = the generic berry frame), like 59 Rubber Gloves, 109 Black Water and
  119 Funga Leather; none of the 97 descriptors points at the eel cell. Its menu condition is list 22 (PAL D0SPSUB
  `D_800FECE8[108]` = 22 → 0x800FF3E8: AREA18 s2, player H 128..192), the same as Black Water: a place to use it, not
  a spawn point. Funga Drum 125 (descriptor 94) and Small Drum 137 (61) share the anim cell 0x80011E8C (one sprite)
  and differ only in the box (`08 10 08 10` vs `0C 0C 0C 0C`); the drum's menu condition is list 41. Wood 91,
  Funga Tree 122, Funga Sap 123 and Chief's Spear 140 have their own descriptors (75, 73, 72, 74) (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **JP item-use lines name two placeholders:** in the JP item-use block (AREA10 A003 #n = item n) #133 is
  １ＵＰをつかった and #134 ゲージＭＡＸ　＋１をつかった (the two "Chick" slots: 1UP and gauge max +1), #131 地下迷宮の地図
  (underground maze map), #147 石炭 (coal); #77-#86 (Math Bead 11.., Number Balls) say エラーです. All have menu list 0
  (never usable); only 77 has a world descriptor (89). Text only, no effect code was found (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **The shock reaction is PAL-only:** PAL X010 `playerStepHurtKnockback` (0x800FD358) sets animation 0x50 for
  hurt reason 3 (+0xD1); the JP routine (X10 0x800FCE48, 1,492 bytes) never reads +0xD1 and always loads animation 0x39
  (`li a2, 0x39` at 0x800FD01C) (SiergiejW, TombaEdit docs/jp-retail-unused-research.md).
- **Not checked:** a full dead-function list of X010 (the naive pointer scan is too noisy), placed-object subtypes per room.

## Folkin's rough notes — verification (non-Civilization claims)

The Civilization claims are checked in [CIVILIZATION.md](CIVILIZATION.md) ("Folkin's rough notes — verification").
These are the rest, checked only where it was cheap. **Unverified** means we did not check it.

| Claim | Verdict | Evidence |
|---|---|---|
| Green Fortune Teller completely cut | **partly** | Event 89 has its own icon (frame 227, "green fortune teller", see Event icons) but is not in `SUBSCREEN_EVENT_ORDER` and no code starts it. Retail has "The Red Fortune Teller" (CFNT #69) and "The Blue Fortune Teller" (#125). The unused item 90 is a letter "addressed to the three sisters". No sprite checked. |
| Funga Chief completely cut; punishment, drum and spear events | **partly** | Unused items: Funga Leather / Tree / Sap (119/122/123, JP "drum material"), Small Drum (137) and Chief's Spear (140). Their use-check lists are `subScreenCondFalse` (D0SPSUB `D_800FECE8`) and no code gives them. The chief's **sprite survives** unused (AREA10 SPR 0x37 frames 31, 47-58, page `SYSTEM/A05700.000`; see "Jungle (AREA10)"). The chief's **judgement dialogue does exist** (AREA10 `MFNT100S` group 2 dx 11-18, unreferenced; see "Jungle (AREA10)" below). |
| Old Lumberjack Town Man completely cut | **confirmed** | His top-down frames are on `SYSTEM/A06100` (AREA11 subtype 0 extra frames 30..50, AREA11/CLUT02 row 36). They belong to the cut AREA11 s0 "Lumberjack Town" ([CIVILIZATION.md](CIVILIZATION.md)). |
| Puffer Fish, Whelk completely cut | unverified | |
| Masakari language learned by biting | **partly** | JP title of event 60 is コトバを吸いとろう！Ⅱ "Suck up the words! II" (JP AREA10 table), and the cut chief lines come in pairs: translated text plus a ◆★ gibberish copy (JP #191/#195, PAL `?????`). No code: 59-61 have no start/complete/read in X010 (see "Jungle (AREA10)"). |
| Navy Evil Pig Bag from the chief | unverified | |
| GoGoCar cacti and an extra track piece | **partly** | The cacti are confirmed (see "GoGoCart cacti / sign" above). The track piece was not checked. |
| Haunted Mansion metal sign model | unverified | |
| Doodle Rock textures/sprites in the Underground Forest | **confirmed (corrected)** | The graffiti rocks exist: five distinct dot-and-bar plaques (2, 5, 18, 139, 59) on room texture pages, two of them placed in retail as 3D decor models (Dwarf Forest AREA01 s1/s3 = 2, Haunted Mansion AREA04 s0/s1 = 5). See "Graffiti rocks (らくがき岩): the plaques exist". |
| Growing flower sprites, cursed and uncursed | **partly** | AREA02 SPR 0x3B frames 27-32 exist, but the VRAM they point at is empty on PAL (see Lost). |
| Iron door in the Iron Castle | **partly** | Event 100 "Iron Keyhole" has an icon (frame 209), is not in the menu order, and nothing starts it. No door sprite checked. |
| Weird plant in the Masakari Jungle VRAM | **not found** | AREA10 4 bpp pages of all 10 load lists rendered (tools/vram.py + find_unused_gfx.py): the unsampled regions are palms, reeds, flowers and wall textures used by backdrops/models, plus the known withdrawn B002 page. No isolated plant sprite. No palette, so a small plant could be missed. |
| "Thin" / "Fat" status text | **not found** | No such word in the PAL English/Spanish WFM text of AREA03/11/16/17/18 or CFNT999. The sub-screen graphics were not searched. |
| Mansion clock opening and gem | unverified | |
| Final Evil Pig door | unverified | |
| An older Pipe item drawn as a Valve | **confirmed** | Item 64 is "Pipe" (MFNT #67). Its icon (A00300 words 64..127, cell 64: x 72, y 72) is a round handwheel, i.e. a valve. Whether this is "older" art is not provable. Its inventory icon is grey because `ITEM_ICON_CLUTS[64]` = 1, the all-grey sub-screen row, on retail PAL and JP alike (an original quirk, not a port bug); the pickup in AREA08 is SPR_DATA[3] frame 2, a brown wooden wheel (area CLUT (304,482)). No row of the sub-screen block colours it as the tarnished brass it is, so the port draws it with a **custom palette, not an original one**: row 0 with the wheel's indices (13, 7, 12, 5, 14, light to shadow) turned into a rusted ochre ramp, rounded to 15 bits and uploaded to (304,511) (`port_valve_clut` in `port2/runtime/main.c`; `port/fixes.txt`, subScreenDrawItemIcon). The icon's frame stays exactly like every other item's: index 15 is kept white, and the frame corners / inner bevel, which share indices 13 / 12 / 5 with the wheel, are re-indexed in a corrected copy of the cell (uploaded over it) to free indices holding row 0's greys. |
| Pure 8 demo: caterpillars in the AREA00 VRAM; "Motocross Course" event starts normally | caterpillars **not found**; Motocross title **confirmed** in strip | Per Folkin, "Pure 8" is the same build as our PrePre 8 (`docs/PREPRE8.md`). All 11 AREA00 TIM pages rendered (`tools/prepre8.py unpack` + `pages`): TIM000 holds only Tomba, pig and grey bird/dragon sprites, the rest are backdrops/textures/fonts. No caterpillar sprite (no palette search, so a tiny one could hide). The title strip does carry モトクロスコース. Gameplay start of the event not tested. |
| Pure 9 demo: a 10,000-year-old-man chest where retail has the 1000-year one | unverified | That disc is not in our set. |

Event titles of PrePre 8 / "Pure 8" (list by Folkin, details in `docs/PREPRE8.md`): 29 titles absent from retail. Slot guesses, unverified: Five Chests (Forgotten Things / Key) -> Lost and Found (area 0 title 魔女の忘れもの fits), Great Thief in the Underground Prison -> before The Great Escape, Tumbling Down / Again -> Where did I come from? / Lava Caves, Where the Light Falls -> event 32 (trumpets). Update 2026-10-30: the full table (167 titles, incl. chests, thief, tumbling, light and many later-area titles such as 島のヒミツ1-5, 大停電, 人魚の王を救え, 「大団円1-3」) is decoded from TOMBA.EXE, see docs/PREPRE8.md. Confirmed as glyphs in the TIM000 strip: Young Boy Sets Off, Large Puddle, Blow Away the Fog, Meeting the 100 Year Old Man, Elder; the chests, thief, tumbling and light titles are not in the strip.

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
`allocObjectLayerN` + `->type =`): layer 4 type 8 (`updateL4Type08Object`, empty), layer 8 type 7 (empty), layer 2 type 0x30 (`updateL2Type30Object`: per-area dispatcher to the I0NFO2 town scenery of areas 5/8/11/16/17/19; those rooms are the cut towns), layer 5 types 9/0xA (`updateL5Type09Item`, `updateL5Type0AItem`: item objects; spawned through `ObjSpawn` with a variable type, so not provably unused), layer 7 types 5/10/13 and layer 2/4 types 0x19, 0x2D, 0x2E, 0x45, 0x4C, 0x41 (`overheadActorHandler`, `topdownItemSpotHandler`, `topdownTalkSpotHandler`, `tunnelActorHandler`, `tunnelTalkSpotHandler`, `tunnelStaticObjHandler`: the AREA02 top-down / tunnel sections of I0NFO / I0NFO3; correction 2026-11-09: in retail only the JP disc places type 0x41, see "Placement records across PAL / USA / JP"). **No unplaced handler with real content.** Layer 8 has no placement record at all (its two live handlers are spawned by code).

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
| Village exits to rooms 4 (DATA003, thatched houses/watch tower/river) and 5 (DATA004, house with pig sign) | entry records `D_800A273C` (room 0 -> room 4, room 1 -> room 5, and back) + position triggers in the player controller 0x80028B50 (states 4/5/9/10); DATA004's only sign reads 「ふるいけ」 | lost in later builds as these models; reachable by the code (not emulated). ふるいけ is a retail place, so DATA004 is an early ふるいけ, not a lost location; DATA003 has no name, no NPC, nothing linking it to a civilization |

Not found (2026-10-30, do not re-search): no dialogue line or room record starts events 20 (5つの箱の忘れもの) or 22
(地下ロウの大ドロボウ), or any id outside 1-4, 6, 9-11, 13; DATA003/DATA004 spawn no object type absent from the village;
no debug room select in TOMBA.EXE; area 1 has no load list. Details: PREPRE8.md "Event ids from data, and the rooms".

## Code hunt, 2 November 2026 (Flower Tower, versions, unused animation scripts)

Static analysis only: throw-away scanners and Ghidra-free disassembly (capstone), not committed. **[V]** verified in the
binaries, **[I]** inferred. Nothing here is restored in the port.

### The Flower Tower area (community question: "is there more code for it?")
- **The inside of the Flower Tower is AREA13 section 0** [V]. PAL X001 `updateAreaScroll` (0x80116990), section 4
  (Charity Square, AREA01 s4) case: while `GAME.playerAP > 999999` **and** `GCD.bce48 != 0` (= event 164 "The Flower Tower"
  started, flag byte 0x8009CDA4 + 0xA4), Tombi standing at x 0xB5C..0xB7B is walked into the depth (x 0xB6C, depth >= 0x8D1);
  after 60 frames the code sets `nextArea 0x0D / section 0 / spawn 0`. That is the only write of `nextArea = 13` in the game
  (every other `nextArea` store in src/ was checked). The AP check is the constant 999999 (`lui 0xF` + `0x423F`), so you
  need **1,000,000 AP**. JP X01 has the same constant (0x80116238) [V]. The keeper's line MFNT010 (2,126) "If you wanna get in
  the tower, you're gonna need lots of AP!!" is the in-game hint.
- **The room holds a single object** [V] (`tools/objects.py list --area 13`): an item object, layer 5 type 4, subtype 132 at
  (160, -470). Reward handler 1 = `rewardItem`, so this is item 0x84 **Sacred Fish** (MFNT #135). No NPC, no enemy, no event.
  X013 `updateAreaScroll` (0x80115FB4) in s0 only waits for Tombi to walk into the exit at x 0xA0 and drops him back into
  AREA01 s4, spawn point 4.
- **No more tower code exists** [V]. X013's area part is 27 functions, 10 KB in all (scroll, carry-player step, backdrop sprites,
  Mighty Fish Food, the s1 scene sequence and the ending stats). Our dead-function scan of X013 finds only the
  helper `func_801174C4`, which every overlay carries, unreferenced. `MFNT130` has 208 lines: group 0 (system/item text) plus one
  `..........???` placeholder (#207), with no dialogue at all. Section 1 is the ending (event 0 "Grandpa's Bracelet",
  `S_END`). The `AREA13_PIGISLAND` name in the sources comes from the psx_tomba constants. In the data the area is
  "Flower Tower top + ending".
- Restorable: nothing is missing. The beta's title for the event, "The Million Tower" (docs/BETA_980203.md), refers to this
  million-AP gate [I].

### Builds: code present in one build only
- **USA (SCUS-94236) vs PAL** [V]: `tools/xmatch.py` + `tools/fn_vs_pal.py` over the USA exe and all 17 `SYSTEM/Xnn.BIN`
  (base 0x800E8028, the same as PAL). Every gap piece of >= 300 B with similarity < 40 is a data block (the shared handler
  tables at 0x800E8028 / 0x80114A38 and the tail data). **No USA-only code in the area overlays.** The INFO/DSPSUB/GOVER/OPTSUB
  overlays were not compared (xmatch has no PAL index for them).
- **JP retail draws the ending stats with the debug font** [V]: JP `X13.BIN` `updateSceneSequence` (0x801162DC) calls
  `sprintf` + `drawDebugText` with `TIME  %3d:%2d`, `AP    %8ld`, `EVENT %3d / %3d` (file +0x2B718). PAL/USA X013 have no
  format string; they draw the same numbers with glyph sprites (`drawGameStats` 0x801179B4 and helpers). The JP exe also prints
  `TRY AGAIN...`, ` GAME OVER ` and `NOW LOADING` with `drawDebugText` (0x8001A1D8, 0x8001C0C4, 0x8001B864). PAL draws those
  from graphics. PAL MAIN0 keeps only the build stamp (`98/8/26 14:10`, 0x80019448) and the area select. Follow-up: "The debug font" below.
- **JP retail per-area code** [V for the method, I for the verdict]: `fn_vs_pal.py` over JP X01..X19 (area part from 0x80115178).
  The low-similarity pieces we read (X04 0x80117938, X03 0x8011D630, X10 0x8011C81C, X09 0x8011F65C, X01 0x80123014, X06 0x80117AB4)
  are rewritten player / collision / model-block code with the same callees. No JP-only feature apart from the X13 stats above.
- **Correction to docs/BETA_980203.md ("Glyph scene object", beta 0x8002F084):** the function is in PAL. It is MAIN0
  0x8002CF40..0x8002DC50 (3344 B), slot 0x80079C14 of the object pool table (right after `dispatchMsgBoxHandler`). It calls
  `loadMessageGlyph`, `freeMessageGlyphs` x4, `GetClut`, `openMenuScreen`, `SfxPlay2` x3 and the matrix calls [V]. USA has it at
  0x8002FA24 (table slot 0x8007C718). It was missed because `notes/functions_main0.csv` and the symbol table skip it
  (`clearTalkPose` ends at 0x8002CF40), so it has no name. It is not cut code.

### Dead functions and handlers (second pass)
Our call graph over MAIN0, every X0nn, I0NFO 1-3, D0SPSUB 1-3, G0OVER and O0PTSUB0 (jal/j, data words, lui/addiu pairs, MAIN0 words into
overlay space; closure over dead callers) reproduces the zombie sweep above. Of the area-part functions of >= 150 B it lists, 66 are not
mentioned in this file. All but one are step-switch copies with a live twin (e.g. X005 `mansionKeyHintTalk` = the
same (2,0x33) line as the live `stepBacoTownNpcTalk`; X016 `stepJuiceSellerTalk` and `stepBrokenStoneGearsTalk`; X004
`stepBigKeyPainting`; X001 `stepRaceEvent`) or helpers / D0SPSUB3 copies of D0SPSUB code. The exception:
- **X001 `segmentedCrawlerWander` (0x801334DC, 1352 B)** [V]: an older, simpler copy of the green caterpillar's movement routine.
  It is unreferenced in PAL, USA and JP. Details, renders and a port experiment are in "Unused patrol routes" below.

### Unreferenced animation scripts
Scan of the area-part data of every X0nn for 8-byte animation entries (tools/anim.py format) ending in 0x8000 or a backward
0x4000 jump, kept only when no data word and no lui/addiu points into them. All of these sit **between referenced scripts of the
same object's tables** (owners from `tools/anim.py list --owner all`). **Rendered (follow-up):** the port was run headless into the
owner's room from the debug select, the VRAM was dumped raw (a temporary port-only hook, not committed) and the SPR parts were cut from
that VRAM with the owner's tpage offset (`w1e`) and CLUT (throw-away renderer, not committed). The decoder shows that every one of these
scripts ends in 0x8000 (play once and hold the last frame); none loops. Images: see "Images" at the end of this section.

| Overlay | Address(es) | Frames | What it shows (render) |
|---|---|---|---|
| X009 / X011 | 0x8012F144, 0x8012F164, 0x8012F17C / 0x8011C6E4, 0x8011C704, 0x8011C71C | 43 44 45 44 / 46 47 48 / 49 50 (50 held for 255 ticks) | **Jellyfish (AREA09 s0) and flying octopus (AREA11 s1), one shared SPR table** (kind 0x3A, base 4 resp. 9, CLUT 30930 resp. 31058). It is one sequence: the body curls its tentacles under and squashes flat twice (43-45), springs up with the tentacles thrown down (46-48), then stands upright on curled tentacles (49-50, held). Frame 42 (tentacles curling in) is in no script. It looks like a landing / "stand on the ground" pose set, not an attack [I]. The AREA11 room does not even load the page area of frames 37-50 (the octopus's sideways swim 37-41, which its live table uses, is garbage there), so the octopus render takes the pixels from AREA09 and the CLUT from AREA11 |
| X003 | 0x80139130 | 12-15 | swaying hop plant table: a grass / reed tuft swaying (decor) |
| X003 | 0x8013A074, 0x8013A0A4, 0x8013A0E4, 0x8013A114; 0x8013A194, 0x8013A1A4, 0x8013A1B4 | 29-32-30, 33-35, 38-41-39, 42-44 (ping-pong); 24-25, 26-27, 46-47 | wind-blown villager table (AREA03 s0, SPR 0x32): talk / gesture loops of the **brown-hat dwarf** (29-35) and the **sombrero dwarf** (38-44), and three two-frame moves of the **bundled-up old dwarf with the red scarf** (24-27, 46-47). Talk animations nobody selects |
| X004 (also AREA12) | 0x80134E7C, 0x80134E9C | 16-19 forward / 19-16 backward | a **stone slab turning** (rotating wall / door panel opening and closing) in the biting-plant / smoke table area |
| X004 | 0x801354E4 (single frame 185, missed by the first scan), 0x801354EC, 0x8013551C, 0x8013554C | 185; 195 187 188 196 188 187; 186-189-187; 190-192-185 | AREA04 s4 SPR 0x32 base 1: the **sombrero dwarf** again (three talk / nod gestures). **Follow-up:** the four scripts are a copy of the town-hall-worker / painting-hint NPC set (table 0x8013540C = 0x80135430 frame 173, 0x80135438, 0x8013546C, 0x801354A0, read through `D_801311B8[subtype]` and `D_801311D0[subtype]`) with every frame number +12 and the loop jumps replaced by stops. The table that would select this second character does not exist |
| X008 | 0x80118504, 0x8011853C | 26-29-26, 30-33 | not rendered. **Owner found (follow-up): the Pipe item.** The block 0x801184DC.. is a run of item scripts drawn from `SPR_DATA[3]` (frames 2-4, 24-35, 13-15, 0-5). 0x801184C8 is not unreferenced: MAIN0 item descriptor 81 (0x8007AF44, item 64 = Pipe through `DAT_8007b084[64]`) points at it, and `initItemObject` takes `anims[0]` (descriptor byte 7 = 0) = 0x801184DC, the wooden wheel (frame 2) seen in AREA08 s1. The other words of 0x801184C8 are not pointers, so nothing selects the rest of the block |
| X010 | 0x80131DFC, 0x80131E14 | 5-7, 8-10 | AREA10 s0 decor (base 12): jungle bush frames and waterfall / foam strips |
| X014 | 0x8012A090; 19 scripts in 0x8012A58C..0x8012A780 | 12-20; 9-33, 47-62 | not rendered: in the boss room (s7) and s2 the page areas of those frames are not resident at the bases the room uses (only frames 0-8, 18-21, 26-29 are) |
| X002 = X019 | 0x8011F2BC, 0x8011F2CC (X019 0x80116178/88) | 6-7, 8-9 | not rendered: not resident in AREA02 s1 at base 4 or 10 |

Restorable only as decoration or extra talk poses: no code selects them. The jellyfish/octopus sequence is the most complete one.

#### Runtime check (port `--log-anim`), 2 November 2026
Question: could a computed selection (table base + offset) reach these scripts, which a static search cannot see? **No: none of them
is ever selected, either at run time or by any code path.** [V]

- **Tool (port-only, off by default):** `--log-anim [FILE]` (port2/runtime/animlog.c, hooks in port/fixes.txt). It reads TObj +0x24 after every
  main-pool object update, inside `AnimLoadDuration` (the call after each `o->anim = script`), `AnimJump`, `AnimAdvance`,
  `AnimAdvanceWithBox` (all four copies) and `tickAnimationFixedRate`, and records the frame each visible object shows in `drawObject`.
  Any pointer inside the windows below, or a jellyfish/octopus script showing frames 42-50, prints `[anim] HIT`. The summary at exit lists
  every script address and frame per overlay, room, category and type. `--log-anim-force CAT:TYPE,...` also puts every object of those types
  through b04 1..3 x step 0..11 (state 0), one combination every 110 frames, so each branch of its update switch runs whether or not the
  player triggers it. Windows: the table above, with X004 widened to 0x801354E4..0x8013558C and the 19 X014 scripts as four ranges.
- **Runs (headless, debug area select):** every section of AREA02, 03, 04, 08, 09, 10, 11 (s1, s2) and 14 (62 rooms x 3600 frames, generic
  walk / jump / attack / run script), plus 19 owner rooms twice (5800 frames each): once with Tombi teleported (`--poke` of his position)
  next to each owner type in turn, attacking, jumping and pressing Circle; once with `--log-anim-force` on the owner types (8451 forced state
  changes). 447,200 frames in all. **0 HIT lines; frames 42-50 never drawn.**
- **What the owners did meanwhile:** the jellyfish (cat 2 type 0x21) used 16 of the 19 scripts of its table (float, swim, grab, hit, hurt fall,
  child follow, defeat); the flying octopus (type 0x2A) used one, because `flyingOctopusMove` only ever selects `D_8011C420[0]` (its table is
  the jellyfish table copied, so its 43-50 scripts are doubly dead); the AREA04 s17/s18 town hall worker (type 0x32) ran 0x80135430 and the
  0x801354A0 loop, whose jump at 0x801354DC goes back to 0x801354A0, the script right before the window; the AREA08 Pipe item showed only
  0x801184DC; hop plants, wind-blown villagers, biting plants, smoke, and the AREA10/14/02 decor objects ran their tables normally.

| Script(s) | Verdict | Evidence |
|---|---|---|
| X009 0x8012F144/164/17C (jellyfish 43-50) | never hit | AREA09 all sections; s0 natural + forced (2 objects); frames 42-50 never drawn |
| X011 0x8011C6E4/704/71C (octopus copy) | never hit | AREA11 s1/s2; code selects only entry 0 of its table |
| X003 0x80139130 (hop plant 12-15) | never hit | AREA03 all sections; types 2:24, 3:28, 3:29, 2:1D natural + forced in s0/s1/s4/s5 |
| X003 0x8013A074..0x8013A1B4 (villager talk poses) | never hit | types 2:32, 2:37 natural (Circle pressed next to them) + forced in s0/s1/s4/s5 |
| X004 0x80134E7C/9C (stone slab) | never hit | AREA04 all 20 sections; biting plant 2:07, smoke 3:2A/2B, collectible 2:29 forced in s0/s9 |
| X004 0x801354E4..0x8013554C (+12 NPC set) | never hit | town hall worker 2:32 in s17/s18 natural + forced; 2:35/3:3C in s15 |
| X008 0x80118504/3C (Pipe item block) | never hit | AREA08 all sections; the Pipe item shows 0x801184DC only (descriptor uses `anims[0]`) |
| X010 0x80131DFC/E14 (decor) | never hit | AREA10 all 9 sections; decor 3:3F, 4:35, biting plant 2:07 forced in s0/s4 |
| X014 0x8012A090, 0x8012A58C..0x8012A780 | never hit | AREA14 all 8 sections; decor 2:4B, 3:50/51/52/23/61, 4:3A/3C forced in s2/s3/s7 |
| X002 0x8011F2BC/CC | never hit | AREA02 all sections; decor 2:18, 3:2C forced in s1/s2 |
| X019 0x80116178/88 (copy of X002's) | not run | the debug select poke for area 19 lands in AREA02; static only |

**Computed selection, re-checked statically** (capstone over the nine owner overlays and MAIN0, throw-away scripts):
- **Every store to TObj +0x24** was traced back to its source register: a word loaded from a table (`lw`) or a lui/addiu constant. The only
  arithmetic sources are the animation advance routines (`AnimAdvance`, `AnimAdvanceWithBox`, `tickAnimationFixedRate`, the X003/X004
  `komaPigAnimAdvance` copies: +8 to the next entry, or the pointer word after a 0x4000 entry), which follow a script to its end and never cross
  a 0x8000 terminator. (X009 0x8011A3E0 stores to +0x24 of a camera struct, not an object.) Counts of `sw rX, 0x24(obj)`: X002 59 load / 73 const,
  X003 221 / 73 (+3 advance), X004 217 / 73 (+3), X008 35 / 73, X009 125 / 78, X010 133 / 78, X011 73 / 73, X014 76 / 74, X019 35 / 73.
- **`AnimJump(o, n)` is the only base + index arithmetic on a script** (`o->anim += n` entries, MAIN0 0x8001FE94). In the area code of the nine
  overlays every call passes n = 0, directly or through the NPC wrappers `npcSetAnim` / `sevenFriendsSetAnim` / `sevenFriendsMemberSetAnim`
  (all callers pass k = 0); the X004 / X010 wrappers (`mermaidSetAnim`, `townHallWorkerSetAnim`, `scaredGoldVolunteerSetAnim`,
  `paintingSlotHintNpcSetAnim`, `tenThousandYearElderSetAnim`) have no callers. The non-zero constants (2, 3, 4, 0xF, `lbu` from a table)
  are in the shared player code and move Tombi's MAIN0 scripts.
- **Table indexing** (`table[n]`, `D_x[subtype].anims[n]`, the item descriptors' `anims[b0c]`) loads a word, and no word holds an address in
  any window: not in the owner overlay, not in MAIN0, and not in any file on the disc read in the same overlay (scan of all 2287 files; the
  only raw hits are inside STR movies and GAM image data).

**Images** (scratchpad of the session, not in the repo): per script a GIF `<prefix>_<address>.gif`, per owner a sheet
`<prefix>_sheet.png` (one row per script, frame numbers on top) and an atlas `<prefix>_spr.png` (all SPR frames of the table):
prefixes `x009_jellyfish`, `x011_octopus` (plus `x009_jellyfish_sequence.gif`, `x011_octopus_sequence.gif`: the three scripts
played in a row), `x003_monkey`, `x003_hopplant`, `x004_bitingplant`, `x004_sombrero`, `x010_decor`, `x014_bossdecor`.

### Checked, nothing new
- **Read-only save variables:** of every GCD byte (0x8009CD00..0x8009D100) that some code reads and none stores by absolute address, every
  one is an element of the event array (0x8009CDA4 + event) or of the item count array (0x8009D0A4 + item, `subScreenDrawItemNames`),
  which are written by index. No forgotten flag.
- Music: `tools/music_refs.py` already shows every sound set and SOUND file reached (docs/CIVILIZATION.md).

### Not checked
SFX ids never played (the id comes from data and animation tables as well as from immediates; needs a census of
`playSFX`/`SfxPlay2` arguments plus `D_800780C4`); the pixels of the X002/X008/X014 unreferenced frames (see above);
the beta's `X01..X19`, `INFO*` and `DSPSUB*` (not on the local disc); the USA/JP system overlays vs PAL.

## The debug font (drawDebugText): follow-up, 2 November 2026

Question: JP draws the ending stats and TRY AGAIN / GAME OVER / NOW LOADING as debug text, so is there a font we can write
anything with? **Yes, in every build, PAL included, and the port can already call it.** [V unless marked]

- **Not Psy-Q's FntPrint.** No `FntLoad` / `FntOpen` / `FntPrint` / `KanjiFntPrint` is identified in the PAL, USA, JP or beta symbol
  maps. The game has its own printer, `drawDebugText(x, y, colour, string)` (PAL MAIN0 0x80020F14, 572 B, matched C in
  `src/drawDebugText.c`; USA 0x80023AA4, JP 0x80021E5C, beta 0x800233A4, same code). Each character becomes one 8x8 `SPRT_8`
  (code 0x75) with u = (c & 15) * 8, v = (c >> 4) * 8 on texture page 0x15 (4 bpp, VRAM x 320, y 256) and CLUT (368, 496 + colour). A `DR_MODE`
  follows every call. It goes into the debug primitive buffer, 0x780 bytes per frame buffer. That is about 115-120 characters per frame in
  total; further characters are dropped (seen in the port: a fifth line did not appear).
- **The glyphs are part of the system texture page** `SYSTEM/A00000.GAM` (loaded at (320, 256) at boot and resident in every
  frame we dumped: title, area select, rooms of AREA00/01; the menus draw other cells of the same page). The font area (128 x 128 px,
  the left half of the page) is **byte-identical in PAL, USA and JP**. Cells: 0x10-0x1F = hex digits `0-9A-F`; 0x20-0x7E = printable
  ASCII **with lower case**, except that 0x40 is **`©`** instead of `@` (hence JP's `@1997 WHOOPEE CAMP Co.,Ltd.` shows a ©);
  0x80-0x89 = dim digits 0-9. Everything from 0x90 up is HUD / effect art of the same page, not font. No accents, no kana. The glyphs
  are shaded (gradient plus shadow, about 10 colours). The 4 CLUT rows give white, blue, orange and green.
- **Who calls it.** PAL / USA: `bootStartupTask` states 2-3 (build stamp `98/8/26` `14:10` in PAL, `98/3/22` in USA; unreachable, state 1
  jumps to 4) and the debug area select `displayDebugScreen` (live with the debug flag `0x1F8001B4`, the port's
  `--title-debug` / sweep pokes). JP adds, all live in retail: the title's `@1997 WHOOPEE CAMP Co.,Ltd.` and `PUSH [START] BUTTON`,
  `TRY AGAIN...`, `NOW LOADING`, ` GAME OVER ` at (96, 96), the X13 ending stats at (32, 64/88/112) (`TIME` from two X13 counters,
  `AP` = playerAP, `EVENT` = the number of event bytes equal to 0xFF over 256 ids / a total), and a collision readout
  `flag= %01d`, `hit pos x/y= %04d`, `no hit`, `pos x/y= %04d`, `dir = %01d` (JP 0x8002FB98..0x8002FCF4, at x 8). PAL / USA replaced
  all of the JP uses with graphics and removed the readout strings.
- **Port.** `drawDebugText` is native code in the port and the page is resident, so port code can call it from any per-frame game hook
  (tested with a throw-away hook in `drawHudIfShown`: the JP stats and a test text drew in-game in all 4 colours; the text goes into the
  game's own ordering table, behind the message box, over the scene). Uses: debug / research overlays (positions, flags), an
  in-picture FPS counter (the current `--show-fps` overlay is drawn at window resolution with its own 3x5 digits, which is sharper at high
  internal resolution), new short texts. Limits: 8x8 fixed width, ASCII plus ©, 4 colours, about 115-120 characters per frame shared by all callers,
  and screen-space only (no scaling).
- Images: font page in the 4 CLUTs, a code chart, the JP screens rebuilt from data (font + JP coordinates + JP format strings with
  sample values), and port screenshots (PAL area select; the JP stats and a test text drawn in-game by the port). Paths in the
  session report.

## Unused patrol routes

Sweep (2 November 2026): every X0nn function list (notes CSV plus the `// FUNC` headers) against jal/j targets, raw pointers and lui/addiu
pairs of the overlay and of MAIN0 (the MAIN0 handler tables point into overlay space). The dead functions were then searched for
movement code (position += velocity, hard-coded position limits, `COS_TABLE` steering, waypoint tables) and for subtype cases that no
placement reaches. The I0NFO2 NPC sets (top-down patrol boxes) were dumped with `tools/topdown.py npcs`. MAIN0 / X000 / I0NFO dead
functions are those of the zombie sweep above (none moves an object along a route). [V] unless marked.

### X001 segmentedCrawlerWander: the caterpillar's first route
- **The owner is the green caterpillar of Wobbly Wharf** (render: a head with an orange underside and a dark mouth, plus 3 or 5
  round green segments with a red spike and a pink spot). It is not the spider. Layer 2 type 0x13, handler `updateSegmentedCrawler` 0x80134C00.
  No APD record places it. It is spawned by the unlayered object type 7 `updateSegmentedCrawlerSpawner` (0x8013C028, MAIN0 unlayered
  table D_8007BF34 index 7). That object comes from the per-section unlayered list `PTR_8007bee4[area][section]` = {2, 1, 7}, found only in
  **AREA01 section 2 (Wobbly Wharf)**. The same list pointer is also reached as AREA07 s2 (the AREA01 copy that is not on the disc) and as AREA00 index 8
  (past AREA00's 6 sections).
- Spawner: the first crawler after 600 frames, then one every 480 / 600 / 360 / 440 frames (random), at most 2 alive. It starts at one of four
  points D_8013CA10 (h, y) = (525, -510), (710, -580), (940, -630), (1130, -570), at the player's depth. Subtype = frame parity & 1:
  **0 = head + 5 segments, 1 = head + 3**. Subtypes 2 and 3 exist in the update switch but are never spawned.
- In this room the horizontal axis is the TObj `b` field (Tombi enters at 215). The limits 0x118 (280) and 0x551 (1361) span the rope-bridge
  stretch of the room [I], and the crawler walks in the tree canopy above it (y about -400 .. -630; Tombi on the bridge is at -229 .. -259).
  Off screen its y is clamped to at most -0x190 (`updateSegmentedCrawler`).
- **Live route (`segmentedCrawlerMove`, 0x80132B64)**: down for 110 frames (JP: 60), turn to face right, run right until h >= 0x551 *or*
  `segmentedCrawlerCheckWall` hits, turn back (through below or above, chosen by y < -0x140), run left until h < 0x118 or a wall,
  turn, and **loop forever** on two lanes.
- **`segmentedCrawlerWander` (0x801334DC, 1352 B)**, no reference in X001, MAIN0 or any table: the same init (but 60 frames down, as
  JP's Move), **no wall check, no lane choice, no loop**: right until h >= 0x551, U-turn to angle 0x80 (state 7; state 9 is a
  no-op), left until h < 0x118, turn up and round to face right (state 13), and then **state 14 has no case: the head stops for good**,
  facing right at h 280, and the body stays behind it. So it is a one-way patrol, a first draft of Move (Move = Wander + wall checks + lanes + loop).
  It **shares** the animation tables with Move and `segmentedCrawlerGrabbed` (D_8013F034: 12 directional head scripts, frames 57/58,
  152/153, 150/151, 148/149 and 124-131; D_8013F064; the body uses D_8013F1BC, frames 0-15). The earlier note "own animation table" and
  "heads up" were wrong (angle 0xC0 is +y, down).
- Which room fits: only Wobbly Wharf. Its limits are the same constants as the live routine, so the patrol was written for this room.
  The four duplicate subtype cases of `updateSegmentedCrawler` (0/1, 2, 3 each call Move) are where one subtype once called Wander [I].
- Builds: **unreferenced in all three retail builds**. JP X01 has it at 0x80131DB8 (masked twin, overlay base 0x800E9AF8) and its
  `updateSegmentedCrawler` (one call, 0x80133618) only calls Move. USA X01 has it at 0x80132814 (the USA X01 file maps to base 0x800E7388) with no reference.
  The beta's X01 is not on the local disc. **PrePre Vol. 8 (follow-up):** its single TOMBA.EXE does have the caterpillar, as class 2 type
  0x13 (the PAL type id), with its own movers per subtype; neither Wander nor Move is in it (masked-shingle similarity <= 17 %), and the object is
  never placed or spawned in the demo (docs/PREPRE8.md "The segmented caterpillar").
- **Port: `--crawler-patrol`** (port/fixes.txt, off by default) makes the head run Wander instead of Move. Recorded: both
  caterpillars come down, run right to 1361, turn, run left and park one above the other at h 280, facing right, for good. Retail keeps
  looping. GIF side by side (retail | patrol); the patrol stays higher than the retail lanes. For the recording the camera was lifted by holding Tombi's y (throw-away hook), because the crawlers walk above the top of the normal view.

### Other unused movement
| Function | Owner / room | Route | Builds | Verdict |
|---|---|---|---|---|
| X009 `flowerTearsSeekerWalkOff` 0x80128B4C (248 B) | flower tears seeker NPC (MFNT090S "came for Flower tears to revive the Charity fountain") | walks left at 1 px a frame until off screen, then hides (b04 2). It is the live `flowerTearsSeekerPace` (paces h 0x708..0x780, with gravity and tile collision) without the turn and the collision | unreferenced in PAL, JP (X09 0x80127CD4) and USA (0x80127E94) | a leave-the-scene walk nobody calls; the live NPC is freed off screen once event 0x28 is done |
| X002 `sevenFriendsMemberFlyUp` 0x80119088 (240 B) | a member of the Seven Friends (AREA02) | waits for its timer, then rises with velocity -0x10 per frame down to -0x600, switches to anim 1 at -0x80 and is freed off screen | unreferenced in PAL, JP (X02 0x801182E4) and USA (0x801183D8) | a fly-away exit; the live scene steps do their own exit [I] |
| X001 `komaPigCheckWall` 0x8012BC84 | koma pig, AREA01 | (wall probe, not a route) | the X003 / X004 copies are live | AREA01's koma pigs never test walls |

Live, for the record: X010 `spearPigWanderB` (subtype 1 of the spear pig, placed in AREA10 s0), X009 `flowerTearsSeekerPace`.

### Top-down patrol boxes (I0NFO2 NPC sets)
Record fields (docs/CIVILIZATION.md): behaviour 1 vertical, 2 horizontal, 3 / 4 diagonal, box = limits. **The only patrol that runs
in retail is AREA19 s2 (set 11) actor 4** (vertical, y 268..304). All the others are in the cut Civilization towns and never run:
set 0D (AREA11 s0) actors 3 (diagonal), 4, 5, 8 (horizontal), 7, 9 (vertical); set 0E (AREA16 s0, boxes relative to the actor) 2, 7 (diagonal),
4 (vertical), 5 (diagonal); set 0F (AREA17 s0) 6, 9, 12 (horizontal), 8, 13, 14, 15 (diagonal); sets 0C (AREA08) and 10 (AREA18) have no patrols.
Diagram: all four sets on one sheet.

### Not checked
Waypoint tables read by live code but with entries no path reaches (e.g. AREA09 `updateWaypointRide`). Unreachable states inside
live movement routines other than the crawler's.

## Tunnel glyph sheet (AREA02/B301): layout, code and text, 2 November 2026

Follow-up to the B301.000 row in "Lost" (community questions: are these separate floor tiles, is any placement code
left, does the Japanese text mention them). Images are not committed (game pixels).

### Layout
- **Grid:** the block is exactly 10 x 3 cells of 24 x 24 px, no gutters (u 0..239, v 72..143). Row 0 (v 72..95):
  Maya numerals 1, 2, 3, 4 (dots), 5 (bar), 6..9 (bar + dots), 0 (shell). Row 1 (v 96..119): **ten** symbols, not
  eleven: sun, bird, crescent moon, framed star, bow and arrow, crossed arrows, gear/cog, ten small rings around a bar,
  grid, mound with an arc. Row 2 (v 120..143): u 0..71 plain stone, u 72..95 "=" (two bars = Maya 10), u 96..239
  index 0 (empty).
- **One stone tile under every glyph:** all 21 glyph cells (numerals, symbols, "=") are the same 24 x 24 stone tile
  with the glyph painted on it (median 97 % of the non-glyph pixels identical to the per-pixel mode; 62 % for the
  framed star, whose inner box covers more). The tile wraps only roughly: mean luminance step at the wrap seams 17.4
  (left/right) and 17.2 (top/bottom) against 11.9/15.7 inside the tile and 21.5 between random columns.
- **The frames are a strip, not plaques:** in row 1 the 1-px frame (CLUT index 4) is the top and bottom row of each
  cell and the **right** column (u = 24k + 23); only the first cell also has a left column (u 0). Cut out alone,
  cells 1..9 are framed on three sides, so the symbols were drawn as one continuous row of boxes (or meant to be
  placed side by side in that order). The numerals have no frame at all.
- **Row 2 u 0..71 is one 72 x 24 piece of stone,** not three tiles: the seams between its cells (7.8, 9.2) are no
  bigger than its interior steps (8.4), while its wrap seams (18.1 / 19.3) are as large as random (17.3).
- **Not a font strip:** cells are 24 x 24, glyphs centred in them (numeral dots/bars in rows 7..17), no 8x8 or 16x16
  sub-grid.
- **Not cut from the tunnel floor:** the grey (CLUT 0x7D8F, neutral greys 80..184) matches no 24 x 24 window of
  pages 12..15 of B301 (best 18 % identical indices, chance level); the tunnel floors are the light cobble of page 14
  (CLUT 0x784F, greys 168..224), sand and dirt. 0x7D8F is row 502, the one free row between the page-13 backdrop
  CLUTs 0x7D4F (501) and 0x7DCF (503).
- **The tunnels have no floor tiles:** the I0NFO3 backdrop is free polygons (see below) with ~1 texel per world
  unit on the floors (median world/texel 1.11 for the cobble, 1.06 for the dirt), and the only grid is the 8 x 8
  collision map. A 24 px glyph cell would cover 3 x 3 collision cells.

### Code (I0NFO3, AREA02 section 3): no placement path
- **Backdrop format (corrects the "882 polys" above):** the kind 0x50 block (D306 + 968) is 24 piece offsets, and
  each piece is groups of `u32 count` + polys in the order FT3, FT4, GT3, GT4 (textured: CLUT, tpage, 1 or 3/4
  colour words, then x, y, z, u, v per vertex), then the untextured groups. 2,506 textured polys; page 13 only at
  v 0..69 (pieces 0x5C/0x60, the alternative sets, CLUT 0x7C0F) and v 146..157 (CLUT 0x7DCF, the small cairn
  stones); no CLUT 0x7D8F; nothing samples v 72..143. Rendering all pieces gives the whole tunnel network at 1:1
  (sand, cobble, a cracked stone dais, a ring of seven small pyramid stones on the sand at about x 385..445,
  y -992..-940).
- **No tile-to-UV formula exists:** `tunnelCollideMapTile` / `tunnelCheckTileTrigger` read `TUNNEL_MAP_TILES`
  (D303, 240 x 168 cells of 8 px, copied to 0x80124000) as collision only. Its values are 0..9 (shape) plus bit 7
  (trigger) and nothing else (no unused values that could select graphics).
- **Trigger cells:** 17 clusters. Five sit on the five exits of `tunnelSectionStep`; two are the backdrop-switch
  zones: on a trigger cell `tunnelPlayerMainStep` stores 1 + the index of the `TUNNEL_TRIGGER_RECTS` rect that holds
  the player into `PLAYER.b6b` = `D_800A60A3`, the value `tunnelSelectBackdropPieces` reads (rect 0 -> set 1,
  rect 1 = (0,0,0,0) placeholder, rect 2 -> set 3). The other ten (5 x 5 cells at x 408..447 y -1184..-1145 and
  592..631 y -1144..-1105, irregular ones at 864..903 / -1048..-1017 and 992..1023 / -968..-929, six single cells)
  have no reader: nothing in I0NFO3 uses the player's trigger bit outside the rects, and the one MAIN0 reader of
  `PLAYER.b68` (script opcode 0x93) calls the side-view overlay address 0x800ECB40, which is inside
  `tunnelItemSpotHandler` in I0NFO3. None of them is 3 x 3 cells.
- **No puzzle logic:** all 55 I0NFO3 functions are decompiled and named; none counts steps, compares a sequence or
  keeps digit counters. Placement (kind 0x83): 20 type-7 records (paired gates) and 2 type-0x3F; the actor spawn
  table: 11 type-0x45 actors; talk spots all show (1, 0) (the save prompt). Object tpage offsets are 0 (player),
  2..4 (actors), 0xF (item spots, i.e. the time chests), 0x15 (talk spots); no sprite table of the section samples
  page 13 v 72..143.
- **Versions:** US I0NFO3 has the same rects and dialogue choices; JP INFO3 too (rects at file 0x67A4). The JP
  collision map (`AREA02/D304.000` on the JP disc) differs from PAL/US in 136 wall cells, never in a trigger bit.
  The JP B301 differs from PAL only at y 144..246 (words 64..175); the glyph block is identical. 0x7D8F occurs in no
  JP/US INFO3 or X02. The beta 980203 and PrePre 8 builds have no AREA02 data.

### The text: a cut "graffiti" puzzle with Maya numerals
- **The digger's hint, skipped by the code.** AREA02 tunnel dialogue group 6 (`tunnelActorOpenDialogue`, the digger
  of case 8): the code shows dx 0xF, starts event 161 "Underground Treasure", then shows dx 0x11; **dx 0x10 is never
  shown** (same constants in PAL, US and JP). PAL #439 = dx 0x10 is `···???`; the JP line (AREA02 A003 #433) is:
  言い伝えだと・・・宝物庫へ行くには仕掛けをクリアしないとダメらしい・・・ でも オレはさぁ 宝物庫を掘り当ててやるんだ！
  ん・・・仕掛けを解く方法？ それは仕掛けをつくったヤツが残した岩のらくがきを 調べてみろよ たしか・・・地上に６コあるハズだから
  見つければ やくに立つかもな・・・
  "According to the legend... to get to the treasure vault you have to clear a contraption... But me, I'm going to
  dig my way into the vault! Hm... how to solve the contraption? Go and study the rock graffiti left by whoever built
  it. I think there are six of them up on the surface, so if you find them they might help..."
  In retail, event 161 is completed by opening time chest subtype 7 in the tunnels (300,000 AP,
  `tunnelItemSpotCheckStep`); no contraption exists.
- **The six rocks use dots and bars in base 20.** AREA16 group 2 dx 3/4 (PAL #211/#212, JP AREA16 A003 #188/#189),
  with no reference in X016:
  キミ、いろんなところを旅してるんだって？ だったら、へんな点とか線がきざんである らくがき岩・・・見たことないかなぁ・・・
  おれ、ずーっとまえに木人の加工場で 見たことがあるんだけど、どうも気になってなぁ・・・
  "You travel all over the place, right? Then have you ever seen a graffiti rock with strange dots and lines carved
  in it...? I saw one ages ago at the wood people's workshop and it's been bugging me..."
  ウワサじゃ 全部で６つのらくがき岩があって、そこに書かれてあるナゾをとけば たくさんの お宝が 手に入るらしいんだけど
  ・・・なんかむずかしそうだなぁ ・・・が１・・・ーが５の・・・ ２０進数とかなんとか・・・ おれにはとても わかんないよ・・・
  "Rumour has it there are six graffiti rocks in all, and if you solve the riddle written on them you get lots of
  treasure... but it sounds hard... '·' is 1... '–' is 5... base twenty or something... I just don't get it..."
  (the "·" and "ー" are the same glyphs as the ellipsis dots and the long-vowel bar, so the line reads as a mumble).
  PAL kept the lines but lost the notation: "1 is...by 5...to the 20th power..." (all languages say "power"). Dot = 1,
  bar = 5 and base 20 are the Maya numerals of the sheet. **Correction:** the rocks are not cut; their plaques are
  on the room texture pages and two are placed in retail (see "Graffiti rocks (らくがき岩): the plaques exist" below).
  What is cut is everything around them: event 67 解読？らくがき岩 "Decipher? The Graffiti Rock" has a JP title (and a
  JP hint, DSPSUB#1 #67 らくがき岩の　もじを　かいどくする "decipher the writing of the graffiti rocks") but no start,
  completion or flag read anywhere (notes/secrets.md).
- **PrePre 8 names the floor.** The PrePre 8 event-title table (`tools/prepre8_text.py`, string at 0x800AB038,
  glyphs checked on `SYS008.000`) has らくがき床のナゾ "The Riddle of the Graffiti Floor", followed by かくされた入口
  "The Hidden Entrance"; it also has 解読?らくがき岩 and 森の地下には何がある? "What is under the forest?" (retail
  "What's Under the Forest?" is the tunnels' own event). No retail build has a "graffiti floor" title.
- **Other hits, unrelated:** 数字玉１..１０ / "Math Bead 1-10" are retail items of the 10,000 Year Old Man's quest
  (JP item text: "a ball with N written on it that the 10,000 Year Old Man asked for"), collected under water in the
  Trick Village; they are numbered 1..10 like the numerals but nothing ties them to the sheet. The other tunnel
  `···???` lines are the mine cart (PAL #436/#437 = JP #430/#431 "it needs water... burning water", "see, it ran on
  water!"; #457 = JP #448 "the cart started moving!") and the gate scene (PAL #406, #414..#418 = JP #400, #408..#412).
  No dialogue in JP retail, the JP demo or PrePre 8 uses 太陽, 鳥, 弓, 矢, 石板, パネル, 記号, 暗号 or パスワード for this.

**Reading:** the sheet is most likely the tile set of the cut "graffiti floor" contraption that guarded the
Underground Maze treasure vault: a row of ten framed symbols and numeral plates in dot-and-bar, base-20 notation,
whose code was to be found on six graffiti rocks on the surface. The floor and any code for it are gone; the rocks
partly survive (five plaque textures, two of them placed as plain decoration, below); the retail game kept the
treasure (event 161, a time chest) and two hint lines, one of them unused.

## Graffiti rocks (らくがき岩): the plaques exist, 2 November 2026

Correction to the section above, which said the six rocks were never built. A community rip shows five stone
plaques with dot-and-bar numerals; they are in the game data. Images are not committed (game pixels).

### Where they are
All five use one and the same 42 x 51 px plaque tile (frame, stone grain, 4 bpp indices identical outside the marks;
only the marks differ), so the whole disc was searched for that index pattern (raw nibble scan of every file,
GAM files unpacked, PAL, US and JP): five distinct plaques, every hit listed.

| Number | File (PAL) | VRAM (4 bpp page, u, v) | Room | Drawn in retail |
|---|---|---|---|---|
| 2 | `AREA01/A003.GAM` (area page, at word (768,0)) x 102, y 64 | page 12 (768,0), u 102..143, v 64..114 | AREA01 (Forest of 100 Flowers / Dwarf Forest), every section | **yes**: object model #22 of the s0/s1 model table (#10 in s3), CLUT 0x7849 (144,481), placed by a layer-4 type 0x13 record (`updateStaticDecorModel`) at (4110, -399, z 48) in s1 and s3, just left of the exit at the right end of s1. Same in AREA07 (copy of AREA01's lists). |
| 5 | `AREA04/A003.GAM` (area page, word (832,0)) x 654, y 192 | page 15 (960,0), u 142..183, v 192..242 | AREA04 (Haunted Mansion), every section | **yes**: model #22 of the s0..s3 tables, CLUT 0x7ACF (240,491), type 0x13 decor record at (1080, -675, z 940) in s0 and s1: a stone block with the plaque on the terrace right of the mansion's face door. Same in AREA12 (copy of AREA04). |
| 18 | `AREA03/A002.GAM` (area page, word (0,256)) x 398, y 24 | page 17 (64,256), u 142..183, v 24..74 | AREA03 (Stormy Mountain / Lava Caves / Phoenix's Nest), every section | no |
| 139 | `AREA10/B004, B104, B203, B302, B404, B504, B603, B702` x 266, y 176 | page 17 (64,256), u 10..51, v 176..226 | AREA10 (Jungle), sections 0..7 | no |
| 59 | `AREA11/B100.GAM` (room page, word (512,0)) x 10, y 136 | page 8 (512,0), u 10..51, v 136..186 | AREA11 section 1 | no |
| (2) | `AREA09/B002.GAM` x 870, y 64 | page 15 (960,0), u 102..143, v 64..114 | AREA09 (Mushroom Forest) section 0 | no; pixel-identical to the AREA01 "2" |

- **US and JP** have the same plaques at the same offsets (US AREA04/A004, AREA09/B004; JP likewise). **JP only:** the
  "59" plaque is also on six JP AREA10 room pages (B003, B102, B203, B403, B502, B603, at y 136), i.e. the Japanese
  jungle carried the 59 rock's texture as well; PAL/US keep it only in AREA11.
- **How "drawn" was decided:** every textured polygon of every room model (kind 0x50, also the WHM-first containers of
  AREA01/09) and of every object-model table (kind 0x51..0x5F pieces) of AREA01/03/04/07/09/10/11/12 was tested against
  the plaque UV boxes: only the two models above sample a plaque. For the other three, no room polygon, no object model
  and no SPR part (only whole-page sub-screen maps) samples the plaque. The layer-4 spawner (`spawnObjectFromRecord`)
  takes the model from record byte 14 (`be`) when byte 12 has bit 0x10: be = 22 (AREA04) and 22 / 10 (AREA01).
- **Checked in the port:** with the fly mode of the port debug menu (`--debug-menu`, L1+R1+Select), AREA04 s0 at player (978, -716) shows the "5" rock right of
  Tombi on the mansion terrace (`--dump-draws`: CLUT (240,491), page (960,0), uv 129..197 x 186..248, frames 1809+);
  AREA01 s1 at (4252, -521) shows the small "2" plaque on the stone wall before the exit (CLUT (144,481), page (768,0)).
- **No code uses them:** `updateStaticDecorModel` (X001 = X004, 0x800EE18C) only sets the draw flags and draws; no
  talk, flag, event or check. Event 67 has no start/complete/read (above). The Stonetown villager says he saw one
  "at the wood people's workshop" (木人の加工場, AREA16 #188): that fits the "59" texture on the AREA11 page. No other
  JP line names a rock, a number, an order or a symbol.

### Reading the numbers (Maya base 20)
Dot = 1, bar = 5, levels read bottom (x1) to top (x20); within a level the dots sit above the bars.
- AREA01: two dots, one level: **2**.
- AREA04: one bar: **5**.
- AREA03: three dots over three bars, evenly spaced, one level: 3 + 15 = **18**.
- AREA10: upper level a dot over a bar (6), clear gap, lower level four dots over three bars (19): 6 x 20 + 19 = **139**.
- AREA11: upper level two dots (2), gap, lower level 19: 2 x 20 + 19 = **59**.
(Read naively as one level the last two would be 6+19 = 25 and 2+19 = 21; the gap and the villager's "base 20" make the
two-level reading the intended one.) The sixth rock is not on any disc (PAL, US, JP); the AREA09 copy of "2" is the only
extra plaque.

**Against the B301 sheet:** the sheet has single numeral tiles 0 (shell), 1..9 and "=" (two bars, 10), 24 x 24 each,
plus ten framed symbols. The rocks need digits up to 19 (18, and 19 in two of them), which the sheet does not have, and
their marks are drawn larger and on a different, framed 42 x 51 stone plaque, so the rocks are not the floor tiles.

**Possible puzzle (inference, nothing in code or text says it):** six rocks spread over the surface areas (forest,
mountain, mansion, jungle, Civilization) each gave one number; the vault "contraption" was a floor of numeral and symbol
tiles where the player had to step on or set the right ones. With digit tiles only up to 10 and an "=" tile, the most
consistent design is an equation or a sequence: e.g. the six numbers in rock order (or summed/combined, the five
known sum to 223) give the tiles to step on, or each rock's number picks one of the ten symbols. The known values 2, 5,
18, 59, 139 do not form an obvious series, and with one rock missing the intended answer cannot be recovered.

## Placement records across PAL / USA / JP (2026-11-09)

Every room's object placement records (APD blocks) compared between the three retail discs. Checked statically on
the disc data (not seen in-game yet).

- **AREA02 section 3 (top-down tunnels): 26 log props only in JP.** JP `AREA02/D307.GAM` (unpacked; model table
  0x23224, placement block 0x23ECC, 48 records) places 26 layer 4 type 0x41 records (`tunnelStaticObjHandler`), all
  model 6 at height 54: groups of three 24-28 px apart at (398-446,-1046), (471-527,-506), (612-660,-637) + (636,-668),
  (1323-1379,-569), (1527-1575,-828), a row of six at y -722 (x 998-1118) and one at y -686 (x 1020-1076). Model 6 is
  a half log (24 x 56). USA/PAL: the same block has 22 records and no type 0x41, but model 6 is still in the table
  (USA `D307.GAM` 0x1E5C0, PAL `D306.GAM` 0x1E5E0): model and handler survive with nothing placing them. The
  collision map is walkable at every log cell, so they were decoration (log floors); a link with the graffiti-floor
  puzzle is a guess.
  The section layout is the same in JP and PAL/USA (PAL and USA files byte-identical): same 240x168 collision grid
  (136 of 40,320 cells differ, local wall edits; every log cell walkable in both), ~97 % of the backdrop polygons
  shared, the 22 shared records (20 gates, 2 type 0x3F) at the same positions, the same five exits and eleven actors
  (JP has a 12th at (495,-409)). Decisive: the floor mesh has a 24x20 / 28x20 tile cut exactly at each of the 25
  distinct log positions (two JP records share (1076,-686)) **in both versions**, and PAL has no tile of that size
  anywhere else: the PAL floor keeps the outlines of the removed logs. In JP each tile also carries a shadow quad
  (octagon blob, page 14, UV 136..159 x 40..63, CLUT 0x79CF) that PAL dropped; the blob texture is still in PAL VRAM.
  So the JP coordinates injected into PAL are the original positions (the shadows are missing).
- **AREA00: five static solid blocks only in JP.** JP layer 4 type 0x08 is a static solid-model handler (X00
  0x8011D824, near-twin of `updateStaticSolid`); in USA/PAL the slot is the empty stub (`updateL4Type08Object`, PAL
  0x80030404, USA 0x80033858). JP records: a rising staircase of three in section 0 at (1021,-120), (1117,-197),
  (1198,-251), models 0x2E/0x2F/0x30, and two in section 1 at (2546,-476), (2612,-370), models 0x35/0x36. The models
  are gone from USA/PAL (JP `AREA00/D003.GAM` model table 0x2F6DC: 85 entries, USA 79 at 0x2D914). They are
  grass/hay-topped blocks. A sixth JP-only model is not accounted for yet.
- **AREA00 section 2: a fourth swaying stone tower.** JP places a layer 4 type 0x01 chain, subtype 3, at
  (2792,-298), 8 stone-block parts (models 3..10 like subtypes 0/1). PAL `updateA00L4Type01` still handles subtype 3
  (child layer 2 type 2 subtype 7) and X000 `D_80138814[3]` = (2792,-377) is that tower's child spawn point, but PAL
  places only subtypes 0-2: dead code and data in PAL.
- **AREA04 sections 0/1: a PAL-only placement edit** (not cut content). USA and JP are identical; PAL (`D002`/`D102`
  vs USA/JP `D004`/`D104`) swaps counted collectibles 9 and 10, unlinks the decoration at depth 1260 (subtype 21 → 0)
  and adds a 3-part biting plant at (1077,-356,1260).
- **JP memory card code accepts a development product code**: JP DSPSUB2 `memCardReadDirectory` (0x800EA490) takes
  save names starting `BISLPS-01144` or `BISCPS-99999`; USA and PAL only their own (details and implications: docs/SAVE_FORMAT.md).
- Still to explain (JP vs USA placements): a JP-only spring pad in AREA03 sections 1/5 at (3264,-1538), backdrop
  sprite sets in AREA16 section 3, an extra scenery sprite in AREA09, extra pool objects / moved positions in
  AREA00/01, 22 objects USA added in AREA03 section 2, and JP's AREA12 copy differing a lot from USA's.
- Checked, nothing new: USA/JP-only code (rewrites of existing features), event ids, files outside the file tables
  (the same 31 on all three discs), event menu order and icons, debug strings.

## PrePre Vol. 9 (PCPX-96093): Tombi animations and art retail does not have (2026-11-10)

Everything below was recomputed from the two discs (PrePre 9 extracted with `tools/disc_fetch.sh PREPRE9`, PAL in
`game/disc/fs`); the scratch scripts were not committed, the method is described so it can be redone with tools/anim.py,
tools/wpp.py, tools/vram.py and tools/area000.py.

### Frames

- Pre9 player frame table: `TOMBA/AREA01/A007.000` (byte-identical to `TOMBA/AREA02/D105.000`), WPP table of **379**
  frames, SPR parts in `AREA01/A008.GAM` (unpacked +0x14A0, 379 frames), palette rows 0/1 of `AREA01/CLUT01.GAM`.
  Retail: `AREA00/A004.000`, 358 frames.
- Compared by md5 of the decoded frame, then by silhouette overlap (bounding-box aligned, >= 0.90 = retouched):
  **173 identical, 100 retouched, 106 new** (not in retail at all; also checked against all 41 WPP tables of every
  retail area, 13,017 frames: no match).
- New frames, runs: 72, 98, 109, 112-124, 138-140, 177-179, 195-203, 219-228, 237-242, 245, 255-258, 264-292,
  298-300, 304-306, 317-326, 331, 334-340, 375.
- Retail frames 265-357 (idle breathing, roll land, victory pose, the later additions) do not exist in Pre9.

### Player animation table, paired by id

Pre9's table is in `TOMBA/X01.BIN` +0x380: **56 ids** (retail: 90 in X0nn +0x3DC). The same id is the same move in both
builds (the state code selects ids), so pairing by id is exact. Mean silhouette overlap of the two sequences aligned in time:

| id | retail name | Pre9 | retail | verdict |
|---:|---|---|---|---|
| 0, 19 | idle | one held frame (0) | 10 frames: 0 for 360 ticks, then 269-275 look-around loop | retail animated it |
| 5 | crouch | 6 frames | 8 frames | changed |
| 33 | crouch walk | dive to crouch 331-338 (6 new frames) | different move | **replaced** |
| 37 | anim_37 | shrug 177-179 (new) | different pose | **replaced** |
| 49 | roll land | somersault 219-228 (10 new frames) | roll land 169-178 | **replaced** |
| 1, 15, 48, 54, 55 | walk, jump alt, get up, 54, get up slow | | | retouched (0.85-0.97) |
| the other 41 | | | | same (>= 0.97) |

Ids 56-89 exist only in retail.

### Orphan player scripts in TOMBA.EXE

Brute scan of the exe data for 8-byte animation entries (tools/anim.py format) that use the new frames. **20 scripts** sit in
the player-script region (0x80011928..0x8001271C, between the 56 referenced ones); nothing points at them: no data word in
TOMBA.EXE, X01.BIN, X02.BIN, DSPSUB*.BIN or INFO.BIN, no lui/addiu pair. They were dropped from the table before Pre9
(1997-11) and the data stayed. Ticks are game ticks (40 ms); every one ends in `hold` (last frame kept, caller signalled),
the shape of retail's multi-stage actions.

| script (vram) | frames / ticks | end | reading of the pose |
|---|---|---|---|
| 0x80011A50 | 112/12 113/8 114/16 | hold | lift, part 1: crouch and grab (long holds) |
| 0x80011ECC | 115/7 116/9 117/10 118/7 119/9 120/10 | hold | lift, part 2: raise to the chest / overhead |
| 0x80011EFC | 121/6 122/5 123/4 124/5 | hold | lift, part 3: lower / put down |
| 0x800123B8 | 255/10 256/6 257/10 258/14 | hold | pull something with both hands, held on the last tug |
| 0x80012178 / 0x80012198 / 0x800121B8 | 195-197, 198-200, 201-203 (6 ticks each, ping-pong) | hold | three punch / strike poses with the leg raised |
| 0x80012400 | 264/2 265/2 266/3 267/4 268/60 | hold | fast wind-up, 60-tick hold |
| 0x80012428 | 269-276, 10-12 ticks each | hold | slow heavy walk (carrying?) |
| 0x80012468 | 277-281 ping-pong, 7-11 ticks | hold | strain / rock back and forth |
| 0x800124A8 | 282/10 283/10 284/10 283/10 | hold | |
| 0x800124C8 | 285-292, 5-8 ticks | hold | flip with kicks |
| 0x80012308 | 238/32 239/8 240/24 239/10 | hold | lying down, two long holds |
| 0x80012328 | 241/6 242/1 243/2 244/3 245/4 244/1 243/1 242/1 | hold | scream / shiver |
| 0x80011FAC | 138/10 139/8 140/10 139/8 | hold | arms up, seen from behind |
| 0x80012530 | 298/10 299/10 300/10 299/10 | hold | |
| 0x80012550 | 304/20 305/8 306/14 305/8 | hold | guard pose |
| 0x800125C8 | 317/3 318/4 319/5 320/6 | hold | crouched with dotted effect boxes (charge?) |
| 0x800125E8 | 321/14 322/10 323/9 324/14 325/10 326/8 | loop | hunched walk, follows 317-320 |
| 0x80011B80 | 339/32 340/60 | hold | |

The 84 frames these scripts use are exactly the new frames no table reaches; the demo cannot play them.

### What was checked for the object they act on (nothing demo-only found)

- **Placement records** (APD 16-byte records, PAL format, `AREA01/D004.GAM` = `D104.GAM`, 315 records / 164 world
  positions; `AREA02/D008.GAM` + `D106.GAM`, 9): matched to retail AREA01 / AREA02 by position. Types are renumbered
  between the builds (Pre9 L4 type 13 at (2876,-300,-45) is retail L4 type 7 `updateAreaMechanisms` at the same spot),
  so the match is by coordinates: 8 Pre9 records have no retail object within 24 units, all known objects moved a few
  units (two scenery sprites, three items, one chest) plus one L2 type 13 at (4187,-378,618) whose handler
  (X01.BIN +0x2E8F8, overlay base 0x800ED548) is a four-state stub that only calls two exe functions. AREA02 is
  identical record for record.
- **Texture pages**: `AREA01/A002.GAM` (objects: chests, barrels, pots, arrow sign, a rolling log in 4 views at page
  x 768-880, y 220-240) differs from retail's `AREA01/A002.GAM` in 21 regions, all retouches; the log is in retail
  AREA01 and AREA07 too. `AREA01/B000.GAM` 3 retouches (the slanted two-pronged trident at (208,192) / (208,216) is the
  Koma pig's); `AREA02/B001.000` identical, `B003.000` 140 texels. `A001` / `A003` of Pre9 are 4x the retail size
  (retail reorganised them) and `AREA01/A005`, `AREA02/A003` have no retail counterpart: not comparable 1:1.
- **Pre8** (PCPX-96084) `TOMBA/AREA00/PLAYER.WPP`: 313 frames of older, smaller art (24-32 px wide, one palette at
  TOMBA.EXE +0x989A8, uploaded to VRAM (0,480)); no frame identical to Pre9 or retail (21 / 19 similar by silhouette);
  its script tables in the exe only cover NPC frames (<= 87), Tombi is animated by code. Pre8 has back-flips, a
  head-first fall and a spinning kick (frames 96-111, 115-125, 208-223) that neither Pre9 nor retail keep.

- **JP demo SLPM-80165** (`GAME_FILES_URL_JP_DEMO`, checked 2026-11-10): `AREA00/A005.000` = Pre9's 379-frame table
  byte for byte, followed by a second table of 83 frames that is already identical to retail's second table of
  `AREA00/A004.000`; the player table (`AREA00/X00.BIN` +0x3A8) has the same 56 ids with the same scripts, and the same
  20 orphan scripts sit in `SLPM_801.65` (0x800109A4..0x8001153C), still unreferenced. So nothing changed for Tombi
  between PrePre 9 and that demo; the new retail frames and ids came later.

### Top-down Tomba (the overhead sprite set), demo vs retail

- Demo table: `AREA08/D002.000` +0x3FA28 in SLPM-80165 (= PrePre 9 `AREA02/D005.000`, byte-identical), **106** frames,
  SPR in `AREA08/D003.GAM`, CLUT rows 0/1. Retail: 127 frames (`AREA19/D204.000`, `AREA02/D004.000`, `AREA05/D005.000`,
  `AREA08/D103.000`; the sets differ per area). By md5 / silhouette against every retail table: 67 identical, 17
  retouched, **22 in no retail table**: 25-29, 31-35, 77-85, 89, 96-97. Retail frames 106-126 (item held overhead,
  crouch, throw / raise in three directions, scream) are later additions.
- Demo player animation block, `SLPM_801.65` 0x80010800 (retail: MAIN0 `MAP_PLAYER_*` 0x800106B4, 37 pointers): **21
  pointers** = idle 5 (one held frame each, 101-105: the demo idle does not animate), walk 5 (frames 0-24, identical
  scripts to retail), button-A walk 5 (demo frames 37-76 by 8; retail renumbered them to 25-64), then only **two action
  sets of three directions**: 25/26, 27/28, 29/30 (6 ticks then a 60-tick hold: raise the arms and keep them up, the
  demo's item pick-up) and 31/32, 33/34, 35/36 (16/16, hold). Retail has eight action sets after the walks (item raise
  126/65, 70/67, 71/69; 72-74; 75-83 in pairs; 84-86 holds; the two look-around idles 87-104; 105-110; 111-125).
  So the demo frames 25-35 are the **first drawing of the item pick-up**, redrawn in retail as 65-74 / 126; they were
  used in the demo, not orphaned.
- Demo frames 77-85, 89 and 96-97 are in no player script of the demo (the exe block above is the only player table;
  the script tables inside the AREA08 overlay `INFO.BIN` belong to the area's NPCs and index their own sprite sets, so
  they say nothing about Tomba's frames). They are unused top-down art, like the side-view orphans, with no script.

### Lost: the top-down Charity Wings glide (JP demo), rebuilt (2026-11-10)

How it was put together, step by step, so it can be redone (`tools/wings_glide.py` does all of it from the demo disc):

1. **The Tomba frames.** The demo's overhead set (`AREA08/D002.000` +0x3FA28, 106 frames; SPR parts `AREA08/D003.GAM`
   table at +0; palette rows 0/1 of `AREA08/CLUT01.GAM`) has frames 77, 78, 79 with the right arm raised overhead: in
   no table and in no player script of the demo, and not in retail (retail's 127-frame set redrew the item moves as
   65-74 and 126). Rendered with each part's own CLUT row like the side view (two parts per frame, torso and legs).
2. **The feather.** Retail draws the Charity Wings item from the resident item table (`SPR_DATA[4]`, kind 0x34, PAL
   `SYS/LD0SYS.BIN` record of AREA00 list 0): item descriptor 7 (`tools/sprclut.py items`) -> anim set 0x80012214 ->
   scripts 0x80012834 (frame 159, static, 24x24) and 0x8001283C (frames 160, 161, 162, the flap, 1 tick each), CLUT
   word 0x7AD6 = VRAM (352,491), i.e. row 11 of the item palette block (`SYSTEM/A10004.000` at (352,480)). The texels
   were cut from the retail VRAM (`vram.cut_part` with tpage base 0x14), packed back to 4 bpp rows and searched byte for
   byte in every demo file at every page width: found in the JP demo's `SYSTEM/A00000.GAM` (unpacked, 512 px wide
   page) and in PrePre 9's `TOMBA/A00002.GAM` (256 px wide; = `AREA01/A005.GAM`, `AREA02/A001.GAM`) at the same page
   coordinates: 159 at (224,104), 160 at (200,80), 161 at (216,64). Retail's 162 (24x16) is not in either demo. The
   palette row 11 is byte-identical in retail, the JP demo (`SYSTEM/A00002.000`) and PrePre 9 (`TOMBA/A00003.000`):
   white, five light blues, three quill browns, a blue-grey.
3. **Pinning the feather to the hand.** Measured from the pixels: the raised hand's topmost opaque pixel (right third
   of the sprite) is at (22,2) in frame 77 and (22,1) in 78 and 79; the feather's quill end is the lowest opaque pixel
   of each feather frame ((2,22) in 159, (2,14) in 160, (7,14) in 161). The quill end goes 1 px above the hand's top
   pixel (the user's call, after trying 2 px inside and 0).
4. **The rotor.** The three feather frames read as phases of one turn: 159 flat, 160 angled, 161 edge-on; mirroring
   160 and 159 gives the other half (retail's missing 162 is exactly the mirrored 160). Cycle of 8 phases at 70 ms;
   Tomba cycles 77-78-79-78 at half that rate; the figure floats +-2 px (sine over 32 steps). The spin order, the
   timings and the float are ours: the demo leaves no script for this sequence.

What is from the disc: every pixel, the hand and quill positions, the palette. What is a reconstruction: the spin
order, the speeds, the float. Output of the tool: `wings_glide_x5.gif`, `wings_glide_1x.gif`, `wings_glide_steps.png`
and the three feather PNGs (never committed).

### The other sprite tables of the JP demo (enemies, NPCs, HUD)

`tools/find_unused_gfx.py OUT --jp game_jp_demo/disc.bin --areas 0,8 --skip-vram` pairs every SPR table of the demo
with a PAL table by shape and compares the 16-byte part records frame by frame. Demo tables: AREA00/A006.GAM (379 Tomba,
10, 194, 267, 83, 11 frames), AREA00/D503.GAM (18), AREA08/A003.GAM (168, 153), AREA08/D003.GAM (106 top-down Tomba,
431 top-down NPCs, 17, 8). Result: the 431-frame top-down NPC table is PAL's AR02 SPR 3B minus its last 6 frames, the
194-frame table (PAL SPR 32) differs in 3 frames, the 267-frame one (PAL SPR 33, 290) in 6, the 163/153-frame ones
(PAL SPR 39, 267) in 52/43 and AREA08's 168-frame one (PAL SPR 34, 289) in 28; but rendered from the demo's own VRAM
(area pages placed as PAL does, SYSTEM atlases tried at (64,0) with `vram.infer_base`) every one of those frames is an
empty rectangle, i.e. placeholder frames, and the tool's own "new images" of them are flat blocks. The 83-frame table
has no PAL twin: the localized HUD / text strip. So apart from Tomba (side view and top-down, above) the demo's sprite
tables hold no art that retail lacks; the differences are retail's additions (PAL-only frames listed in the tool's
INDICE.txt: e.g. SPR 34 frames 192-252, SPR 3A 70-126).

### Items and placed objects of the JP demo that retail does not have (2026-11-10)

Items (`tools/item_icons_find.py game_jp_demo/fs`, `tools/jpdemo_text.py game_jp_demo/disc.bin OUT --jp
game_jp/disc/tomba_jp.bin`, retail JP text from `tools/jp_text.py`):

- Icon page `SYSTEM/A00012.000` (1536 px wide, grid from x 512): 64 slots, 58 retail icons found. Slots **34, 36, 44,
  45, 52, 61** carry icons retail never used (the corkscrew / curly key / bottle / powder / rocks set already seen in
  Dengeki D5 and PrePre 9) while their description entries are the placeholders アイテム０３４.., and slot **24** is a
  grey pod icon described as 魔ブタを封じこめる たいせつな ふくろ ("the precious bag that seals an Evil Pig"): one generic
  bag before the seven coloured ones (37-43, present too). Description bank `SYSTEM/A00013.000` (201 entries, retail id
  order): 11 entries reworded against retail JP (ids 0, 6, 9-14, 24, 26, 27).
- Item names in the demo's "used X" messages that appear nowhere in the retail JP text: はやさレベル＋１ (Speed Level +1),
  ケガをしたコイヌ (Injured Puppy: the early Baron), みえないカガミ (Invisible Mirror), バンクの実 (Bunk Fruit; retail: Bunk
  Flower), ワライダケ / ナキダケ (laughing / weeping mushroom under other names), ライフゲージかくだい (Life gauge expand;
  retail keeps only ぞうか). グルドカ / グルピン / ドカピン / ケモノダッシュ / アルコール / エサねんりょう exist in retail JP.
- No feather object: the only feather is the Charity Wings item (icon 7, めぐみの羽根), used in the demo as in retail.

Placed objects: the demo keeps its APD records inside the room containers (`AREA00/D003.GAM` +0x481CC.., `D302.GAM`,
`D402.GAM`, `D503.GAM`; 403 distinct records) like the retail JP disc (`AREA00/D003.GAM` etc., 426). Compared record by
record with retail JP (same layer, type, subtype, x, y, z) the demo-only ones are:

| where | record | note |
|---|---|---|
| section 2 (3020,-512) | layer 5 type 4 subtype 14 = **Baron lying in the world as a pick-up** (flag 44) | retail gives Baron through the event; no retail disc places item 14 |
| section 5 (2642,-300,168) | layer 2 type 39, sprite set 5, flag 47 | no retail disc places an L2 type 39 in AREA00; in PAL the L2 slot 39 is not a handler |
| room 3 (120,-56) | layer 3 type 21 | PAL slot 21 of layer 3 is not a handler |
| room 5 (2670 / 2600,-395,907) | layer 4 type 7 subtype 8, models 1 / 2 | area-mechanism variant 8, unplaced in retail |
| section 0 (1200,-36,204) | five layer 4 type 27 records, models 0x4C..0x50 at one point | a five-model sequence (`updateL4Type1BObject` in PAL) retail does not place |
| section 0 / 1 | 12 layer 2 type 1 objects in rows (x 230-558, 2014-2038), L4 type 26 subtype 3 (800,-40), L4 type 0 subtypes 5-6 (room 1), Chick at (2023,-345) | small enemies / fixtures / an item retail moved or removed |

Caveat: type numbers are the demo's; its handler tables were not matched against retail's, so a "type not placed in
retail" can also be a renumbered type (as with PrePre 9's L4 13 = retail L4 7). The static solid blocks (L4 type 8) and
the fourth stone tower (L4 type 1 subtype 3) the demo places are also in retail JP (see "Placement records across
PAL / USA / JP") and are not listed here. Not rendered: `tools/model_view.py obj` needs the PAL load lists; the demo's
model table is at `D003.GAM` unpacked +0x30A78 (87 entries).

Conclusion: the lift / pull mechanic was programmed with its timing and then removed before PrePre 9; the object it
used is in none of the demo's data. Only the frames and these scripts remain.

### VRAM sweep of the two demo discs: texture pages, retail filtered out (2026-11-10)

Every texture page of the JP demo (25 pages, load records at `SLPM_801.65` +0x5C258) and of PrePre 9 (38 pages,
`TOMBA.EXE` +0x64A5C) was placed in VRAM from the demo's own load lists, hashed in 8x8 / 16x16 texel blocks against
every retail PAL page (and retail JP and the other demo as extra references), and the texels no SPR part (every table
found by shape, every tpage base) and no model polygon samples were kept. Code-drawn primitives with fixed UVs (HUD,
sub-screen, title, text) are not modelled, so on those pages "unsampled" alone proves nothing. Throw-away scripts, not
committed. What came out as art retail does not have:

| Disc, page | Find | Palette | Status |
|---|---|---|---|
| PrePre 9 `TOMBA/AREA02/B000.GAM` (0,0)-(512,170); byte-identical copies in the sub-screen pages `A00061`/`A00064`/`A00067`/`A00069` | **An unused dwarf sprite sheet**: about 24 poses of the forest dwarf (idle, walk, kick, knocked back), six 36x40 robed, bearded elders (front / side / back), five pennants on poles, two small dark silhouettes. 0 % sampled by any Pre9 part or polygon; 5-25 % block matches with PAL/JP (noise level). | The retail forest dwarfs (AREA01 list 1, SPR 0x32) use CLUT 0x7C13 / 0x7C53 / 0x7C93 / 0x7CD3 = `CLUT01` rows 16-19 col 11 (pink, tan, light tan, green); the same four rows are in Pre9 `TOMBA/AREA01/CLUT01.GAM` and colour the sheet exactly like the retail dwarfs. | Cut art: an SPR-page version of the dwarfs retail only keeps as WPP / the AREA01 table. |
| JP demo `SYSTEM/A00012.000` (10,67) 28x36 | A small full-body Tomba (pink hair, green shorts) on the sub-screen page, next to the マップ label. Not in PAL, JP retail or Pre9 (3-4 % block matches). Probably the extra-life icon of the demo's sub-screen, drawn by DSPSUB code (not modelled); the user notes retail's extra-life icon differs. | `SYSTEM/DSPCLUT.TIM` row 9 col 0. | Demo-only sub-screen art. |
| JP demo `SYSTEM/A00007.000` (8 bpp, 64x256 words at (0,256), CLUT `A00006.000`) | Eight strips of Japanese text: the demo's start-up disclaimer ("this software is under development, unexpected bugs may occur... the disc cannot be exchanged"). | `A00006.000` | Demo-only. |

Not new, after checking: the Pre9 scarf runner in `TOMBA/AREA01/B001.GAM` (255,118), two 35x43 frames of a running
figure with a red cap, goggles and a brown scarf (`CLUT01` row 0 col 11) is the thief who is in the retail game (user
identification; the frames are retouched / on another page there, so the block match is low); the demo-only item icons
(slots 34, 36, 44, 45, 52, 61, listed above); the Japanese title logo, faces, kanji font and sub-screen labels (all in
retail JP); the feather, the Evil Pig trident and the logs (sampled, or in retail JP). JP AREA00/AREA08 room pages, the
Pre9 AREA01 object atlases and AREA02 room pages hold no other unreferenced art. USA and SCES-01330 discs not checked.
