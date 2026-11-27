# PrePre 8 ("Pure 8") event titles: areas, retail fate, Tomba! 2 and the candies

PrePre Vol. 8 (PCPX-96084, June 1997; the community calls it "Pure 8") has the oldest event-title list of any Tomba
build. This page sorts every title by the area it was planned for, says what became of it in retail, flags the titles
that resemble Tomba! 2 content, and lists every candy known in the Tomba! 1 builds. Background on the build:
[PREPRE8.md](PREPRE8.md). Cut items: [CUT_CONTENT.md](CUT_CONTENT.md). Retail events by area:
[EVENTS_MAP.md](EVENTS_MAP.md). Civilization towns: [CIVILIZATION.md](CIVILIZATION.md).

Credit: community researcher **Folkin** published the in-game list of this demo and later a full annotated list of
the titles (a video script, "Folkin's Pure 8 event list" below). His list is in the same order as the table in the exe
and was used to cross-check every reading and mapping on this page (section "Cross-check with Folkin's list").

Nothing from the discs is committed. The titles are quoted one by one, as the other docs do. Decoder:
`python3 -I tools/prepre8_text.py TOMBA.EXE`. Sources checked on 2026-11-27: PrePre 8 `TOMBA.EXE`; PAL retail
(event titles and the sub-screen order from `MAIN0.EXE` / `D0SPSUB.BIN`); JP retail SLPS-01144 (the per-area title
tables of `tools/event_titles.py`, their title lists and the `DSPSUB.BIN` hint bank, via `tools/jp_text.py`); the
Tomba! 2 USA disc (`MAIN.EXE`, whose event and item names are plain ASCII).

## How the list was read

- **169 titles, not 167.** The table at 0x800AA140 holds 169 event titles (indexes 0..168, the last three being
  「大団円1」..「大団円3」), then the placeholder `・・・・・・・・・・・` and six item names. The earlier count in
  PREPRE8.md was off by two.
- **Twelve more kanji read.** The 12x12 kanji of the title font are sorted by on-reading, so an unread glyph sits
  between two known readings. Seven of the unread glyphs are confirmed by the retail JP title of the same event
  (泳, 恵, 絵, 燃, 能, 紙, and 具: the old decoder had 具 and 空 one slot apart, so 道空 is 道具 and 窟の主 is 空の主).
  Five are read from their slot alone and fit the title: 雲 (うん), 砦 (さい), 毒 (どく), 勇 (ゆう), 裸 (ら). One glyph,
  in the item name 花の〓, is still unread. `tools/prepre8_text.py` now has them.
- **Retail event id = PrePre 8 index - 1.** This holds for the whole list, not only the 25 slots the demo uses (notes/
  secrets.md showed it for those). Over 70 titles are identical, or identical but for spelling and punctuation, to the JP retail title of
  that id (e.g. 30 嵐の山の怪 = retail 29, 127 万年老人の数字玉 = retail 126, 145 億年老人の願い = retail 144), and the
  Dengeki D5 hint for id 83 is still the PrePre 8 title 84 お城の食事会 ("a dinner party has a lot of feast"). So the
  PrePre 8 list is the retail event table in an earlier state. A cut title is an id that retail either left untitled
  (most of the 0xFF events of notes/secrets.md), or reused for a different event. Retail ids 166 and up are later
  additions (Hungry Monkey, Peach Flower Gas, the pig bags...), appended after the old list.
- **Area evidence.** PrePre 8 itself has area data only for area 0 and 1 (25 list slots, area-0 rooms, see
  PREPRE8.md), so no title past index 24 has an area in this build. The area of each title therefore comes from:
  the retail area of the same id (EVENTS_MAP.md, and the JP per-area title tables, which title an event only in the
  areas where it can pop up); the list order (the list runs area by area, like the retail ids); the neighbours; and the
  names in the title. **evidence** = the retail id is placed in that area, the JP table titles it there, or the title
  names the place; **inferred** = only the order, the neighbours or the theme.
- **Status.** *retail (same)*: the same event, same or near-same title. *retail (renamed / reworked)*: the same event
  slot with the same goal, retitled. *cut (slot reused)*: the PrePre 8 event is gone and retail put a different event
  in that id. *cut*: retail has no playable event for the id (untitled, or titled but never triggered).
- **JP hint bank.** Retail JP `DSPSUB.BIN` block 1 holds one hint per id, also for most cut ids. Where it has one, it
  is quoted in the tables; most fit the PrePre 8 title (106 "nobody beats me in a footrace", 109 "glasses that see
  through anything" for 110 ブタの町のブタメガネ, 117 "where is the new road" for 118 トンネルはどこだ, 136 "weight decides
  the path?" for 137 おもいは地、かるいは天, 137 "an important person in the clouds" for 138 雲の中身, 165 "before you know
  it, everyone is happy" for 166 「大団円1」). Two do not: 91 (Pig Town) has "so light it floats up", and 143 (the
  Crystal Cave) has the Civilization reunion "the magic fades and the three towns become one".

## Overview

| Area | Titles | In retail | Cut | Tomba! 2 link |
|---|---|---|---|---|
| Village of All Beginnings | 14 | 11 | 3 | 1 |
| 100 Flowers / Dwarf Village | 27 | 24 | 3 | 2 |
| Phoenix Mountain | 10 | 9 | 1 | 0 |
| Haunted Mansion | 9 | 7 | 2 | 3 |
| Baccus Village / Lake | 10 | 6 | 4 | 1 |
| Masakari Jungle | 19 | 10 | 9 | 1 |
| Sea / mermaids / water | 12 | 5 | 7 | 4 |
| Village of Civilization | 23 | 7 | 16 | 3 |
| Island secrets (cut area) | 7 | 0 | 7 | 0 |
| Unplaced | 2 | 0 | 2 | 1 |
| Snow / cold area (cut) | 9 | 0 | 9 | 6 |
| Pig Town (cut) | 8 | 0 | 8 | 5 |
| Sky / towers / light (cut) | 7 | 0 | 7 | 3 |
| Endgame / Evil Pigs | 12 | 6 | 6 | 3 |
| **All** | **169** | **85** | **84** | **33** |

By status: 40 same, 45 renamed / reworked, 22 cut with the slot reused by another retail event, 62 cut. Tomba! 2: 4 confirmed, 29 likely.

Status grid, in list order (R = in retail, x = cut, ² = confirmed Tomba! 2 counterpart, ²? = likely one; the code
is the area, see the tables below: V0 Village of All Beginnings, DW 100 Flowers / Dwarf Village, PM Phoenix Mountain,
HM Haunted Mansion, BV Baccus, MJ Masakari Jungle, SE sea / mermaids / water, CV Village of Civilization, IS island
secrets, SN snow / cold area, PT Pig Town, SK sky / towers / light, EG endgame, UN unplaced):

| # | +0 | +1 | +2 | +3 | +4 | +5 | +6 | +7 | +8 | +9 |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 | x V0 | R V0 | R V0 | R V0 | R V0 | R V0 | R V0 | x V0 | R² V0 | R V0 |
| 10 | R V0 | R V0 | R V0 | R DW | R DW | R DW | R DW | R DW | R DW | x DW |
| 20 | R DW | R DW | R DW | R DW | R²? DW | R DW | R DW | x DW | R² DW | x DW |
| 30 | R PM | R PM | R PM | x PM | R PM | R PM | R PM | R PM | R DW | R DW |
| 40 | R DW | R DW | R²? HM | R²? HM | x² HM | R BV | x BV | R²? BV | R BV | x BV |
| 50 | R BV | R BV | R HM | x BV | R BV | R HM | R HM | R HM | x HM | R HM |
| 60 | x MJ | x MJ | x MJ | R MJ | R MJ | R MJ | R CV | x IS | x IS | x IS |
| 70 | x IS | x IS | x IS | x IS | R CV | x CV | x CV | x CV | x CV | x²? CV |
| 80 | R DW | R DW | x UN | R PM | x²? UN | x²? SN | x SN | x SN | x²? SN | x²? SN |
| 90 | x CV | R MJ | x²? PT | x PT | x²? PT | x CV | x CV | x CV | x CV | x²? CV |
| 100 | x CV | x CV | x CV | R CV | x CV | R²? CV | R CV | x PT | x²? PT | x²? PT |
| 110 | x²? PT | x PT | x V0 | x²? SN | R DW | R CV | x BV | R CV | x MJ | R MJ |
| 120 | R MJ | x SE | R²? SE | x SE | R²? MJ | R MJ | x² SE | R MJ | x MJ | x MJ |
| 130 | R PM | x SK | x SK | x CV | R SE | R SE | R SE | x²? SK | x SK | x MJ |
| 140 | x MJ | x MJ | x²? SN | x SN | x²? SN | R DW | R DW | R DW | x²? SE | x²? SK |
| 150 | x SE | R MJ | x²? SK | x SK | R SE | x²? SE | x SE | R EG | R²? EG | x²? EG |
| 160 | R EG | R EG | R²? EG | x EG | x EG | R EG | x EG | x EG | x EG |  |

A bar chart of the same counts (PNG, dark, text only) was made for Discord; it is not in the repo.

## Places that are not in retail

**A snow / cold area.** Two runs of titles describe one: 85 光がほしい ("I want light"), 86 おひさまポカポカ… ("the sun
is nice and warm..."), 87 寒い!裸? ("Cold! Naked?"), 88 溶かす、溶ければ… ("melt it, and if it melts...") and 89
雪玉ゴロゴロその先は… ("where the rolling snowball goes..."); later 142 雪がふったら… ("when the snow falls..."), 143
毒毒どうくつのナゾ (a poison cave) and 144 水晶のどうくつ (a crystal cave). 113 固める、止まった時に… ("harden it, when
time stops") reads as the freezing half of 88. Retail has no snow anywhere; ids 84 and 141-143 are untitled in every
build, and 85-88 were reused for the jungle pig and three Dwarf Village errands. *Inferred* (titles only): no
retail area, no data. Tomba! 2 has exactly this: snow and ice at Kujara Ranch (Snow Fireflies, "Collect Snow",
the Evil Ice Pig, "Melt the Giant Ice!", "Sea Anemone's Ice").

**Pig Town.** 92 ブタの町 ("Pig Town"), 93 白いケムリの家 ("the house of white smoke"), 94 コトバをおぼえよう!Ⅲ ("Let's
learn the language! III": I was the dwarves', II the Masakari's), and a second run 107-111: 森の案内人 ("the forest
guide"), うまい果実、まずい果実 ("tasty fruit, nasty fruit"), 挑戦!勇気の大樹 ("challenge! the great tree of courage"),
ブタの町のブタメガネ ("the pig glasses of Pig Town") and 王の本性 ("the king's true nature"). *Evidence* for the town
(titles 92 and 110 name it); the rest is *inferred* from the order. All these ids are untitled in every retail build.
Tomba! 2 has a Pig Tribe, a Clown Circus and a Pig Suit that lets Tomba talk to pigs (its item text: "speak Pig
Language"), which is what Language III would be.

**The island secrets.** 67 盗まれた島の絵本 ("the stolen island picture book"), 68 解読?らくがき岩 ("decode the graffiti
rock?") and 69-73 島のヒミツその1-5 ("secret of the island, parts 1-5"). Retail keeps only 68's title (JP, event 67,
hint "decode the letters of the graffiti rock"); the graffiti plaques exist in retail rooms (CUT_CONTENT.md
"Graffiti rocks"), and 163 らくがき床のナゾ ("riddle of the graffiti floor") matches the tunnel glyph sheet. Which
island is not said (the whole game is on one island).

**Sky, towers and light.** 131 危ない!落ちたら死ぬぞ, 132 ほこらの中に何がある? (a shrine), 137 おもいは地、かるいは天
("heavy to the earth, light to the sky"), 138 雲の中身 ("what's inside the cloud"), 149 塔のもりびと ("the tower keeper"),
152 光と影 ("light and shadow"), 153 光る石を探せ ("find the glowing stone"). *Inferred* group; no retail area.

**Sea and mermaids beyond retail.** 121 トビウオ アクロバット (flying fish), 123 海族の宝物 (the "sea folk", a pun on
海賊 pirates; the JP hint says "find the pirates' treasure"), 126 水門をあけろ! ("open the floodgate!"), 148
水を止めなきゃ!, 150 海を渡る方法, 155 人魚の王を救え ("save the Mermaid King"), 156 海底通路の行き先 ("where the undersea
passage leads"). Retail kept only the two mermaids, the raft and the diving.

**Endings.** 166-168 「大団円1-3」 ("grand finale 1-3"): three ending titles. Retail id 165 is untitled with the
hint "before you know it, everyone is happy"; ids 166-167 were reused for area-0 additions. Also 0 少年の旅立ち ("the
boy sets off"), the opening event, which has no retail id at all (retail starts at PrePre 8 index 1).

## Village of Civilization: proto material in the list

| Candidate | Evidence | Verdict |
|---|---|---|
| The Civilization run, 74-79 and 95-106 (retail ids 73-78 and 94-105) | Same ids as the cut towns' events: 76 Black Water and 99 started only in the cut Iron Town, 97/98 at the Stonetown clock-tower gears, 77 read only by the town spawners (notes/secrets.md). Titles: 大停電 (blackout), 黒い水の不思議, 文明のキカイ, 仲良くしようよ, 木人の村の大岩 (Wood People's village), いま何時? (= JP 動かない時計台), こわれない歯車?, 材料はどこだ!?, 大きなカギ穴 (= JP 鉄のカギ穴), さびたトビラをこわせ!, エサ燃料って何?, あぶない届け物 (= the bomb) | **Proto-VoC, evidence.** The towns were already planned in June 1997, as a chain of about twenty events |
| 79 進め!きかんしゃ ("Go, locomotive!") | In the Civilization run; retail reused id 78 for Find Charles! in the jungle | Proto-VoC, inferred (machine theme). Tomba! 2 has a trolley and a coal-mining town |
| 104 動かない鉄の人形 ("the iron doll that won't move") | In the run; the Iron Men are the AREA17 people; retail id 103 became The Cute Witch (AREA18) | Proto-VoC, inferred |
| 133 世紀の大発明 ("the invention of the century") | Retail id 132 became the witch's cold (AREA18) | Possible, inferred (invention theme only) |
| 90 緑の占い師 (Green Fortune Teller) | JP titles id 89 in the AREA11 and AREA18 lists; cut Wood village line about a fortune-teller's hut (CIVILIZATION.md) | Proto-VoC, evidence |
| 66 ナミダのビンちょうだい (tear bottle) | Retail id 65, started in AREA11 (Lumberjack Town) | Already retail; next to the Civilization run |
| 76 事件発生、犯人を探せ! ("incident! find the culprit!") | Id 75, untitled; JP hint "find the stolen Evil Pig Bag"; in the run | Proto-VoC, inferred. Folkin ties it to the green pig bag behind the Lumberjack boulder (not checked) |
| 75 電気うなぎを捕まえろ! (electric eel) | Id 74, JP titles it in the jungle (X10) and Civilization (17, 18) lists; item 108 Electric Eel | Jungle catch for a Civilization use, evidence for both lists |
| 20 / 27 the five boxes and their key | 20 = retail 19 Lost and Found, whose JP title is 魔女の忘れもの ("what the witch forgot"): witch Mizuno's crystal balls. Mizuno lives in the Civilization hub (AREA18). 27's id became To Phoenix Mountain | Weak link only: the boxes are in the forest; the witch they lead to is a VoC character |
| 19 ドワーフの助け (wood from the lumberjack dwarf) | JP hint "have the lumberjack dwarf cut a tree"; unused item 91 Wood; the cut AREA11 texts have a raft maker, and retail builds the raft at the Factory | Possible supply chain into Lumberjack Town, inferred |
| 14 / 61 / 94 Learn the Language I, II, III | I = dwarves (retail), II = Masakari (JP jungle list), III = after Pig Town | Not VoC. 62 マサカリ族のヒミツ is read only by the cut Stonetown dialogue, so the Stone People knew the Masakari secret |
| 144 水晶のどうくつ | Its id 143 has the JP hint "the magic fades and the three towns become one" | Retail reused this id for the Civilization reunion (hint only, never wired) |
| Unused items | 61 Iron, 62 Iron Wheel (only the cut smith gives it), 108 Electric Eel, 109 Black Water, 147 Coal: Civilization. 90 Letter (three sisters) and 140 Chief's Spear: jungle | As in CUT_CONTENT.md |

## By area

Rows keep the list index (#); the retail column gives the retail id (= # - 1), its PAL English title and, in
brackets, the JP retail title where JP titles it.

### Village of All Beginnings (AREA00)

14 titles: 11 in retail, 3 cut, 1 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 0 | 少年の旅立ち | A Young Boy Sets Off | cut | none: no retail id (retail ids start at PrePre 8 index 1) | inferred: first title; the opening, before Grandpa's Bracelet |  |
| 1 | ジィちゃんの形見 | Grandpa's Keepsake | retail (same) | 0: Grandpa's Bracelet (ジイちゃんの形見) | evidence: retail 0 is set in AREA00; PrePre 8 sets it at 0x800204C0 |  |
| 2 | 100年老人との出会い | Meeting the 100 Year Old Man | retail (same) | 1: The 100 Year Old Wise Man (100年老人との出会い) | evidence: PrePre 8 area 0 room 0 record starts it |  |
| 3 | 霧を吹き飛ばせ | Blow Away the Fog | retail (same) | 2: Clear the Fog (霧を吹き飛ばせ！) | evidence: retail AREA00 s0 |  |
| 4 | カエル…にかえる | Return the Frog... | retail (same) | 3: Take Me Home (カエル水にかえる) | evidence: retail AREA00 s0 |  |
| 5 | モトクロスコース | Motocross Course | retail (same) | 4: Motocross Course (モトクロスコース) | evidence: title in the area-0 JP list |  |
| 6 | キミ、だあれ? | Who Are You? | retail (same) | 5: Who are you? (キミ、だあれ？) | evidence: retail AREA00 |  |
| 7 | 大きな水たまり? | A Big Puddle? | cut | 6: untitled in PAL (JP 大きな水たまり？), never triggered | evidence, but conflicting. For area 0: PrePre 8's own dialogue has a kid asking 「キミ、泳げるのかい?」 (can you swim?) about the big puddle in the area-0 script, next to the place name ふるいけ (Ol' Pond), and Hyper 12's AREA00 title block lists 大きな水たまり？ at index 7 (= event 6 + 1). For the jungle: JP retail titles id 6 in the jungle (X10) list, and the 10,000 Year Old Man is 水たまりの万年老人 ("of the puddle"), id 118 = 119 大きな水たまりを渡れ. Area 0 is the better-supported reading for June 1997; the jungle list may only reflect where retail later moved the puddle idea |  |
| 8 | かくれ里のヤン | Yan of the Hidden Village | retail (renamed / reworked) | 7: Hide and Go Seek (JP keeps かくれ里のヤン) | evidence: retail AREA00; Yan is in PrePre 8's area-0 dialogue | confirmed: Yan of the Hidden Village |
| 9 | オイラ泳げない… | I Can't Swim... | retail (same) | 8: I Can't Swim... (オイラ泳げない…) | evidence: retail AREA00 / AREA10; PrePre 8 kid asks 「キミ、泳げるのかい?」 |  |
| 10 | コッカ卵の中身 | Inside the Kokka Eggs | retail (same) | 9: Inside the Kokka Eggs (コッカタマゴの中身) | evidence: PrePre 8 100-year-old man asks for the eggs |  |
| 11 | 100年老人の話 | Tale of the 100 Year Old Man | retail (renamed / reworked) | 10: Tale of the Evil Pigs (100年老人の話) | evidence: same JP title in retail |  |
| 12 | 100年老人の仲間 | A Friend of the 100 Year Old Man | retail (renamed / reworked) | 11: The 1,000 Year Old Man? (100年老人の仲間) | evidence: same JP title in retail |  |
| 112 | 始まりの村 | The Village of All Beginnings | cut (slot reused) | 111: Baccus Village (バッカスの村) | inferred: names area 0; slot reused for Baccus |  |

### Forest of 100 Flowers and Dwarf Village (AREA01 / AREA02 / AREA09)

27 titles: 24 in retail, 3 cut, 2 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 13 | ドワーフの長老 | The Dwarf Elder | retail (same) | 12: Dwarf Elder (ドワーフの長老) | evidence: retail; PrePre 8 man sends Tomba to the dwarf elder |  |
| 14 | コトバをおぼえよう!Ⅰ | Let's Learn the Language! I | retail (renamed / reworked) | 13: Beginner's Dwarf Language (コトバを吸いとろう！) | evidence: retail AREA01; "learn" became "suck up" the words |  |
| 15 | まいごのドワーフ | The Lost Dwarf | retail (same) | 14: A Lost Child (まいごのドワーフ) | evidence: retail AREA02 s0; PrePre 8 forest script has the lost dwarf child |  |
| 16 | 花のたね | Flower Seeds | retail (same) | 15: Flower Seeds (花のたね) | evidence: retail item-use event |  |
| 17 | AP・ゲート | AP Gate | retail (renamed / reworked) | 16: The AP Box (ＡＰボックス) | evidence: PrePre 8 dwarves talk about the AP "point gate" |  |
| 18 | ドワーフを救え | Save the Dwarves | retail (same) | 17: Save the Dwarves (ドワーフを救え) | evidence: retail AREA02 s1 |  |
| 19 | ドワーフの助け | The Dwarves' Help | cut | 18: untitled in PAL (JP ドワーフの助け), never triggered | evidence: JP hint: have the lumberjack dwarf cut a tree; JP hint: have the lumberjack dwarf cut a tree |  |
| 20 | 5つの箱の忘れもの | The Things Left in the Five Boxes | retail (renamed / reworked) | 19: Lost and Found (魔女の忘れもの) | evidence: retail AREA01 s3: witch Mizuno's three crystal balls; the "forgotten things" became the witch's |  |
| 21 | 消えた長老 | The Elder Has Vanished | retail (renamed / reworked) | 20: Stop the Fight! (ケンカをとめて) | evidence: retail AREA02 s1; the slot keeps the dwarf village |  |
| 22 | 地下ロウの大ドロボウ | The Great Thief of the Underground Jail | retail (renamed / reworked) | 21: The Great Escape (JP keeps 地下ロウの大ドロボウ) | evidence: retail AREA02 s2 = Underground Prison; same JP title |  |
| 23 | のぞいてごらん?! | Take a Look?! | retail (same) | 22: Look and See?! (のぞいてごらん？！) | evidence: retail AREA01 s3 |  |
| 24 | バロンは友だち | Baron Is a Friend | retail (renamed / reworked) | 23: A Man's Best Friend (バロンは友だち) | evidence: same JP title; PrePre 8 dwarf nurse names the dog Baron | likely: Baron Turned to Stone (Baron returns) |
| 25 | これ、な〜んだ? | What's This? | retail (same) | 24: What is this? (これ、な～んだ？) | evidence: retail AREA01 s3 |  |
| 26 | やしきの宝物? | The Mansion's Treasure? | retail (same) | 25: Treasures of the Mansion? (やしきの宝物？) | evidence: same JP title; titled in the forest/village lists |  |
| 27 | 5つの箱のカギ | The Key to the Five Boxes | cut (slot reused) | 26: To Phoenix Mountain... (大トリの山へ) | inferred: slot moved to Phoenix Mountain; the five-boxes chain (with 20) was dropped |  |
| 28 | 動かない恵みコゾウ | The Blessing Boy Who Won't Move | retail (renamed / reworked) | 27: The Broken Fountain (JP keeps 動かない恵みコゾウ) | evidence: retail AREA01 (Charity Square fountain statue); 〓 read as 恵 | confirmed: Unmoving Blessed Priest (title reused) |
| 29 | 大脱出 | The Great Escape | cut (slot reused) | 28: Familiar Looking Mansion (見た事あるやしき) | inferred: follows the Great Thief (22); retail "The Great Escape" is the English name of id 21, so this slot was freed and reused |  |
| 38 | 怪物?勝てるかな? | A Monster? Can I Win? | retail (same) | 37: Monster Hunt (怪物？勝てるかな？) | evidence: same JP title |  |
| 39 | 死神の実をつぶせ | Crush the Reaper's Fruit | retail (renamed / reworked) | 38: Death Fruit Juice (死神の実をつぶせ) | evidence: same JP title |  |
| 40 | 花をさかせよう! | Let's Make Flowers Bloom! | retail (renamed / reworked) | 39: Plant a Flower Garden (花をさかせよう！) | evidence: same JP title; PrePre 8 forest script has wilted flowers |  |
| 41 | お涙ちょうだい | Give Me Your Tears | retail (renamed / reworked) | 40: Tears from a Flower (お涙ちょうだい) | evidence: same JP title |  |
| 80 | 森の地下には何がある? | What's Under the Forest? | retail (same) | 79: What's Under the Forest? (森の地下には何がある？) | evidence: same JP title |  |
| 81 | 花の森 | The Flower Forest | retail (renamed / reworked) | 80: The 100 Flower Forest (花の森) | evidence: same JP title (pig bag of the forest) |  |
| 114 | バロンの力 | Baron's Power | retail (same) | 113: Baron's Strength (バロンの力！？) | evidence: retail AREA02 |  |
| 145 | 億年老人の願い | The 100 Million Year Old Man's Wish | retail (same) | 144: Million Year Old Wish (億年老人の願い) | evidence: same JP title; AREA02 s4 |  |
| 146 | うまった道を行く | Down the Buried Road | retail (renamed / reworked) | 145: Dig Like a Mole (うまった道を行く) | evidence: same JP title; AREA02 tunnels |  |
| 147 | 黒の占い師 | The Black Fortune Teller | retail (renamed / reworked) | 146: The Blue Fortune Teller (青の占い師) | evidence: slot; retail AREA02 s3 |  |

### Phoenix Mountain, the "Stormy Mountain" (AREA03)

10 titles: 9 in retail, 1 cut, 0 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 30 | 嵐の山の怪 | Mystery of the Stormy Mountain | retail (renamed / reworked) | 29: A Stormy Pig Bag (JP keeps 嵐の山の怪) | evidence: same JP title; retail AREA03 = Stormy Mountain |  |
| 31 | 大トリの山 | Big Bird Mountain | retail (same) | 30: Phoenix Mountain (大トリの山) | evidence: same JP title |  |
| 32 | コロガルゾォ〜 | It's Gonna Roll! | retail (renamed / reworked) | 31: Where did I come from? (どこからきたの？) | inferred: slot in the Phoenix Mountain run; retail 31 is the Koma Pig drop-in |  |
| 33 | 光の落ちるところ | Where the Light Falls | cut | 32: untitled in PAL; JP ラッパでミニミニ ("Miniature Making Trumpets"), not awarded | inferred: slot in the Phoenix Mountain run; JP hint for 32: where did the coin blow in from |  |
| 34 | いよっ!穴ほり名人! | Hey! The Master Digger! | retail (same) | 33: The Famous Digger (いよっ！穴ほり名人！) | evidence: same JP title |  |
| 35 | またコロガルゾォ〜 | It's Gonna Roll Again! | retail (renamed / reworked) | 34: Lava Caves (マグマのどうくつ) | inferred: slot; second of the rolling pair |  |
| 36 | 空の主 | Master of the Sky | retail (same) | 35: The Master of the Skies (空の主) | evidence: same JP title (old decoder read 窟) |  |
| 37 | そこ、どいて | Move Over, Please | retail (renamed / reworked) | 36: What's a Funga? (フンガのタイコ) | inferred: slot; a Funga blocks the way in retail |  |
| 83 | おなかペコペコ? | Hungry? | retail (same) | 82: I'm So Hungry (おなかペコペコ？) | evidence: same JP title |  |
| 130 | 風になりたい | I Want to Be the Wind | retail (renamed / reworked) | 129: Green Hidden Powers (かくされた力その３) | evidence: JP retail still has the string 風になりたい (AREA19 list, unused); Jewel of Wind |  |

### Haunted Mansion, the "Thieves' Mansion" (AREA04)

9 titles: 7 in retail, 2 cut, 3 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 42 | 笑いが止まらない | Can't Stop Laughing | retail (renamed / reworked) | 41: Smile! (笑ってごらん) | inferred: slot; the laugh/cry pair moved to the mansion's Laughing Room | likely: Laughing Forest / Laughing Fruit |
| 43 | 涙が止まらない | Can't Stop Crying | retail (renamed / reworked) | 42: Cry Baby (泣いてごらん) | inferred: slot; Crying Room of the mansion | likely: Crying Forest / Crying Fruit |
| 44 | 泣き笑いのドア | The Crying and Laughing Door | cut (slot reused) | 43: Can't Stop Crying (涙がとまらない) | inferred: slot; retail's laughing and crying doors are separate | confirmed: The Crying Door / The Laughing Door |
| 52 | 大きなドアのむこう側 | Beyond the Big Door | retail (renamed / reworked) | 51: A Small Key Hole! (ちいさなカギのドア) | inferred: slot; retail AREA04 door |  |
| 55 | 盗賊マンション | The Thieves' Mansion | retail (renamed / reworked) | 54: The Haunted Mansion (シーフマンシヨン) | evidence: JP retail "Thief Mansion" |  |
| 56 | あやしいトビラ | A Suspicious Door | retail (renamed / reworked) | 55: A Large Key Hole! (JP keeps あやしいトビラ) | evidence: same JP title |  |
| 57 | 大きなカギの絵? | A Picture of a Big Key? | retail (same) | 56: Painting of a Big Key? (大きなカギの絵？) | evidence: same JP title; 〓 read as 絵 |  |
| 58 | 1000年老人 | The 1,000 Year Old Man | cut (slot reused) | 57: Break the Magic Egg! (魔法タマゴをわろう！) | evidence: retail AREA04 s15 = the 1,000 Year Old Man's room |  |
| 59 | かくされた力 | Hidden Power | retail (renamed / reworked) | 58: Red Hidden Powers (かくされた力) | evidence: same JP title; reward of the 1,000 Year Old Man |  |

### Baccus Village and Baccus Lake (AREA05 / AREA08)

10 titles: 6 in retail, 4 cut, 1 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 45 | 赤の占い師 | The Red Fortune Teller | retail (same) | 44: The Red Fortune Teller (赤の占い師) | evidence: same JP title; retail AREA05 |  |
| 46 | APスロットマシーン | AP Slot Machine | cut | 45: untitled in PAL (JP ＡＰスロットマシーン), never triggered | evidence: JP titles it in the Baccus and dwarf lists; JP hint: the more AP, the better the item |  |
| 47 | 子ども?大人? | A Child? An Adult? | retail (renamed / reworked) | 46: Where's the Baby Mouse? (子ネズミどこだ？) | inferred: slot in the Baccus run | likely: Tiny Tomba / Minitta Mushroom (shrinking) |
| 48 | 集まったら持っといで… | Bring Them When You Have Them... | retail (renamed / reworked) | 47: Some Cheese Please (集まったら持っといで…) | evidence: same JP title |  |
| 49 | ポイントのジョー? | Joe of the Points? | cut | 48: untitled everywhere, never triggered | inferred: between Baccus events |  |
| 50 | 大人の飲み物 | A Drink for Grown-ups | retail (same) | 49: A Drink for Grownups (大人の飲み物) | evidence: same JP title |  |
| 51 | バッカスの湖 | Baccus Lake | retail (renamed / reworked) | 50: Road to Baccus Lake (湖への道) | evidence: retail AREA08 |  |
| 53 | 赤ちゃんブリッジ? | Baby Bridge? | cut | 52: untitled in PAL (JP 渡れなくなっちゃった…), never triggered | inferred: in the Baccus run; JP lists it in the Phoenix Mountain table; JP hint: no bridge, stuck |  |
| 54 | 魔ブタをさがせ! | Find the Evil Pig! | retail (renamed / reworked) | 53: The Mouse Pig Bag (もとの姿にもどりたい) | inferred: slot; first pig-bag slot |  |
| 116 | 花よりダンゴ | Dumplings over Flowers | cut (slot reused) | 115: A Safe Mushroom? (まともなキノコ) | inferred: proverb title; slot in AREA05/09 |  |

### Masakari Jungle (AREA10)

19 titles: 10 in retail, 9 cut, 1 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 60 | 長老のたのみごと? | The Elder's Request? | cut | 59: untitled in PAL (JP 族長のたのみごと？ "the chief's request"), never triggered | evidence: JP jungle list (X10) |  |
| 61 | コトバをおぼえよう!Ⅱ | Let's Learn the Language! II | cut | 60: untitled in PAL (JP コトバを吸いとろう！Ⅱ), never triggered | evidence: JP jungle list; Masakari language |  |
| 62 | マサカリ族のヒミツ | Secret of the Masakari Tribe | cut | 61: untitled in PAL (JP マサカリ族のヒミツ); read only by the cut Stonetown | evidence: JP jungle list |  |
| 63 | 物知りの木は知っている | The Wise Tree Knows | retail (same) | 62: Tree of Knowledge Knows (物知りの木は知っている) | evidence: retail AREA10 |  |
| 64 | ポンプ岩のナゾ | Riddle of the Pump Rocks | retail (same) | 63: The Pump Rocks (ポンプ岩のナゾ) | evidence: same JP title |  |
| 65 | おぼれちゃう!? | I'm Going to Drown!? | retail (renamed / reworked) | 64: A Refreshing Drink (バナナジュース) | inferred: retail AREA10 swim teacher |  |
| 91 | かくされた力その2 | Hidden Power, Part 2 | retail (renamed / reworked) | 90: Blue Hidden Powers (かくされた力その２) | evidence: same JP title; AREA10 s3 |  |
| 118 | トンネルはどこだ | Where's the Tunnel? | cut | 117: untitled everywhere | inferred: before the puddle titles; JP hint for 117: "where is the new road..." |  |
| 119 | 大きな水たまりを渡れ | Cross the Big Puddle | retail (renamed / reworked) | 118: The 10,000 Year Old Man (水たまりの万年老人) | evidence: JP retail: "the 10,000 Year Old Man of the puddle" |  |
| 120 | おいしい?万能魚 | Tasty? The Mighty Fish | retail (same) | 119: Mighty Fish Food (おいしい？万能魚) | evidence: same JP title; 〓 read as 能 |  |
| 124 | 水に沈んだ村 | The Sunken Village | retail (renamed / reworked) | 123: The Underwater Pig Bag (JP keeps 水に沈んだ村) | evidence: same JP title; AREA10 | likely: Water Temple |
| 125 | いにしえの村 | The Ancient Village | retail (renamed / reworked) | 124: Trick Village (からくり村) | evidence: retail AREA10 s3 |  |
| 127 | 万年老人の数字玉 | The 10,000 Year Old Man's Number Beads | retail (renamed / reworked) | 126: The 10 Math Beads (万年老人の数字玉) | evidence: same JP title |  |
| 128 | 体力を上げましょー | Let's Build Up Stamina | cut (slot reused) | 127: The 5 Golden Items (黄金の５大アイテム) | inferred: slot; AREA10 |  |
| 129 | 青の占い師 | The Blue Fortune Teller | cut (slot reused) | 128: Unbreakable Wire (丈夫なハリガネ) | inferred: retail Blue Fortune Teller is id 146 (PrePre 8's black one) |  |
| 139 | からくり?サァたいへん | A Contraption? Oh No! | cut | 138: untitled in PAL (JP つかまえてごらん), never triggered | evidence: JP jungle list |  |
| 140 | からくり砦の3姉妹 | The Three Sisters of the Trick Fort | cut | 139: untitled in PAL (JP ３姉妹のかくれんぼ), never triggered | evidence: JP jungle list; 〓 read as 砦 |  |
| 141 | 3姉妹への手紙 | A Letter to the Three Sisters | cut | 140: untitled in PAL (JP ３姉妹への手紙), never triggered | evidence: item 90 Letter |  |
| 151 | イカダを作ろう! | Let's Build a Raft! | retail (renamed / reworked) | 150: Let's Ride the Raft! (イカダにのろう！) | evidence: retail AREA10 s2 |  |

### Sea, mermaids and water (AREA06 and the water titles)

12 titles: 5 in retail, 7 cut, 4 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 121 | トビウオ アクロバット | Flying Fish Acrobatics | cut (slot reused) | 120: Let's Make Candy! (ドロップを作ろう！) | inferred: slot reused by the candy event |  |
| 122 | 歌を忘れた人魚 | The Mermaid Who Forgot Her Song | retail (renamed / reworked) | 121: Mermaid's Singing Rock (人魚の歌岩) | evidence: retail AREA06 mermaid | likely: The Broken-Up Mermaid Harp! |
| 123 | 海族の宝物 | Treasure of the Sea Folk | cut | 122: untitled everywhere; JP hint "find the pirates' treasure" | evidence: 海族 puns on 海賊 (pirates) |  |
| 126 | 水門をあけろ! | Open the Floodgate! | cut (slot reused) | 125: The Thief's Door (盗賊のトビラ) | inferred: water run | confirmed: Open the Water Gate |
| 134 | 銅色のメダルがほしい | I Want the Bronze Medal | retail (same) | 133: I Want a Bronze Medal | evidence: same JP title; AREA06 |  |
| 135 | 銀色のメダルがほしい | I Want the Silver Medal | retail (same) | 134: I Want a Silver Medal | evidence: same JP title |  |
| 136 | 金色のメダルがほしい | I Want the Gold Medal | retail (same) | 135: I Want a Gold Medal | evidence: same JP title |  |
| 148 | 水を止めなきゃ! | I Have to Stop the Water! | cut | 147: untitled everywhere | inferred: water run 148-150 | likely: Pour the Water In! |
| 150 | 海を渡る方法 | How to Cross the Sea | cut | 149: untitled everywhere | inferred: before the raft |  |
| 154 | 水の下には何がある? | What's Under the Water? | retail (same) | 153: What's Underwater? (水の下には何がある？) | evidence: same JP title; AREA04/10 mermaid diving |  |
| 155 | 人魚の王を救え | Save the Mermaid King | cut | 154: untitled everywhere | inferred: mermaid | likely: A Mermaid in the Water Tank! |
| 156 | 海底通路の行き先 | Where the Undersea Passage Leads | cut | 155: untitled everywhere | inferred: sea |  |

### Village of Civilization (AREA11 / AREA16 / AREA17 / AREA18)

23 titles: 7 in retail, 16 cut, 3 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 66 | ナミダのビンちょうだい | Give Me a Tear Bottle | retail (same) | 65: I Need a Tear Bottle (ナミダのビンちょうだい) | evidence: retail AREA11 (Bonsugee researcher) |  |
| 74 | 大停電 | The Big Blackout | retail (renamed / reworked) | 73: We Need Power... (キカイを動かすには…) | evidence: retail AREA16/17 power |  |
| 75 | 電気うなぎを捕まえろ! | Catch the Electric Eel! | cut | 74: untitled in PAL (JP 電気うなぎを捕まえろ！), never triggered | evidence: item 108 Electric Eel; JP lists 10/17/18 |  |
| 76 | 事件発生、犯人を探せ! | Incident! Find the Culprit! | cut | 75: untitled everywhere, never triggered | inferred: in the Civilization run; JP hint for 75: find the stolen Evil Pig Bag |  |
| 77 | 黒い水の不思議 | The Mystery of the Black Water | cut | 76: PAL title #90 "The Black Water", started only in the cut Iron Town | evidence: item 109 Black Water; JP hint: a thing with dangerous power |  |
| 78 | 文明のキカイ | The Civilization Machine | cut | 77: The Civilization Machine (文明のキカイ), never set | evidence: read only by the cut towns; JP hint: the revived machine |  |
| 79 | 進め!きかんしゃ | Go, Locomotive! | cut (slot reused) | 78: Find Charles! (サンタを探せ！) | inferred: in the Civilization run; slot reused in the jungle | likely: Quick! The Trolley! / Turbo Trolley |
| 90 | 緑の占い師 | The Green Fortune Teller | cut | 89: untitled in PAL (JP 緑の占い師), never triggered | evidence: cut Wood village line about a fortune-teller's hut |  |
| 95 | 仲良くしようよ | Let's Get Along | cut | 94: untitled in PAL (JP 仲良くしようよ), never triggered | evidence: JP lists 11/16/18 |  |
| 96 | 木人の村の大岩 | The Big Rock of the Wood People's Village | cut | 95: untitled in PAL (JP same), never triggered | evidence: Wood People = AREA11 |  |
| 97 | こわれた橋… | The Broken Bridge... | cut | 96: untitled in PAL (JP 未完の橋…), never triggered | evidence: JP AREA18 list |  |
| 98 | いま何時? | What Time Is It? | cut | 97: untitled in PAL (JP 動かない時計台); completed by the Clock Tower gears | evidence: Stonetown clock tower |  |
| 99 | こわれない歯車? | An Unbreakable Gear? | cut | 98: untitled in PAL (JP こわれない歯車？), started at the gears | evidence: same JP title | likely: gear wheels (round / triangle / hexagon) |
| 100 | 材料はどこだ!? | Where Are the Materials!? | cut | 99: untitled in PAL (JP 材料はどこだ！？), started in the cut Iron Town | evidence: same JP title |  |
| 101 | 大きなカギ穴 | The Big Keyhole | cut | 100: untitled in PAL (JP 鉄のカギ穴), never triggered | evidence: JP lists 11/18 |  |
| 102 | どうやって取るの? | How Do I Get It? | cut | 101: untitled everywhere | inferred: in the Civilization run |  |
| 103 | さびたトビラをこわせ! | Break the Rusty Door! | retail (same) | 102: Break the Rusty Door! (さびたトビラをこわせ！) | evidence: retail AREA17 Iron Castle |  |
| 104 | 動かない鉄の人形 | The Iron Doll That Won't Move | cut (slot reused) | 103: The Cute Witch (かわいい？魔女ミズノ) | inferred: slot; retail AREA18 |  |
| 105 | エサ燃料って何? | What's Feed Fuel? | retail (same) | 104: Food for Fuel? (エサ燃料って何？) | evidence: same JP title; 〓 read as 燃 | likely: The Fuel that Burns Well |
| 106 | あぶない届け物 | A Dangerous Delivery | retail (renamed / reworked) | 105: I Need a Bomb (バクダンの作り方) | inferred: slot; the bomb for the Iron Castle door |  |
| 115 | 魔女の探し物 | What the Witch Is Looking For | retail (renamed / reworked) | 114: What the Witch Lost... (魔女の探し物) | evidence: same JP title; retail AREA18 Mizuno |  |
| 117 | 道具が強くなる!? | Tools Get Stronger!? | retail (same) | 116: Power Up for Tools?! (道具が強くなる！？) | evidence: same JP title (old decoder read 道空); AREA10/18 |  |
| 133 | 世紀の大発明 | The Invention of the Century | cut (slot reused) | 132: Take Two of These (魔女がカゼひいた…) | inferred: slot; retail AREA18 witch's cold |  |

### The island secrets (no retail area)

7 titles: 0 in retail, 7 cut, 0 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 67 | 盗まれた島の絵本 | The Stolen Island Picture Book | cut | 66: untitled everywhere, never triggered | inferred: opens the island run 67-73; 〓 read as 絵 |  |
| 68 | 解読?らくがき岩 | Decode the Graffiti Rock? | cut | 67: untitled in PAL (JP 解読？らくがき岩), never triggered | evidence: graffiti plaques exist in AREA01/AREA04 (docs/CUT_CONTENT.md); JP hint: decode the letters of the graffiti rock |  |
| 69 | 島のヒミツその1 | Secret of the Island, Part 1 | cut | 68: untitled everywhere | inferred: island chain |  |
| 70 | 島のヒミツその2 | Secret of the Island, Part 2 | cut | 69: untitled everywhere | inferred: island chain |  |
| 71 | 島のヒミツその3 | Secret of the Island, Part 3 | cut | 70: untitled everywhere | inferred: island chain |  |
| 72 | 島のヒミツその4 | Secret of the Island, Part 4 | cut | 71: untitled everywhere | inferred: island chain |  |
| 73 | 島のヒミツその5 | Secret of the Island, Part 5 | cut | 72: untitled everywhere | inferred: island chain |  |

### Unplaced

2 titles: 0 in retail, 2 cut, 1 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 82 | かんだら割れちゃうゾ | It'll Crack If You Bite It | cut (slot reused) | 81: The Boss' Treasure (カシラの大事なもの) | inferred: slot; theme unclear |  |
| 84 | お城の食事会 | The Castle Banquet | cut | 83: PAL #126 "The Rusty Steel Car" (JP 動かない鉄の車), never triggered | evidence: Dengeki D5 hint for 83 is still "a dinner party has a lot of feast" | likely: Cooking with Tomba! / The Cook's Special Menu |

### The cold / snow area (no retail area)

9 titles: 0 in retail, 9 cut, 6 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 85 | 光がほしい | I Want Light | cut | 84: untitled everywhere | inferred: start of the cold run 85-89 | likely: Too Dark to See! |
| 86 | おひさまポカポカ… | The Sun Is Nice and Warm... | cut (slot reused) | 85: The Deep Jungle Pig (ジャングル) | inferred: cold run |  |
| 87 | 寒い!裸? | Cold! Naked? | cut (slot reused) | 86: Healing Herbs for Baron (ケガに効く ナオリ草) | inferred: cold run; 〓 read as 裸 |  |
| 88 | 溶かす、溶ければ… | Melt It, and If It Melts... | cut (slot reused) | 87: Delicious Knowledge Fruit (おいしい百識の木の実) | inferred: cold run | likely: Melt the Giant Ice! |
| 89 | 雪玉ゴロゴロその先は… | Where the Rolling Snowball Goes... | cut (slot reused) | 88: Seaweed for Your Health (海草でみんな元気) | inferred: cold run | likely: Collect Snow (Kujara Ranch) |
| 113 | 固める、止まった時に… | Harden It, When Time Stops... | cut (slot reused) | 112: The Mermaid's Necklace (人魚の思い出) | inferred: pairs with 88 (melt / harden) | likely: Sea Anemone's Ice |
| 142 | 雪がふったら… | When the Snow Falls... | cut | 141: untitled everywhere | inferred: second cold run 142-144 | likely: Snow Fireflies (Kujara Ranch) |
| 143 | 毒毒どうくつのナゾ | Riddle of the Poison Cave | cut | 142: untitled everywhere | inferred: 〓〓 now read as 毒毒 (毒々) |  |
| 144 | 水晶のどうくつ | The Crystal Cave | cut | 143: untitled everywhere | inferred: same run; JP hint for 143 is the Civilization reunion ("the magic fades and the three towns become one") | likely: Crystal Panel Blocks a Hole! |

### Pig Town (no retail area)

8 titles: 0 in retail, 8 cut, 5 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 92 | ブタの町 | Pig Town | cut | 91: untitled everywhere | inferred: first of the pig-town titles; JP hint for 91 is unrelated ("so light it floats up") | likely: Pig Tribe / Clown Circus |
| 93 | 白いケムリの家 | The House of White Smoke | cut | 92: untitled everywhere | inferred: after Pig Town |  |
| 94 | コトバをおぼえよう!Ⅲ | Let's Learn the Language! III | cut | 93: untitled everywhere | inferred: after Pig Town; I and II were dwarf and Masakari | likely: Use Pig Suit to Talk! (pig language) |
| 107 | 森の案内人 | The Forest Guide | cut | 106: untitled everywhere | inferred: run 107-111 ends with Pig Town titles; JP hint for 106: "nobody beats me in a footrace!" |  |
| 108 | うまい果実、まずい果実 | Tasty Fruit, Nasty Fruit | cut | 107: untitled everywhere | inferred: same run | likely: Normal Fruit / Strange Fruit |
| 109 | 挑戦!勇気の大樹 | Challenge! The Great Tree of Courage | cut | 108: untitled everywhere | inferred: same run; 〓 read as 勇 | likely: Mythical Tower of Courage |
| 110 | ブタの町のブタメガネ | The Pig Glasses of Pig Town | cut | 109: untitled everywhere | evidence: names Pig Town; JP hint for 109: "glasses that see through anything" | likely: Pig Nose Panel |
| 111 | 王の本性 | The King's True Nature | cut | 110: untitled everywhere | inferred: after the Pig Town glasses |  |

### Sky, towers and light (no retail area)

7 titles: 0 in retail, 7 cut, 3 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 131 | 危ない!落ちたら死ぬぞ | Careful! Fall and You Die | cut | 130: untitled everywhere | inferred: after the wind title; JP hint for 130: "make up your mind and cross!" |  |
| 132 | ほこらの中に何がある? | What's in the Shrine? | cut | 131: untitled everywhere | inferred: same run |  |
| 137 | おもいは地、かるいは天 | Heavy to the Earth, Light to the Sky | cut | 136: untitled everywhere | inferred: weight puzzle; JP hint for 136: "weight decides the path?" | likely: Use the Seesaw! / Rock Crabs for Balance! |
| 138 | 雲の中身 | What's Inside the Cloud | cut | 137: untitled everywhere | inferred: 〓 read as 雲; JP hint for 137: "an important person in the clouds!?" |  |
| 149 | 塔のもりびと | The Tower Keeper | cut | 148: untitled everywhere | inferred: tower | likely: Mythical Towers |
| 152 | 光と影 | Light and Shadow | cut (slot reused) | 151: Take Out (息子よ、どこに…) | inferred: slot | likely: Find Another Reflector! |
| 153 | 光る石を探せ | Find the Glowing Stone | cut | 152: untitled everywhere | inferred: after light and shadow |  |

### Endgame: Dwarf Village underground, Evil Pigs, endings (AREA02 s3-s4, AREA14)

12 titles: 6 in retail, 6 cut, 3 with a Tomba! 2 link.

| # | Japanese | English | Status | Retail id: title (JP retail) | Area basis | Tomba! 2 |
|---|---|---|---|---|---|---|
| 157 | 魔力のみなもと? | The Source of the Magic? | retail (same) | 156: Source of Evil Magic (魔力のみなもと？) | evidence: same JP title; AREA02 s4 |  |
| 158 | 友達10人 | Ten Friends | retail (renamed / reworked) | 157: Seven Friends (友達７人) | evidence: ten became seven | likely: Proof of friendship items |
| 159 | 金色のカイロウ | The Golden Corridor | cut | 158: untitled everywhere | inferred: endgame run | likely: Golden Tower |
| 160 | 悪いヤツは誰だ? | Who's the Bad Guy? | retail (renamed / reworked) | 159: The 8th Evil Pig Bag? (８つめの封印ぶくろ？) | inferred: slot |  |
| 161 | 本当の魔ブタ | The Real Evil Pig | retail (same) | 160: The Real Evil Pig (本当の魔ブタ) | evidence: same JP title |  |
| 162 | 夢?幻?黄金郷を探せ | Dream? Illusion? Find El Dorado | retail (renamed / reworked) | 161: Underground Treasure (夢？幻？宝物庫を探せ) | evidence: JP retail: 黄金郷 → 宝物庫 (treasury) | likely: Golden Tower |
| 163 | らくがき床のナゾ | Riddle of the Graffiti Floor | cut | 162: untitled everywhere | inferred: tunnel glyph sheet AREA02/B301 (docs/CUT_CONTENT.md) |  |
| 164 | かくされた入口 | The Hidden Entrance | cut | 163: untitled everywhere | inferred: endgame run |  |
| 165 | ミリオンの塔 | The Million Tower | retail (same) | 164: The Flower Tower (ミリオンの塔) | evidence: same JP title |  |
| 166 | 「大団円1」 | "Grand Finale 1" | cut | 165: untitled everywhere | inferred: ending placeholders; JP hint for 165: "before you know it, everyone is happy!" |  |
| 167 | 「大団円2」 | "Grand Finale 2" | cut (slot reused) | 166: A Hungry Monkey (サンタの好きなもの…) | inferred: retail 166+ are later area-0 additions |  |
| 168 | 「大団円3」 | "Grand Finale 3" | cut (slot reused) | 167: Peach Flower Gas (小さなブタ…マゴブタ？) | inferred: same |  |

## Tomba! 2

Checked directly: the Tomba! 2 USA disc (SCUS-94454) has its event, item and place names as plain ASCII in
`MAIN.EXE`. No JP Tomba! 2 disc was available, so the matches compare English T2 titles with Japanese T1 titles.
**confirmed** = the T2 title says the same thing; **likely** = the same theme or mechanic.

| PrePre 8 | T2 (USA) | Match | Notes |
|---|---|---|---|
| 8 かくれ里のヤン | "Yan of the Hidden Village" (event) and "Hidden Village" (place) | **confirmed** | T1 retail renamed it Hide and Go Seek in English; JP T1 kept the title |
| 28 動かない恵みコゾウ | "Unmoving Blessed Priest" | **confirmed** | also T1 retail id 27 (The Broken Fountain); T2 reuses the title |
| 44 泣き笑いのドア | "The Crying Door", "The Laughing Door"; Laughing / Crying Forest, Laughing / Crying Fruit | **confirmed** | T1 retail split it into Smile! / Cry Baby rooms |
| 126 水門をあけろ! | "Open the Water Gate" | **confirmed** | retail T1 reused the id for The Thief's Door |
| 85, 88, 89, 113, 142, 144 (cold area) | Kujara Ranch: "Collect Snow", "Collect Snow Fireflies", Evil Ice Pig, "Melt the Giant Ice!", "Sea Anemone's Ice" / "Burning Dregs" | likely | 88 溶かす vs "Melt the Giant Ice!" is the closest pair |
| 85 光がほしい, 152 光と影 | "Too Dark to See!", "Find Another Reflector!", Mystery Reflector | likely | |
| 92, 94, 110 (Pig Town) | Pig Tribe, "A Pig Tribe Clown Statue!", Clown Circus, "Use Pig Suit to Talk!", Pig Nose Panel | likely | Language III = pig language |
| 109 挑戦!勇気の大樹, 149 塔のもりびと | Mythical Towers of Wisdom / Strength / Courage | likely | |
| 79 進め!きかんしゃ | "Quick! The Trolley!", Turbo Trolley, Coal-Mining Town | likely | |
| 105 エサ燃料って何? | "The Fuel that Burns Well" (Power Coal) | likely | also T1 retail (Food for Fuel?) |
| 99 こわれない歯車? | round / triangle / hexagon gear wheels, "Wind it Up!" | likely | also a T1 Civilization event |
| 84 お城の食事会 | "Cooking with Tomba!", "The Cook's Special Menu" | likely (weak) | |
| 122 歌を忘れた人魚, 155 人魚の王を救え | "The Broken-Up Mermaid Harp!", "A Mermaid in the Water Tank!", Water Temple | likely | |
| 124 水に沈んだ村 | Water Temple | likely (weak) | |
| 47 子ども?大人? | Tiny Tomba / Mini Tomba, Minitta Mushroom | likely (weak) | shrinking |
| 108 うまい果実、まずい果実 | Normal Fruit / Strange Fruit, Laughing / Crying Fruit | likely (weak) | |
| 137 おもいは地、かるいは天 | "Use the Seesaw!", "Use Rock Crabs for Balance!" | likely (weak) | weight puzzle |
| 144 水晶のどうくつ | "Crystal Panel Blocks a Hole!", Large Mine | likely (weak) | |
| 158 友達10人 | "Proof of friendship" items from the 100-Year-Old Man, Yan, Mizuno, the Dwarf | likely (weak) | |
| 159 金色のカイロウ, 162 黄金郷 | Golden Tower, Golden Sun / Star / Moon | likely (weak) | |
| 24 / 114 Baron | "Baron Turned to Stone" | T1 character reused | also T1 retail |
| Candies | "Collect Ice Candy" (item Ice Candy, "a carnival favorite"), "Witch's Concoction" | likely | see Candies |

T1 retail events that T2 also reuses by name (not PrePre 8 specific): "Lost and Found", "Where's My Son?" (T1 id 151's JP
title 息子よ、どこに…). T2 also brings back T1's Mizuno, Yan, the 100-Year-Old Man, the dwarves, Baron and Charles.

## Candies

Retail item names are PAL English (D0SPSUB) and JP retail; JP descriptions are `DSPSUB.BIN` block 0 entry id + 4.
Use effects are the item-use switch, `src/d0spsub/func_800E89F8.c` (PAL) and `func_800EA2BC_slps01144.c` (JP, the same
cases). "Given" means an `addItemToInventory` with that id exists (CUT_CONTENT.md: 110-114 are never given).

| Id | JP name | English | Obtainable | Use effect / purpose | JP description |
|---|---|---|---|---|---|
| 110 (0x6E) | 赤のドロップ | Red Candy | no | none (no use case) | たべてみると わかるハズ ("you'll know when you eat it") |
| 111 (0x6F) | 青のドロップ | Blue Candy | no | +4 vitality, consumed | same |
| 112 (0x70) | 緑のドロップ | Green Candy | no | +8 vitality, consumed (the same code as the Lunch Box) | same |
| 113 (0x71) | 黒のドロップ | Black Candy | no | full vitality, consumed (the same code as the Large Lunch Box) | same |
| 114 (0x72) | 銀のドロップ | Silver Candy | no | none | same |
| 115 (0x73) | 金のドロップ | Gold Candy | **yes** | made at witch Mizuno's cauldron (event 120 Let's Make Candy!, X018 `witchMizunoCandyRecipeTalk`): placing the six ingredients completes 120 and gives one | ５大アイテムのひとつ ("one of the five great items", i.e. The 5 Golden Items, event 127) |

The recipe (MFNT180 3/15-22, PAL text "To make candy you need..."; JP 「ドロップをつくるには つぎの材料が いるんだからね！！」):

| Ingredient | Id | JP | Notes |
|---|---|---|---|
| Biting Plant Flower | 8 | かみつき草の花 | handing it over completes event 169 (PAL/USA); JP retail and the beta complete 169 when the item is *used* (notes/secrets.md) |
| Silver Powder | 102 (0x66) | 銀の粉 | |
| Molasses | 103 (0x67) | 黒みつ | |
| Kokka Claw | 104 (0x68) | コッカのツメ | |
| Butamushi Thorn | 105 (0x69) | ブタムシのトゲ | |
| Needlegator Teeth | 106 (0x6A) | ハリゲーターのキバ | "the secret ingredient" |

Differences between builds:
- **JP retail** = PAL for the candies: same ids, the same three use cases, the same six ingredients in the dialogue.
  But the JP hint for event 120 says 「３つのアイテムを調合して 魔法の ドロップをつくろう！」 ("combine **three**
  items to make a magic candy"): an earlier, smaller recipe survives in the hint.
- **PrePre 8** has no candy at all: its item table has six names (ヒヨコ, カエル, ドワーフの迷子, バナナ, たつまき,
  花の〓) and the id of Let's Make Candy! (120) was then 121 トビウオ アクロバット ("flying fish acrobatics"). The
  witch is already there (115 魔女の探し物 = retail 114 What the Witch Lost...).
- **Beta 980203, USA, the other demos**: not checked for candies on this pass (the beta's item-use switch is the
  retail one per notes/secrets.md, apart from event 169).
- The four coloured candies share their colours with PrePre 8's four fortune tellers (赤 45, 緑 90, 青 129, 黒 147),
  and silver/gold with the medals. This is an observation, not evidence of a link.
- Tomba! 2 has "Collect Ice Candy" and an Ice Candy item ("a carnival favorite"); its lunch boxes restore 4 / 8 / 15
  vitality, the same 4 and 8 steps as T1's blue and green candies.

## Cross-check with Folkin's Pure 8 event list

Folkin's list has the same 169 titles in the same order. The translations agree almost everywhere; his readings of
the glyphs that were unread here (clouds 雲, poison 毒, fort 砦, naked 裸, courage 勇) match the new decode. His "same
spot as retail event X" notes match the id rule above in nearly every case. Where the evidence here differs:

| # | Folkin's list | Evidence here |
|---|---|---|
| 29 大脱出 | "The Great Escape", exactly the retail event | Retail "The Great Escape" is id 21, whose JP retail title is still 地下ロウの大ドロボウ (PrePre 8 22, the Great Thief). Index 29's id 28 is Familiar Looking Mansion. So the Great Thief event was renamed The Great Escape, and PrePre 8's own 大脱出 was dropped |
| 30 嵐の山の怪 | maybe "To Phoenix Mountain..." | JP retail id 29 keeps 嵐の山の怪; in English it is "A Stormy Pig Bag". To Phoenix Mountain is id 26, the slot of 27 5つの箱のカギ |
| 7 大きな水たまり? | maybe Ol' Pond | Agreed, with a caveat: PrePre 8's own area-0 dialogue ("can you swim?", next to ふるいけ) and Hyper 12's AREA00 title block place it in area 0; JP retail later lists id 6 in the jungle (X10), where the 10,000 Year Old Man "of the puddle" lives |
| 112 始まりの村 | (title only) | Id 111 became Baccus Village (バッカスの村): the slot kept "village", not area 0 |
| 33 光の落ちるところ | the retail leftover is "Miniature Making Trumpets" | Agreed; the JP retail title is ラッパでミニミニ and the JP hint is about a coin blown in by the wind |
| 129 青の占い師 / 147 黒の占い師 | 147 is a new, black fortune teller | Agreed that black is new; retail's Blue Fortune Teller is id 146, i.e. it took the black one's slot, and the blue one's old id 128 became Unbreakable Wire |
| 98 いま何時? .. 101 | (titles only) | JP retail titles ids 97-100 as 動かない時計台, こわれない歯車？, 材料はどこだ！？, 鉄のカギ穴: the Stonetown clock tower and the Iron Town chain |
| 76 事件発生、犯人を探せ! | find the stolen Green Evil Pig Bag | The JP hint of id 75 is "find the stolen Evil Pig Bag"; the colour and the boulder are not checked |

## Uncertain

- Every "inferred" area. The snow area, Pig Town, the island and the sky group have no retail area and no data;
  they rest on the titles and their order.
- The five kanji read from their slot alone (雲, 砦, 毒, 勇, 裸): plausible readings, not confirmed by a retail title.
- *retail (renamed / reworked)* rows marked inferred (e.g. 32/35 the rolling titles -> Where did I come from? / Lava
  Caves, 37 そこ、どいて -> What's a Funga?): the id matches, the goal is a guess.
- The JP title tables of AREA01 and AREA18 were read through the AREA13 and AREA11 lists (tools/jp_text.py skips the
  AREA01/A008 and AREA18/A003 title blocks); where those lists differ, the JP title shown was picked by hand.
- Tomba! 2: English USA titles only; "likely" matches are themes, not proof that the content moved.
