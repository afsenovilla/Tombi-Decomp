# PrePre 8 ("Pure 8") event titles: areas, retail fate, Tomba! 2 and the candies

PrePre Vol. 8 (PCPX-96084, June 1997; the community calls it "Pure 8") has the oldest event-title list of any Tomba
build. This page places every title in an area where data supports it, rates that evidence, says what became of
the title in retail, flags the titles that resemble Tomba! 2 content, and lists every candy known in the Tomba! 1 builds. Background on the build:
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

Browsable version, one page per area with a section per event (descriptions, neighbours, links):
[wiki: PrePre 8 events](wiki/PrePre8-Events.md), generated from this page by `python3 tools/prepre8_wiki.py --check`
(re-run it after editing the tables here).

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
  secrets.md showed it for those). 69 titles are identical, or identical but for spelling and punctuation, to the JP retail title of
  that id (e.g. 30 嵐の山の怪 = retail 29, 127 万年老人の数字玉 = retail 126, 145 億年老人の願い = retail 144); only two
  match a retail title at another id: 43 涙が止まらない is retail 43 (one later) and 129 青の占い師 is retail 146 (the
  blue fortune teller moved). The Dengeki D5 hint for id 83 is still the PrePre 8 title 84 お城の食事会 ("a dinner party has a lot of feast"). So the
  PrePre 8 list is the retail event table in an earlier state. A cut title is an id that retail either left untitled
  (most of the 0xFF events of notes/secrets.md), or reused for a different event. Retail ids 166 and up are later
  additions (Hungry Monkey, Peach Flower Gas, the pig bags...), appended after the old list.
- **JP hint bank.** Retail JP `DSPSUB.BIN` block 1 holds one hint per id, also for most cut ids. Where it has one, it
  is quoted in the tables; most fit the PrePre 8 title (106 "nobody beats me in a footrace", 109 "glasses that see
  through anything" for 110 ブタの町のブタメガネ, 117 "where is the new road" for 118 トンネルはどこだ, 136 "weight decides
  the path?" for 137 おもいは地、かるいは天, 137 "an important person in the clouds" for 138 雲の中身, 165 "before you know
  it, everyone is happy" for 166 「大団円1」). Two do not: 91 (Pig Town) has "so light it floats up", and 143 (the
  Crystal Cave) has the Civilization reunion "the magic fades and the three towns become one".
- **Descriptions.** PrePre 8 has no event descriptions: a scan of TOMBA.EXE and every file of the disc (GAMs
  unpacked) for 5-byte glyph tables followed by u16 strings ending in 0xFFFE finds only the six known blocks (the
  title table and the five dialogue tables). Nor does PrePre 9 (its `DSPSUB.BIN` holds no text block) or Hyper 12.
  So the "Description (source)" column gives, for each title, what each build says, best source first: **Pure 8
  dialogue** (PrePre 8's own dialogue about the event, translated; the forest bank's unlabelled kanji read from the
  same lines in the JP demo's early forest bank `AREA08/A003`), the 1997 demo hint banks **JP demo** / **Yoiko 2**
  (`SYSTEM/A00014` / `A00013`, 1997-10-10) and **Dengeki D5** (`SYSTEM/A00014`, 1997-11-06), translated and listed
  only where they differ from JP retail, **beta 980203** (the prototype's English, where its wording differs),
  **retail EN** (the PAL English hint of the retail id, `D0SPSUB.BIN`), **JP hint (translated)** (JP retail
  `DSPSUB.BIN` where the PAL English is a placeholder), or **none**. Where retail reused the id, its retail hint is
  for the other event and is marked so. Dengeki's hints differ from retail for 15 of these ids, most of them cut
  titles whose early hint echoes the PrePre 8 title (29 "the door has shut and you cannot get out", 82 "careful not
  to bite!", 84 "a dinner party with lots of feast", 88 "melt it and a path appears", 89 "become a snowball and roll
  down", 122 "the thing she forgot is a memory?").
- **Fate.** *retail (same)*: the same event, same or near-same JP title (the English may differ). *retail
  (renamed)*: the same event under another title: the JP title differs, but the goal is the same (the language
  lesson, a place-named pig bag, ten friends that became seven...). *retail, other id*: the same title at another id.
  *cut*: no playable event for it in retail; where retail reused the id for another event, the row says so.

## Evidence levels

Each title's area is rated by what supports it, with the argument in the "Argument" column:

- **evidence** (87): data places it. PrePre 8 itself (its code constants, the area 0 room 0 record, the area-0 and
  Dwarf Village dialogue banks); a demo's area title block (Hyper 12, the JP demo and Dengeki have the same area-0
  block: ids 0-12, 166, 167; PrePre 9's AREA01 / AREA02 and the JP demo's AREA08 have the same 30-title forest block);
  a JP retail per-area title table that holds the same title (see "The JP per-area title tables" for how far they are
  per area); or a retail event with the same JP title (near-same: one word or a number changed) that retail sets in
  that area (`docs/EVENTS_MAP.md`). Two titles name a place (51 Baccus Lake, 112 the Village of All Beginnings) and
  two name Pig Town.
- **strong** (19): the same retail id is used in that area for an event with a matching theme, but under another
  title and with no pre-release data.
- **weak** (16): list position only: the nearest titles on both sides with evidence or strong are of one area and at
  most 4 indexes apart. On the retail-matched titles this guess is right 16 times in 21 (see below).
- **unknown** (47): theme only, or nothing. These are under "Area unknown", or in a title cluster when consecutive
  titles share a theme (a cluster is not an area).

## Is the list ordered by area?

Grouped, not sorted. Measured on the 69 titles that are the same event in retail at id = index - 1 and whose id has
call sites in one region only (`docs/EVENTS_MAP.md`; the pig bags counted where the bag is, not at the AREA14 boss;
AREA01/02/19 count as one region, as do AREA05/08 and AREA11/16/17/18), 11 regions in all
(script: the scratch `order3.py` of this audit, not in the repo):

| Measure | Result |
|---|---|
| neighbouring matched titles in the same region | 35 of 68 (51%); a shuffled list gives 14% |
| runs of one region | 34; a list sorted by area would have 11 |
| regions that come back after another | the forest / Dwarf Village 7 times (14-28, 40, 81-83, 114, 145-146, 157-165), the village, the mansion and the jungle 3 times each |
| sandwich test: a title between two titles of one region (at most 4 apart) is in that region | 16 of 21 (76%) |
| the same on all retail ids 0..167 with a one-region call site | consecutive ids share the region 33 of 70 times (47%); sandwich 26 of 40 (65%) |

So the start follows the story (village 1-13, forest 14-28, Phoenix Mountain 30-36), but from about 37 on the areas
mix and come back, the way retail ids 166+ were appended for area 0. Position is a hint, right about three times in four between two close titles of one area, and that is all
the "weak" level claims. The misses include 5 Motocross Course and 17 AP Gate, events that retail moved (the demos'
area-0 block and PrePre 8's dwarf dialogue place them where the list does).

## The JP per-area title tables

JP retail has one event-to-title table per area (`tools/event_titles.py`). They are not strictly per area. Checked
against the call-site regions of `docs/EVENTS_MAP.md`:

| Tables | Entries whose id has a call site in the table's region |
|---|---|
| early areas X00, X03, X04, X05, X06, X08, X09, X10 | 85 of 115 (74%) |
| forest X01, X02 | 53 of 73 (73%) |
| Civilization X11, X16, X17, X18 | 30 of 69 (43%); 26 of 32 without the forest list they copy |
| X13, X14, X19 | 4 of 65: X13 and X14 are copies of the forest table, X19 has 4 entries |

The Civilization tables (and X13 / X14) start with a copy of the forest list (ids 18-27, 37, 38, 80, 168-182), so a
low id in those tables says nothing. Elsewhere about three entries in four are in the table's area, and the misses
are mostly events of two areas (a door between Phoenix Mountain and the mansion, the hidden son of 151). A title in
the table of one region only counts as evidence here; titles in tables of two regions go under "Data for more than one
area".

## Overview

| Group | Titles | In retail | Cut | of which a Tomba! 2 idea | evidence | strong | weak | unknown |
|---|---|---|---|---|---|---|---|---|
| Village of All Beginnings (AREA00) | 14 | 12 | 2 | 0 | 14 | 0 | 0 | 0 |
| Forest of 100 Flowers and Dwarf Village (AREA01 / AREA02 / AREA19) | 33 | 25 | 8 | 1 | 22 | 5 | 6 | 0 |
| Phoenix Mountain (AREA03) | 10 | 5 | 5 | 0 | 5 | 2 | 3 | 0 |
| Haunted Mansion (AREA04) | 6 | 5 | 1 | 0 | 4 | 2 | 0 | 0 |
| Baccus Village and Baccus Lake (AREA05 / AREA08) | 10 | 7 | 3 | 2 | 5 | 2 | 3 | 0 |
| Mushroom Forest (AREA09) | 2 | 2 | 0 | 0 | 2 | 0 | 0 | 0 |
| Mermaid's Singing Rock (AREA06) | 4 | 4 | 0 | 0 | 3 | 1 | 0 | 0 |
| Masakari Jungle (AREA10) | 15 | 8 | 7 | 1 | 10 | 4 | 1 | 0 |
| Village of Civilization towns (AREA11 / AREA16 / AREA17 / AREA18) | 19 | 6 | 13 | 0 | 13 | 3 | 3 | 0 |
| Evil Pigs (AREA14) | 1 | 1 | 0 | 0 | 1 | 0 | 0 | 0 |
| Pig Town: a place two titles name (no retail area) | 2 | 0 | 2 | 2 | 2 | 0 | 0 | 0 |
| Title cluster: the island (67, 69-73) | 6 | 0 | 6 | 0 | 0 | 0 | 0 | 6 |
| Title cluster: light, sun and cold (85-89) | 5 | 0 | 5 | 3 | 0 | 0 | 0 | 5 |
| Title cluster: sky (137-138) | 2 | 0 | 2 | 1 | 0 | 0 | 0 | 2 |
| Title cluster: caves (143-144) | 2 | 0 | 2 | 1 | 0 | 0 | 0 | 2 |
| Title cluster: light (152-153) | 2 | 0 | 2 | 1 | 0 | 0 | 0 | 2 |
| Title cluster: grand finale (166-168) | 3 | 0 | 3 | 0 | 0 | 0 | 0 | 3 |
| Data for more than one area | 6 | 3 | 3 | 0 | 6 | 0 | 0 | 0 |
| Area unknown | 27 | 0 | 27 | 10 | 0 | 0 | 0 | 27 |
| **All** | **169** | **78** | **91** | **22** | **87** | **19** | **16** | **47** |

By fate: 61 same, 15 renamed, 2 with the same title at another id, 91 cut (28 of them with the id reused by another
retail event). Tomba! 2: 4 titles share a name with a Tomba! 2 event (8, 28, 44, 126), 21 share a theme only.
Best description per title (first in the "Description (source)" column, the beta's English draft not counted):
retail EN 83, none 49, Pure 8 dialogue 18, Dengeki D5 14, JP hint (translated) 4, JP demo 1.

A poster of all 169 events by area (PNG, dark, text only) and a bar chart of these counts were made for Discord; they
are not in the repo.

Status grid, in list order (R = in retail, x = cut; then the group code of the tables below and the level, e / s / w / u): V0 Village of All Beginnings, FD Forest of 100 Flowers and Dwarf Village, PM Phoenix Mountain, HM Haunted Mansion, BV Baccus Village and Baccus Lake, MF Mushroom Forest, MR Mermaid's Singing Rock, MJ Masakari Jungle, CV Village of Civilization towns, EP Evil Pigs, PT Pig Town, KI cluster: island, KC cluster: light, sun, cold, KS cluster: sky, KV cluster: caves, KL cluster: light, KE cluster: grand finale, MU more than one area, UN area unknown.

| # | +0 | +1 | +2 | +3 | +4 | +5 | +6 | +7 | +8 | +9 |
|---|---|---|---|---|---|---|---|---|---|---|
| 0 | x UN u | R V0 e | R V0 e | R V0 e | R V0 e | R V0 e | R V0 e | x V0 e | R V0 e | R V0 e |
| 10 | R V0 e | R V0 e | R V0 e | R V0 e | R FD s | R FD e | R FD e | R FD e | R FD e | x FD e |
| 20 | R FD s | x FD w | R FD e | R FD e | R FD e | R FD e | R FD e | x FD w | R FD e | x UN u |
| 30 | R PM e | R PM e | x PM w | x PM w | R PM e | x PM w | R PM e | x UN u | R MF e | R BV e |
| 40 | R FD e | R MF e | x PM s | R BV s | x BV w | R BV e | x MU e | x BV w | R BV e | x BV w |
| 50 | R BV e | R BV e | x HM s | x PM s | R BV s | R HM e | R PM e | R HM e | R HM s | R HM e |
| 60 | x MJ e | x MJ e | x MJ e | R MJ e | R MU e | R MJ s | R CV e | x KI u | x MU e | x KI u |
| 70 | x KI u | x KI u | x KI u | x KI u | R CV s | x MU e | x CV w | x CV e | x CV e | x UN u |
| 80 | R FD e | R FD e | x FD w | R FD e | x UN u | x KC u | x KC u | x KC u | x KC u | x KC u |
| 90 | x CV e | R MJ e | x PT e | x UN u | x UN u | x CV e | x CV e | x CV e | x CV s | x CV e |
| 100 | x CV e | x CV e | x CV w | R CV e | x CV w | R CV e | R CV s | x UN u | x UN u | x UN u |
| 110 | x PT e | x UN u | x V0 e | x UN u | R FD e | R CV e | x UN u | R MU e | x UN u | R MJ s |
| 120 | R MU e | x UN u | R MR s | x UN u | R MJ e | R MJ s | x MJ w | R MJ e | x UN u | R FD s |
| 130 | x UN u | x UN u | x UN u | x UN u | R MR e | R MR e | R MR e | x KS u | x KS u | x MJ s |
| 140 | x MJ e | x MJ e | x UN u | x KV u | x KV u | R FD e | R FD e | x FD s | x UN u | x UN u |
| 150 | x UN u | R MJ e | x KL u | x KL u | R HM e | x UN u | x UN u | R FD e | R FD e | x FD w |
| 160 | R FD s | R EP e | R FD e | x FD w | x FD w | R FD e | x KE u | x KE u | x KE u |  |

## Title clusters and places that are not in retail

None of these has a retail area, and apart from the titles themselves there is no data: they are groups of titles,
not areas.

- **The island, 67 and 69-73** (cluster, consecutive): 67 盗まれた島の絵本 ("the stolen island picture book") and 69-73
  島のヒミツその1-5 ("secret of the island 1-5"). 68 解読?らくがき岩 sits inside the run; JP titles its id in the
  forest (X01) and Civilization (X16, X18) tables, and graffiti plaques exist in AREA01 / AREA04 rooms (CUT_CONTENT.md
  "Graffiti rocks"). The run lies between 66 and 74, both Civilization, but 8 indexes apart: too far for the weak rule.
  Which island is not said (the whole game is on one; retail's AREA13 is Pig Island, with no link shown).
- **Light, sun and cold, 85-89** (cluster, consecutive): 85 光がほしい ("I want light"), 86 おひさまポカポカ… ("the sun
  is nice and warm..."), 87 寒い!裸? ("cold! naked?"), 88 溶かす、溶ければ… ("melt it, and if it melts..."), 89
  雪玉ゴロゴロその先は… ("where the rolling snowball goes..."). Ids 84-88 are untitled or reused in retail (the
  jungle pig bag and three Dwarf Village errands), so retail says nothing about them. 142 雪がふったら… ("when the snow
  falls...") and 113 固める、止まった時に… ("harden it, when time stops...") share the theme but are not next to the
  run: unknown. A snow or cold area is a reading of these titles, not a known area. Tomba! 2 has snow and ice (Kujara
  Ranch "Collect Snow", "Snow Fireflies", "Melt the Giant Ice!"): theme only.
- **Pig Town** (a place two titles name): 92 ブタの町 ("Pig Town") and 110 ブタの町のブタメガネ ("the pig glasses of Pig
  Town"). That a Pig Town was planned is shown by the titles; where it was is not. The titles next to them (93
  白いケムリの家, 94 コトバをおぼえよう!Ⅲ, 107-109, 111 王の本性) share no pig theme and stay unknown. Tomba! 2's Pig Tribe,
  Clown Circus and pig suit ("Use Pig Suit to Talk!"): theme only.
- **Sky, 137-138** (cluster): 137 おもいは地、かるいは天 ("heavy to the earth, light to the sky") and 138 雲の中身 ("what's
  inside the cloud"). The earlier "sky / towers / light" group put 131, 132, 149, 152 and 153 with them; those are not
  consecutive with it and are now unknown, or the light pair below.
- **Caves, 143-144** (cluster): 143 毒毒どうくつのナゾ ("riddle of the poison cave"), 144 水晶のどうくつ ("the crystal cave").
- **Light, 152-153** (cluster): 152 光と影 ("light and shadow"), 153 光る石を探せ ("find the glowing stone").
- **Grand finale, 166-168** (cluster): 「大団円1-3」, three ending placeholders. Retail id 165 is untitled with the hint
  "before you know it, everyone is happy"; ids 166-167 were reused for area-0 additions.
- **Sea and mermaid titles** (121, 123, 148, 150, 155, 156): the sea has a retail area (AREA06), so they are not a
  cluster; none sits between two titles of one area, so they are unknown. 126 水門をあけろ! ("open the floodgate!") is
  weak jungle (between 125 and 127).
- 0 少年の旅立ち ("the boy sets off") has no retail id and no area data; it reads as the opening, which is a theme.

## Village of Civilization: proto material in the list

| Candidate | Data | Verdict |
|---|---|---|
| The Civilization run, 74-78 and 95-106 (retail ids 73-77 and 94-105) | Same or near-same JP titles in the Civilization tables for 77, 78, 95, 96, 97, 99, 100, 101, 103, 105; retail code for the same ids in AREA16 / AREA17 (97, 98 at the gears, 102), the cut Iron Town (76, 99) and the cut towns' spawners (77) | **evidence** for these 10 titles (11 with 90 below), **strong** for 74 (blackout / power), 98 (clock tower) and 106 (the bomb), **weak** for 76, 102, 104. The towns were planned in June 1997 |
| 79 進め!きかんしゃ ("Go, locomotive!") | Retail reused id 78 for Find Charles! (jungle); neighbours 78 (Civilization) and 80 (forest) | **theme only**, area unknown. Tomba! 2's trolley is theme only too |
| 104 動かない鉄の人形 ("the iron doll that won't move") | Between 103 and 105 (Civilization); retail id 103 is The Cute Witch | **weak** (position) |
| 133 世紀の大発明 ("the invention of the century") | Retail id 132 is the witch's cold (AREA18) | **theme only**, area unknown |
| 90 緑の占い師 (Green Fortune Teller) | Same JP title in the AREA11 and AREA18 tables; the cut AREA11 data has his hut and line (CIVILIZATION.md) | **evidence** |
| 66 ナミダのビンちょうだい (tear bottle) | Retail id 65, AREA11 (the Bonsugee researcher), same JP title | **evidence** (already retail) |
| 76 事件発生、犯人を探せ! ("incident! find the culprit!") | Id 75 untitled; JP hint "find the stolen Evil Pig Bag" | **weak** (between 74 and 77). Folkin ties it to the green pig bag behind the Lumberjack boulder (not checked) |
| 75 電気うなぎを捕まえろ! (electric eel) | Same JP title in the jungle (X10) and Iron Town / Civilization (X17, X18) tables; unused item 108 Electric Eel | **evidence for both areas**: under "Data for more than one area" |
| 20 / 27 the five boxes and their key | 20 = retail 19 Lost and Found (JP 魔女の忘れもの, "what the witch forgot"), in AREA01 | 20 is **strong** forest; a link to the witch's Civilization home is **theme only** |
| 19 ドワーフの助け (the dwarves' help) | Forest title blocks of PrePre 9 and the JP demo; JP hint "have the lumberjack dwarf cut a tree" | **evidence** forest; a Lumberjack Town supply chain is **theme only** |
| 14 / 61 / 94 Learn the Language I, II, III | I = dwarves (strong, retail AREA01), II = Masakari (evidence, JP X10 only), III = unknown | Not VoC. 62 マサカリ族のヒミツ is read only by the cut Stonetown dialogue |
| 144 水晶のどうくつ | Its id 143 has the JP hint "the magic fades and the three towns become one" | Retail reused the id's hint for the Civilization reunion (never wired); says nothing about 144's area |
| Unused items | 61 Iron, 62 Iron Wheel (only the cut smith gives it), 108 Electric Eel, 109 Black Water, 147 Coal: Civilization. 90 Letter (three sisters) and 140 Chief's Spear: jungle | As in CUT_CONTENT.md |

## By area

Rows keep the list index (#); the retail column gives the retail id (= # - 1), its PAL English title and, in
brackets, the JP retail title where JP titles it. "JP X10" = the JP retail AREA10 title table, and so on.

### Village of All Beginnings (AREA00)

14 titles: 12 in retail, 2 cut; 14 evidence.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 1 | ジィちゃんの形見 | Grandpa's Keepsake | retail (same): Grandpa's Bracelet | 0: Grandpa's Bracelet (ジイちゃんの形見) | evidence | PrePre 8 sets it in code (0x800204C0); area-0 title block of Hyper 12, the JP demo and Dengeki; JP X00 |  | **beta 980203**: Find the bracelet stolen by the Komabuta. *(earlier English wording)*<br>**retail EN**: Find the bracelet stolen by the Koma Pigs. |
| 2 | 100年老人との出会い | Meeting the 100 Year Old Man | retail (same): The 100 Year Old Wise Man | 1: The 100 Year Old Wise Man (100年老人との出会い) | evidence | PrePre 8 area 0 room 0 record starts it; area-0 title blocks of the demos |  | **Pure 8 dialogue**: Yan: "If you climb on up ahead, there's a know-it-all old geezer called the 100 Year Old Man living there."<br>**beta 980203**: Meet the 100 year old wise man. *(earlier English wording)*<br>**retail EN**: Meet the 100 Year Old Wise Man. |
| 3 | 霧を吹き飛ばせ | Blow Away the Fog | retail (same): Clear the Fog | 2: Clear the Fog (霧を吹き飛ばせ！) | evidence | PrePre 8 starts it in code; area-0 title blocks of the demos; retail AREA00 s0 |  | **retail EN**: Blow away the thick fog. |
| 4 | カエル…にかえる | Return the Frog... | retail (same): Take Me Home | 3: Take Me Home (カエル水にかえる) | evidence | PrePre 8 starts it in code; area-0 title blocks of the demos; retail AREA00 s0 |  | **beta 980203**: Catch the frog. *(earlier English wording)*<br>**retail EN**: Bring the frog home. |
| 5 | モトクロスコース | Motocross Course | retail (same): Motocross Course | 4: Motocross Course (モトクロスコース) | evidence | area-0 title blocks of the demos and JP X00 (retail sets it from AREA03 code) |  | **JP demo**: An exciting course. *(differs from retail)*<br>**Yoiko 2**: It seems you have to be able to swim. *(differs from retail)*<br>**retail EN**: The bumpy road beyond the fence. |
| 6 | キミ、だあれ? | Who Are You? | retail (same): Who are you? | 5: Who are you? (キミ、だあれ？) | evidence | PrePre 8 starts it in code; area-0 title blocks of the demos; retail AREA00 |  | **Pure 8 dialogue**: Yan, when asked who he is: "...Eh? Oh, me? I'm... I'll tell you next time we meet!"<br>**beta 980203**: Talk to the person peeking from the window. *(earlier English wording)*<br>**retail EN**: Who is this mysterious person? |
| 7 | 大きな水たまり? | A Big Puddle? | cut | 6: untitled in PAL (JP 大きな水たまり？), never triggered | evidence | PrePre 8's area-0 dialogue: a big puddle behind the hut, 'can you swim?'; area-0 title blocks of Hyper 12, JP demo, Dengeki (JP retail later lists it in the jungle X10) |  | **Pure 8 dialogue**: Yan: "Go straight back behind this hut and there's a big puddle... Trust me and go and see."<br>**retail EN**: Listen to the story of the big puddle. |
| 8 | かくれ里のヤン | Yan of the Hidden Village | retail (same): Hide and Go Seek | 7: Hide and Go Seek (JP keeps かくれ里のヤン) | evidence | Yan in PrePre 8's area-0 dialogue; area-0 title blocks of the demos; retail AREA00 | same name: Yan of the Hidden Village | **Pure 8 dialogue**: Yan: "I'm called Yan of the Hidden Village... If you can keep finding me, I'll teach you more."<br>**beta 980203**: Find out the name of the person hiding. *(earlier English wording)*<br>**retail EN**: Play hide and go seek with Yan. |
| 9 | オイラ泳げない… | I Can't Swim... | retail (same): I Can't Swim... | 8: I Can't Swim... (オイラ泳げない…) | evidence | PrePre 8 starts it in code; its area-0 kid asks 'can you swim?'; area-0 title blocks of the demos |  | **Pure 8 dialogue**: A kid: "Can you swim? ...That's no good. If you can't swim, you can't go to the big puddle ahead."<br>**retail EN**: Need to learn how to swim. |
| 10 | コッカ卵の中身 | Inside the Kokka Eggs | retail (same): Inside the Kokka Eggs | 9: Inside the Kokka Eggs (コッカタマゴの中身) | evidence | PrePre 8 starts it in code; its area-0 old man asks for the Kokka eggs |  | **Pure 8 dialogue**: The 100 Year Old Man: "Would you gather the Kokka eggs scattered outside? Kokka steals eggs and keeps them."<br>**beta 980203**: Collect the things inside the eggs. *(earlier English wording)*<br>**retail EN**: Collect the chicks inside the eggs. |
| 11 | 100年老人の話 | Tale of the 100 Year Old Man | retail (same): Tale of the Evil Pigs | 10: Tale of the Evil Pigs (100年老人の話) | evidence | PrePre 8 code (set-4 11); its area-0 old man tells of the Evil Pigs; same JP title; JP X00 |  | **Pure 8 dialogue**: The 100 Year Old Man, once the eggs are in: "The Evil Pigs!? ...Very well... listen closely, lively child."<br>**retail EN**: Learn about what happened when the Evil Pigs came to town. |
| 12 | 100年老人の仲間 | A Friend of the 100 Year Old Man | retail (same): The 1,000 Year Old Man? | 11: The 1,000 Year Old Man? (100年老人の仲間) | evidence | same JP title in JP X00; its slot (shown as ???) is in the area-0 title blocks of the demos; retail completes it in AREA04 s15 |  | **beta 980203**: An old man in captivity? *(earlier English wording)*<br>**retail EN**: Free the 1,000 Year Old Man from captivity. |
| 13 | ドワーフの長老 | The Dwarf Elder | retail (same): Dwarf Elder | 12: Dwarf Elder (ドワーフの長老) | evidence | PrePre 8's area-0 door object handles it and its old man sends Tomba to the elder; area-0 title blocks; retail AREA00 s3 |  | **Pure 8 dialogue**: The 100 Year Old Man: "First, go and meet the chief of the dwarves who live in the forest ahead."<br>**retail EN**: Meet the Dwarf Elder. |
| 112 | 始まりの村 | The Village of All Beginnings | cut, id reused for the Baccus Village pig bag | 111: Baccus Village (バッカスの村) | evidence | the title is the name of area 0; its id became the Baccus Village pig bag, another place-named event |  | **none**: No description in any build. Its id 111 became the Baccus Village pig bag in retail. *(its retail hint, for the other event: "Seal the powers of the Evil Pig in the bag.")* |

### Forest of 100 Flowers and Dwarf Village (AREA01 / AREA02 / AREA19)

33 titles: 25 in retail, 8 cut; 22 evidence, 5 strong, 6 weak.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 14 | コトバをおぼえよう!Ⅰ | Let's Learn the Language! I | retail (renamed): Beginner's Dwarf Language | 13: Beginner's Dwarf Language (コトバを吸いとろう！) | strong | retail id 13 is the dwarf language (AREA01 script), the same goal under another title |  | **Pure 8 dialogue**: A dwarf: "Oh, you understand our language!? We are in great trouble..."<br>**retail EN**: Learn how to speak dwarf language. |
| 15 | まいごのドワーフ | The Lost Dwarf | retail (same): A Lost Child | 14: A Lost Child (まいごのドワーフ) | evidence | same JP title; PrePre 8's dwarf dialogue has the lost dwarf child; forest title block of PrePre 9; retail AREA02 s0 |  | **Pure 8 dialogue**: A dwarf: "My child went into the forest to look for flower seeds and never came back..."<br>**retail EN**: Look for the child that is lost in the forest. |
| 16 | 花のたね | Flower Seeds | retail (same): Flower Seeds | 15: Flower Seeds (花のたね) | evidence | same JP title in the forest title blocks of PrePre 9 and the JP demo, and JP X01 / X02 |  | **beta 980203**: Pick up some seeds. *(earlier English wording)*<br>**retail EN**: Where should I plant the seeds? |
| 17 | AP・ゲート | AP Gate | retail (renamed): The AP Box | 16: The AP Box (ＡＰボックス) | evidence | PrePre 8's dwarf dialogue talks of the AP 'point gate' (retail moved the AP Box to AREA00 s2) |  | **Pure 8 dialogue**: A dwarf: "Seen the strange thing with AP written on it? Rumour says it is called a point gate... we cannot get in."<br>**retail EN**: Collect AP points for something special. |
| 18 | ドワーフを救え | Save the Dwarves | retail (same): Save the Dwarves | 17: Save the Dwarves (ドワーフを救え) | evidence | same JP title; PrePre 9's live AREA01 scripts award it; retail AREA02 s1 |  | **Pure 8 dialogue**: The dwarf elder: "Right now many of our companions have gone missing deep in this forest..."<br>**beta 980203**: Find the missing dwarfs in the forest. *(earlier English wording)*<br>**retail EN**: Find the missing dwarves in the forest. |
| 19 | ドワーフの助け | The Dwarves' Help | cut | 18: untitled in PAL (JP ドワーフの助け), never triggered; JP hint: "have the lumberjack dwarf cut a tree" | evidence | same JP title in the forest title blocks of PrePre 9 and the JP demo (between Save the Dwarves and the witch) and JP X01 / X02 |  | **retail EN**: Ask the lumberjack dwarf for some wood. |
| 20 | 5つの箱の忘れもの | The Things Left in the Five Boxes | retail (renamed): Lost and Found | 19: Lost and Found (魔女の忘れもの) | strong | retail id 19 (JP 'the witch's forgotten things') is an item object in AREA01 s3; the 'forgotten things' goal is kept |  | **Pure 8 dialogue**: A dwarf: "Long ago a witch came to this village from the sea. The boxes in the forest that won't open are her forgotten things."<br>**Dengeki D5**: Something is inside the crystal of the mysterious forest. *(differs from retail)*<br>**beta 980203**: Three Crystals that the witch left behind *(earlier English wording)*<br>**retail EN**: Find the remaining crystal balls and return them to their owner. |
| 21 | 消えた長老 | The Elder Has Vanished | cut, id reused for Stop the Fight! | 20: Stop the Fight! (ケンカをとめて) | weak | between 20 and 22, both Dwarf Village |  | **Pure 8 dialogue**: A dwarf: "Oh, are you looking for the elder too? Two dwarves started a fight... if the elder were here he'd stop them."<br>**retail EN, other event**: "Break up the fight among the dwarves." *(the id's retail hint, for Stop the Fight!)* |
| 22 | 地下ロウの大ドロボウ | The Great Thief of the Underground Jail | retail (same): The Great Escape | 21: The Great Escape (JP keeps 地下ロウの大ドロボウ) | evidence | same JP title; PrePre 8's dwarf dialogue has the trapped thief; retail AREA02 s2 (Underground Prison) |  | **Pure 8 dialogue**: The thief: "You're the brat who blocked the passage! I tried to escape while the Koma Pigs weren't looking..."<br>**JP demo**: Talk with the great thief underground. *(same in Yoiko 2, Dengeki D5; differs from retail)*<br>**beta 980203**: The great thief tried to escape···but? *(earlier English wording)*<br>**retail EN**: The thief is trying to escape. |
| 23 | のぞいてごらん?! | Take a Look?! | retail (same): Look and See?! | 22: Look and See?! (のぞいてごらん？！) | evidence | same JP title; forest title block of PrePre 9; retail AREA01 s3 |  | **retail EN**: Enjoy the view. |
| 24 | バロンは友だち | Baron Is a Friend | retail (same): A Man's Best Friend | 23: A Man's Best Friend (バロンは友だち) | evidence | same JP title; PrePre 8's dwarf nurse names the dog Baron; retail AREA02 |  | **Pure 8 dialogue**: The dwarf nurse, about an injured dog: "What's its name? ...Baron? ...That's a good name!"<br>**JP demo**: Help the injured puppy. *(same in Yoiko 2; differs from retail)*<br>**beta 980203**: Help the injured Baron. *(earlier English wording)*<br>**retail EN**: Help the injured puppy. |
| 25 | これ、な〜んだ? | What's This? | retail (same): What is this? | 24: What is this? (これ、な～んだ？) | evidence | same JP title; forest title block of PrePre 9; retail AREA01 s3 |  | **beta 980203**: The broken elevator that's hidden from view. *(earlier English wording)*<br>**retail EN**: Something was hidden from view. |
| 26 | やしきの宝物? | The Mansion's Treasure? | retail (same): Treasures of the Mansion? | 25: Treasures of the Mansion? (やしきの宝物？) | evidence | same JP title in the forest title blocks of PrePre 9 and the JP demo (retail code checks it in every area) |  | **Pure 8 dialogue**: A dwarf: "Going out of the forest? Try the seashore: very pretty, and you can see a big mansion far away."<br>**retail EN**: Find the rumored treasures. |
| 27 | 5つの箱のカギ | The Key to the Five Boxes | cut, id reused for To Phoenix Mountain... | 26: To Phoenix Mountain... (大トリの山へ) | weak | between 26 and 28, both forest; pairs with 20 (the five boxes) |  | **Dengeki D5**: Have them teach you the secret spell. *(the id's hint in November 1997; it does not echo the title; differs from retail)*<br>**retail EN, other event**: "Head to Phoenix Mountain." *(the id's retail hint, for To Phoenix Mountain...)* |
| 28 | 動かない恵みコゾウ | The Blessing Boy Who Won't Move | retail (same): The Broken Fountain | 27: The Broken Fountain (JP keeps 動かない恵みコゾウ) | evidence | same JP title; forest title blocks of PrePre 9 and the JP demo; retail AREA01 | same name: Unmoving Blessed Priest | **beta 980203**: Revive the statue. *(earlier English wording)*<br>**retail EN**: Revive the statues. |
| 40 | 花をさかせよう! | Let's Make Flowers Bloom! | retail (same): Plant a Flower Garden | 39: Plant a Flower Garden (花をさかせよう！) | evidence | same JP title; JP X01 / X02; retail AREA02; PrePre 8's dwarf script has wilted flowers |  | **Pure 8 dialogue**: A dwarf: "I'd love to make flowers bloom in this bed... the flowers were all wilted."<br>**retail EN**: Plant flower seeds to grow a beautiful flower garden. |
| 80 | 森の地下には何がある? | What's Under the Forest? | retail (same): What's Under the Forest? | 79: What's Under the Forest? (森の地下には何がある？) | evidence | same JP title in the forest lists X01 / X02 (and X11); retail starts it in AREA11 with 'the elevator near the Watchtower' |  | **retail EN**: The underground world comes back to life. |
| 81 | 花の森 | The Flower Forest | retail (same): The 100 Flower Forest | 80: The 100 Flower Forest (花の森) | evidence | same JP title; the forest pig bag (JP X01 / X02) |  | **Dengeki D5**: The Evil Pig's magic wears off and the forest gets its vigour back. *(differs from retail)*<br>**retail EN**: Seal the powers of the Evil Pig in the bag. |
| 82 | かんだら割れちゃうゾ | It'll Crack If You Bite It | cut, id reused for The Boss' Treasure | 81: The Boss' Treasure (カシラの大事なもの) | weak | between 81 and 83, both forest / Dwarf Village (AREA19 is the Dwarf Village in blossom) |  | **Dengeki D5**: Careful not to bite! *(differs from retail)*<br>**retail EN, other event**: "He says you have his treasure." *(the id's retail hint, for The Boss' Treasure)* |
| 83 | おなかペコペコ? | Hungry? | retail (same): I'm So Hungry | 82: I'm So Hungry (おなかペコペコ？) | evidence | same JP title; retail AREA19 actor (the Dwarf Village in blossom); JP X19 (and X03) |  | **beta 980203**: Give something to eat to Taro. *(earlier English wording)*<br>**retail EN**: Find something for this person to eat. |
| 114 | バロンの力 | Baron's Power | retail (same): Baron's Strength | 113: Baron's Strength (バロンの力！？) | evidence | same JP title, only in JP X02; retail AREA02 |  | **beta 980203**: This dog must possess some special hidden power··· *(earlier English wording)*<br>**retail EN**: This dog must possess some special, hidden power... |
| 129 | 青の占い師 | The Blue Fortune Teller | retail, other id: The Blue Fortune Teller (same title, moved to id 146) | 128: Unbreakable Wire (丈夫なハリガネ); **the same title is retail 146** The Blue Fortune Teller (青の占い師) | strong | the same title is retail id 146, in the Dwarf Village tunnel AREA02 s3 |  | **beta 980203**: To know the best of yourself, seek fortunetellers.! *(earlier English wording)*<br>**retail EN**: To know the best of yourself, seek fortune tellers! |
| 145 | 億年老人の願い | The 100 Million Year Old Man's Wish | retail (same): Million Year Old Wish | 144: Million Year Old Wish (億年老人の願い) | evidence | same JP title; retail AREA02 s4 |  | **beta 980203**: The 100 million year old man's wish is to seal the Magic Pig··· *(earlier English wording)*<br>**retail EN**: The Million Year Old Man's wish is to seal all the Evil Pigs... |
| 146 | うまった道を行く | Down the Buried Road | retail (same): Dig Like a Mole | 145: Dig Like a Mole (うまった道を行く) | evidence | same JP title; retail AREA02 s3 |  | **beta 980203**: Let's proceed the path that doesn't have ways··· *(earlier English wording)*<br>**retail EN**: A digger trying to dig a new path. |
| 147 | 黒の占い師 | The Black Fortune Teller | cut, its id went to the Blue Fortune Teller (moved from id 128) | 146: The Blue Fortune Teller (青の占い師): the blue one took this id | strong | retail id 146 is a fortune teller in AREA02 s3 (JP X01 / X02) |  | **none**: No description in any build. Its id 146 became the Blue Fortune Teller in retail. *(its retail hint, for the other event: "To know the best of yourself, seek fortune tellers!")* |
| 157 | 魔力のみなもと? | The Source of the Magic? | retail (same): Source of Evil Magic | 156: Source of Evil Magic (魔力のみなもと？) | evidence | same JP title; retail AREA02 s4 |  | **beta 980203**: Where does the evil power derive from? *(earlier English wording)*<br>**retail EN**: The Million Year Old Man knows where it comes from. |
| 158 | 友達10人 | Ten Friends | retail (same): Seven Friends | 157: Seven Friends (友達７人) | evidence | same JP title but the number (ten -> seven); retail AREA02 |  | **beta 980203**: All of us should get together if one cannot do alone.. *(earlier English wording)*<br>**retail EN**: Find seven friends to open the last gate. |
| 159 | 金色のカイロウ | The Golden Corridor | cut; Tomba! 2 has a similar idea (theme only) | 158: untitled everywhere | weak | between 158 and 160, both Dwarf Village | theme only: Golden Tower | **none**: No description in any build. Retail id 158 has no hint text in any build (placeholder only). |
| 160 | 悪いヤツは誰だ? | Who's the Bad Guy? | retail (renamed): The 8th Evil Pig Bag? | 159: The 8th Evil Pig Bag? (８つめの封印ぶくろ？) | strong | retail id 159 is the hidden eighth pig bag of AREA02: who the bad guy is |  | **beta 980203**: Why? I found the eighth Evil Pig Bag. *(earlier English wording)*<br>**retail EN**: Seal the powers of the Master Evil Pig. |
| 162 | 夢?幻?黄金郷を探せ | Dream? Illusion? Find El Dorado | retail (same): Underground Treasure | 161: Underground Treasure (夢？幻？宝物庫を探せ) | evidence | same JP title but one word (El Dorado -> treasury); retail AREA02 s3 |  | **beta 980203**: Is the true treasure box somewhere in this world? *(earlier English wording)*<br>**retail EN**: Find the underground treasure. |
| 163 | らくがき床のナゾ | Riddle of the Graffiti Floor | cut | 162: untitled everywhere | weak | between 162 and 165, both forest / Dwarf Village |  | **none**: No description in any build. Retail id 162 has no hint text in any build (placeholder only). |
| 164 | かくされた入口 | The Hidden Entrance | cut | 163: untitled everywhere | weak | between 162 and 165, both forest / Dwarf Village |  | **none**: No description in any build. Retail id 163 has no hint text in any build (placeholder only). |
| 165 | ミリオンの塔 | The Million Tower | retail (same): The Flower Tower | 164: The Flower Tower (ミリオンの塔) | evidence | same JP title; retail AREA01 s4 |  | **beta 980203**: How old is a million? *(earlier English wording)*<br>**retail EN**: What's inside the Flower Tower? |

### Phoenix Mountain (AREA03)

10 titles: 5 in retail, 5 cut; 5 evidence, 2 strong, 3 weak.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 30 | 嵐の山の怪 | Mystery of the Stormy Mountain | retail (same): A Stormy Pig Bag | 29: A Stormy Pig Bag (JP keeps 嵐の山の怪) | evidence | same JP title, only in JP X03; retail AREA03 |  | **beta 980203**: Find the hidden Evil Pig Bag. *(earlier English wording)*<br>**retail EN**: Find the Pig Bag for the Stormy Pig. |
| 31 | 大トリの山 | Big Bird Mountain | retail (same): Phoenix Mountain | 30: Phoenix Mountain (大トリの山) | evidence | same JP title in JP X03; the Phoenix Mountain pig bag |  | **retail EN**: Seal the Evil Pig's powers in the bag. |
| 32 | コロガルゾォ〜 | It's Gonna Roll! | cut, id reused for Where did I come from? | 31: Where did I come from? (どこからきたの？) | weak | between 31 and 34, both Phoenix Mountain |  | **none**: No description in any build. Its id 31 became Where did I come from? in retail. *(its retail hint, for the other event: "How could the path take me here?")* |
| 33 | 光の落ちるところ | Where the Light Falls | cut | 32: untitled in PAL; JP ラッパでミニミニ ("Miniature Making Trumpets"), not awarded; JP hint: "where did the coin blow in from" | weak | between 31 and 34, both Phoenix Mountain (its id is only in JP X03, under another title) |  | **retail EN**: Where did the coin come from? |
| 34 | いよっ!穴ほり名人! | Hey! The Master Digger! | retail (same): The Famous Digger | 33: The Famous Digger (いよっ！穴ほり名人！) | evidence | same JP title in JP X03; retail AREA03 |  | **beta 980203**: Meet the Infamous Digger. *(earlier English wording)*<br>**retail EN**: Visit the Famous Digger after he's finished digging his hole. |
| 35 | またコロガルゾォ〜 | It's Gonna Roll Again! | cut, id reused for the Lava Caves pig bag | 34: Lava Caves (マグマのどうくつ) | weak | between 34 and 36, both Phoenix Mountain |  | **Dengeki D5**: Put out the flames blocking your way. *(the id's hint in November 1997; it does not echo the title; differs from retail)*<br>**retail EN, other event**: "Seal the Evil Pig's powers in the bag." *(the id's retail hint, for the Lava Caves pig bag)* |
| 36 | 空の主 | Master of the Sky | retail (same): The Master of the Skies | 35: The Master of the Skies (空の主) | evidence | same JP title, only in JP X03; retail AREA03 |  | **beta 980203**: Meet the Giant Bird at Stormy Mountain. *(earlier English wording)*<br>**retail EN**: Meet the Phoenix at Stormy Mountain. |
| 42 | 笑いが止まらない | Can't Stop Laughing | cut, id reused for Smile! (a laughing door) | 41: Smile! (笑ってごらん) | strong | retail id 41 is a laughing door in AREA03 s1 (JP X03, X04): same laughing theme |  | **none**: No description in any build. Its id 41 became Smile! (a laughing door) in retail. *(its retail hint, for the other event: "A locked door with a large grin.")* |
| 53 | 赤ちゃんブリッジ? | Baby Bridge? | cut | 52: untitled in PAL (JP 渡れなくなっちゃった…), never triggered; JP hint: "no bridge, stuck" | strong | JP titles id 52 only in Phoenix Mountain's X03, as 'I can't cross any more': same bridge theme |  | **retail EN**: Can't cross without a bridge. |
| 56 | あやしいトビラ | A Suspicious Door | retail (same): A Large Key Hole! | 55: A Large Key Hole! (JP keeps あやしいトビラ) | evidence | same JP title; retail door object in AREA03 s1 (JP X03 and X04) |  | **beta 980203**: Find the small key that fits the door. *(earlier English wording)*<br>**retail EN**: Find the large key that fits the door. |

### Haunted Mansion (AREA04)

6 titles: 5 in retail, 1 cut; 4 evidence, 2 strong.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 52 | 大きなドアのむこう側 | Beyond the Big Door | cut, id reused for A Small Key Hole! (a door) | 51: A Small Key Hole! (ちいさなカギのドア) | strong | retail id 51 is a door of AREA04 s0 (JP X04 only): same door theme |  | **none**: No description in any build. Its id 51 became A Small Key Hole! (a door) in retail. *(its retail hint, for the other event: "Find the small key that fits the door.")* |
| 55 | 盗賊マンション | The Thieves' Mansion | retail (renamed): The Haunted Mansion | 54: The Haunted Mansion (シーフマンシヨン) | evidence | same JP title ('Thief Mansion'); the mansion's pig bag, JP X04 |  | **retail EN**: Seal the powers of the Evil Pig in the bag. |
| 57 | 大きなカギの絵? | A Picture of a Big Key? | retail (same): Painting of a Big Key? | 56: Painting of a Big Key? (大きなカギの絵？) | evidence | same JP title, only in JP X04; retail AREA04 |  | **retail EN**: Find all the pieces of the picture to get an item. |
| 58 | 1000年老人 | The 1,000 Year Old Man | retail (renamed): Break the Magic Egg! | 57: Break the Magic Egg! (魔法タマゴをわろう！) | strong | retail id 57 plays in the 1,000 Year Old Man's room, AREA04 s15 (JP X04) |  | **retail EN**: Break the spell and listen to the 1,000 year old man's story. |
| 59 | かくされた力 | Hidden Power | retail (same): Red Hidden Powers | 58: Red Hidden Powers (かくされた力) | evidence | same JP title, only in JP X04; retail AREA04 s15 |  | **beta 980203**: Unleash the hidden powers. *(earlier English wording)*<br>**retail EN**: Find the Jewel of Fire to unleash the hidden powers from within. |
| 154 | 水の下には何がある? | What's Under the Water? | retail (same): What's Underwater? | 153: What's Underwater? (水の下には何がある？) | evidence | same JP title; retail mermaid diving code in AREA04 (JP X04 / X10) |  | **beta 980203**: The Invisible underwater that looks almost visible··· *(earlier English wording)*<br>**retail EN**: Learn to dive underwater. |

### Baccus Village and Baccus Lake (AREA05 / AREA08)

10 titles: 7 in retail, 3 cut; 5 evidence, 2 strong, 3 weak.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 39 | 死神の実をつぶせ | Crush the Reaper's Fruit | retail (same): Death Fruit Juice | 38: Death Fruit Juice (死神の実をつぶせ) | evidence | same JP title; retail AREA05 actors; JP X05 (the forest lists also title it) |  | **retail EN**: Crush the fruit that's killing the forest plants. |
| 43 | 涙が止まらない | Can't Stop Crying | retail, other id: Can't Stop Crying (same title, id 43) | 42: Cry Baby (泣いてごらん); **the same title is retail 43** Can't Stop Crying (涙がとまらない, AREA05) | strong | word for word retail id 43, a Baccus event (JP X05), one id off the rule; by the rule it would be id 42, the mansion's crying door |  | **retail EN**: Can't stop the tears from flowing. |
| 44 | 泣き笑いのドア | The Crying and Laughing Door | cut, id reused for Can't Stop Crying; Tomba! 2 has the same name | 43: Can't Stop Crying (涙がとまらない) | weak | between 43 and 45, both Baccus (43 only by its title); the theme is retail's laughing and crying doors (AREA03 s1 / AREA04 s0) | same name: The Crying Door / The Laughing Door | **none**: No description in any build. Its id 43 became Can't Stop Crying in retail. *(its retail hint, for the other event: "Can't stop the tears from flowing.")* |
| 45 | 赤の占い師 | The Red Fortune Teller | retail (same): The Red Fortune Teller | 44: The Red Fortune Teller (赤の占い師) | evidence | same JP title, JP X05; retail AREA05 (dialogue only in AREA05's bank) |  | **retail EN**: Meet the Red Fortune Teller. |
| 47 | 子ども?大人? | A Child? An Adult? | cut, id reused for Where's the Baby Mouse?; Tomba! 2 has a similar idea (theme only) | 46: Where's the Baby Mouse? (子ネズミどこだ？) | weak | between 46 and 48 (Baccus run 45-51); the retail id is Baccus too, but 'child or adult' vs a baby mouse is not the same goal | theme only: Tiny Tomba (shrinking) | **none**: No description in any build. Its id 46 became Where's the Baby Mouse? in retail. *(its retail hint, for the other event: "Find the lost baby mouse.")* |
| 48 | 集まったら持っといで… | Bring Them When You Have Them... | retail (same): Some Cheese Please | 47: Some Cheese Please (集まったら持っといで…) | evidence | same JP title, JP X05; retail AREA05 actors |  | **beta 980203**: Gather unusual items for the collector and receive an item in return. *(earlier English wording)*<br>**retail EN**: Gather ten slices of cheese and return them to the collector. |
| 49 | ポイントのジョー? | Joe of the Points? | cut | 48: untitled everywhere, never triggered | weak | between 48 and 50, both Baccus |  | **none**: No description in any build. Retail id 48 has no hint text in any build (placeholder only). |
| 50 | 大人の飲み物 | A Drink for Grown-ups | retail (same): A Drink for Grownups | 49: A Drink for Grownups (大人の飲み物) | evidence | same JP title, JP X05; retail AREA05 s1 |  | **retail EN**: Brave the Haunted Mansion to fix the pump. |
| 51 | バッカスの湖 | Baccus Lake | retail (renamed): Road to Baccus Lake | 50: Road to Baccus Lake (湖への道) | evidence | the title names Baccus Lake; retail id 50 is set in AREA08 (JP X05 / X08) |  | **beta 980203**: Run fast through the dangerous to get to the Lake. *(earlier English wording)*<br>**retail EN**: Head through the Haunted Mansion to Baccus Lake. |
| 54 | 魔ブタをさがせ! | Find the Evil Pig! | retail (renamed): The Mouse Pig Bag | 53: The Mouse Pig Bag (もとの姿にもどりたい) | strong | retail id 53 is the Baccus pig bag (AREA05, JP X05): an Evil Pig to find |  | **beta 980203**: Return the mice to their original selves. *(earlier English wording)*<br>**retail EN**: Find the Evil Pig Bag to restore Baccus Village. |

### Mushroom Forest (AREA09)

2 titles: 2 in retail, 0 cut; 2 evidence.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 38 | 怪物?勝てるかな? | A Monster? Can I Win? | retail (same): Monster Hunt | 37: Monster Hunt (怪物？勝てるかな？) | evidence | same JP title; retail AREA09 s0 object; JP X09 (the forest lists also title it) |  | **retail EN**: Fight the monster that lives in the Mushroom Forest. |
| 41 | お涙ちょうだい | Give Me Your Tears | retail (same): Tears from a Flower | 40: Tears from a Flower (お涙ちょうだい) | evidence | same JP title, only in JP X09; retail AREA09 |  | **beta 980203**: Are the tears needed to revive the statue? *(earlier English wording)*<br>**retail EN**: Will the tears revive the fountain? |

### Mermaid's Singing Rock (AREA06)

4 titles: 4 in retail, 0 cut; 3 evidence, 1 strong.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 122 | 歌を忘れた人魚 | The Mermaid Who Forgot Her Song | retail (renamed): Mermaid's Singing Rock | 121: Mermaid's Singing Rock (人魚の歌岩) | strong | retail id 121 is the singing mermaid of AREA06 s0 (JP X06): same mermaid's song | theme only: The Broken-Up Mermaid Harp! | **Dengeki D5**: The thing she forgot is a memory? *(differs from retail)*<br>**retail EN**: The Motocross Goal! |
| 134 | 銅色のメダルがほしい | I Want the Bronze Medal | retail (same): I Want a Bronze Medal | 133: I Want a Bronze Medal | evidence | same JP title, only in JP X06; retail AREA06 s2 |  | **retail EN**: An honorable medal for exceeding the existing third winner's record. |
| 135 | 銀色のメダルがほしい | I Want the Silver Medal | retail (same): I Want a Silver Medal | 134: I Want a Silver Medal | evidence | same JP title, only in JP X06; retail AREA06 s2 |  | **retail EN**: An honorable medal for exceeding the existing second winner's record. |
| 136 | 金色のメダルがほしい | I Want the Gold Medal | retail (same): I Want a Gold Medal | 135: I Want a Gold Medal | evidence | same JP title, only in JP X06; retail AREA06 s2 |  | **retail EN**: A marvelous medal for exceeding the existing champion's record. |

### Masakari Jungle (AREA10)

15 titles: 8 in retail, 7 cut; 10 evidence, 4 strong, 1 weak.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 60 | 長老のたのみごと? | The Elder's Request? | cut | 59: untitled in PAL (JP 族長のたのみごと？ "the chief's request"), never triggered | evidence | JP X10 only, as 'the chief's request' (elder -> chief) |  | **retail EN**: Find the Masakari Chief's spear. |
| 61 | コトバをおぼえよう!Ⅱ | Let's Learn the Language! II | cut | 60: untitled in PAL (JP コトバを吸いとろう！Ⅱ), never triggered | evidence | JP X10 only (Language II, the Masakari) |  | **retail EN**: Learn the Masakari People's language. |
| 62 | マサカリ族のヒミツ | Secret of the Masakari Tribe | cut | 61: untitled in PAL (JP マサカリ族のヒミツ); read only by the cut Stonetown | evidence | same JP title, JP X10 only |  | **retail EN**: Unveil the secrets of the Masakari. |
| 63 | 物知りの木は知っている | The Wise Tree Knows | retail (same): Tree of Knowledge Knows | 62: Tree of Knowledge Knows (物知りの木は知っている) | evidence | same JP title; retail AREA10 |  | **retail EN**: Go to the Wise Old Tree. |
| 65 | おぼれちゃう!? | I'm Going to Drown!? | retail (renamed): A Refreshing Drink | 64: A Refreshing Drink (バナナジュース) | strong | retail id 64 is the jungle swim teacher's event (AREA10, 'you can't swim'): same swimming theme |  | **beta 980203**: Give something Charles likes to learn something new. *(earlier English wording)*<br>**retail EN**: Quench Charles' thirst to learn a new ability. |
| 91 | かくされた力その2 | Hidden Power, Part 2 | retail (same): Blue Hidden Powers | 90: Blue Hidden Powers (かくされた力その２) | evidence | same JP title, only in JP X10; retail AREA10 s3 |  | **beta 980203**: Find new powers from within. *(earlier English wording)*<br>**retail EN**: Find the Jewel of Water to unleash other hidden powers. |
| 119 | 大きな水たまりを渡れ | Cross the Big Puddle | retail (renamed): The 10,000 Year Old Man | 118: The 10,000 Year Old Man (水たまりの万年老人) | strong | retail id 118 (JP 'the 10,000 Year Old Man of the puddle') is in AREA10: same puddle |  | **beta 980203**: Sink or swim! *(earlier English wording)*<br>**retail EN**: Visit the 10,000 Year Old Man. |
| 124 | 水に沈んだ村 | The Sunken Village | retail (same): The Underwater Pig Bag | 123: The Underwater Pig Bag (JP keeps 水に沈んだ村) | evidence | same JP title, only in JP X10; retail AREA10 |  | **beta 980203**: Seal the powers of the Evil Pig in the bag. *(earlier English wording)*<br>**retail EN**: Find the Evil Pig Bag for the Underwater Pig. |
| 125 | いにしえの村 | The Ancient Village | retail (renamed): Trick Village | 124: Trick Village (からくり村) | strong | retail id 124 is the Trick Village pig bag of AREA10 s3: a village-named pig bag |  | **Dengeki D5**: The magic fades and the village gets its old form back. *(differs from retail)*<br>**retail EN**: Seal the powers of the Evil Pig in the bag. |
| 126 | 水門をあけろ! | Open the Floodgate! | cut, id reused for The Thief's Door; Tomba! 2 has the same name | 125: The Thief's Door (盗賊のトビラ) | weak | between 125 and 127, both jungle | same name: Open the Water Gate | **none**: No description in any build. Its id 125 became The Thief's Door in retail. *(its retail hint, for the other event: "What tool should I use to open this door?")* |
| 127 | 万年老人の数字玉 | The 10,000 Year Old Man's Number Beads | retail (same): The 10 Math Beads | 126: The 10 Math Beads (万年老人の数字玉) | evidence | same JP title, only in JP X10; retail AREA10 |  | **beta 980203**: Seek for the hidden beads of ordeal *(earlier English wording)*<br>**retail EN**: Find the 10,000 Year Old Man's Math Beads. |
| 139 | からくり?サァたいへん | A Contraption? Oh No! | cut | 138: untitled in PAL (JP つかまえてごらん), never triggered | strong | JP titles id 138 only in X10 (another title); 'contraption' fits Trick Village |  | **JP hint (translated)**: Keep calm, don't panic, find the exit!! |
| 140 | からくり砦の3姉妹 | The Three Sisters of the Trick Fort | cut | 139: untitled in PAL (JP ３姉妹のかくれんぼ), never triggered | evidence | JP X10 only, as 'the three sisters' hide and seek' |  | **none**: No description in any build. Retail id 139 has no hint text in any build (placeholder only). |
| 141 | 3姉妹への手紙 | A Letter to the Three Sisters | cut | 140: untitled in PAL (JP ３姉妹への手紙), never triggered | evidence | same JP title, JP X10 only; unused item 90 Letter |  | **none**: No description in any build. Retail id 140 has no hint text in any build (placeholder only). |
| 151 | イカダを作ろう! | Let's Build a Raft! | retail (renamed): Let's Ride the Raft! | 150: Let's Ride the Raft! (イカダにのろう！) | evidence | retail raft object in AREA10 s2; the same JP title is in the Civilization lists (the Factory builds it) |  | **retail EN**: Need a ride... |

### Village of Civilization towns (AREA11 / AREA16 / AREA17 / AREA18)

19 titles: 6 in retail, 13 cut; 13 evidence, 3 strong, 3 weak.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 66 | ナミダのビンちょうだい | Give Me a Tear Bottle | retail (same): I Need a Tear Bottle | 65: I Need a Tear Bottle (ナミダのビンちょうだい) | evidence | same JP title; retail AREA11 (the Bonsugee researcher) |  | **beta 980203**: Where is the Tear Bottle? *(earlier English wording)*<br>**retail EN**: Get the Tear Bottle from the Yellow Bonsugee. |
| 74 | 大停電 | The Big Blackout | retail (renamed): We Need Power... | 73: We Need Power... (キカイを動かすには…) | strong | retail id 73 is the power for the machines (AREA16 / AREA17; JP X16-X18): a blackout to fix |  | **retail EN**: Fix the clock tower. |
| 76 | 事件発生、犯人を探せ! | Incident! Find the Culprit! | cut | 75: untitled everywhere, never triggered; JP hint: "find the stolen Evil Pig Bag" | weak | between 74 and 77, both Civilization (75 is in both jungle and Civilization lists) |  | **retail EN**: Find the stolen Evil Pig Bag. |
| 77 | 黒い水の不思議 | The Mystery of the Black Water | cut | 76: PAL title #90 "The Black Water", started only in the cut Iron Town; JP hint: "a thing with dangerous power" | evidence | same JP title in X11, X16, X17, X18; only the cut Iron Town starts it |  | **retail EN**: Something with dangerous powers... |
| 78 | 文明のキカイ | The Civilization Machine | cut | 77: The Civilization Machine (文明のキカイ), never set; JP hint: "the revived machine" | evidence | same JP title in X17 / X18; read only by the cut town spawners |  | **retail EN**: The revived machine. |
| 90 | 緑の占い師 | The Green Fortune Teller | cut | 89: untitled in PAL (JP 緑の占い師), never triggered | evidence | same JP title in X11 / X18; the cut AREA11 data has the Green Fortune Teller's hut and his line |  | **retail EN**: Listen to the Green Fortune Teller's story. |
| 95 | 仲良くしようよ | Let's Get Along | cut | 94: untitled in PAL (JP 仲良くしようよ), never triggered | evidence | same JP title in X11, X16, X18 |  | **retail EN**: Can't we all just get along? |
| 96 | 木人の村の大岩 | The Big Rock of the Wood People's Village | cut | 95: untitled in PAL (JP same), never triggered | evidence | same JP title in X11 / X18; the Wood People are AREA11's |  | **retail EN**: A broken bridge blocks the way. |
| 97 | こわれた橋… | The Broken Bridge... | cut | 96: untitled in PAL (JP 未完の橋…), never triggered | evidence | JP X18 only, as 'the unfinished bridge' (text in the AREA11 list file) |  | **none**: No description in any build. Retail id 96 has no hint text in any build (placeholder only). |
| 98 | いま何時? | What Time Is It? | cut | 97: untitled in PAL (JP 動かない時計台); completed by the Clock Tower gears | strong | retail id 97 is the stopped clock tower of AREA16 (JP X16 / X18): same clock theme |  | **retail EN**: The clock that stopped. |
| 99 | こわれない歯車? | An Unbreakable Gear? | cut | 98: untitled in PAL (JP こわれない歯車？), started at the gears | evidence | same JP title in X16 / X18; retail code at the AREA16 gears |  | **retail EN**: Cogwheels make the clock tick. |
| 100 | 材料はどこだ!? | Where Are the Materials!? | cut | 99: untitled in PAL (JP 材料はどこだ！？), started in the cut Iron Town | evidence | same JP title in X17 / X18; the cut Iron Town smith starts it |  | **retail EN**: What's stronger than stone? |
| 101 | 大きなカギ穴 | The Big Keyhole | cut | 100: untitled in PAL (JP 鉄のカギ穴), never triggered | evidence | JP X11 / X18, as 'the iron keyhole' |  | **retail EN**: The door with the key hole that will not open. |
| 102 | どうやって取るの? | How Do I Get It? | cut | 101: untitled everywhere | weak | between 101 and 103, both Civilization |  | **none**: No description in any build. Retail id 101 has no hint text in any build (placeholder only). |
| 103 | さびたトビラをこわせ! | Break the Rusty Door! | retail (same): Break the Rusty Door! | 102: Break the Rusty Door! (さびたトビラをこわせ！) | evidence | same JP title; retail AREA17 (Iron Castle) |  | **beta 980203**: The door that is rusted shut. *(earlier English wording)*<br>**retail EN**: Find a way to open the rusted door. |
| 104 | 動かない鉄の人形 | The Iron Doll That Won't Move | cut, id reused for The Cute Witch | 103: The Cute Witch (かわいい？魔女ミズノ) | weak | between 103 and 105, both Civilization |  | **none**: No description in any build. Its id 103 became The Cute Witch in retail. *(its retail hint, for the other event: "Visit Mizuno in the Village of All Beginnings.")* |
| 105 | エサ燃料って何? | What's Feed Fuel? | retail (same): Food for Fuel? | 104: Food for Fuel? (エサ燃料って何？) | evidence | same JP title; retail AREA11 |  | **beta 980203**: Food for energy? *(earlier English wording)*<br>**retail EN**: Bring back a bucket of wine for the GoGoCar. |
| 106 | あぶない届け物 | A Dangerous Delivery | retail (renamed): I Need a Bomb | 105: I Need a Bomb (バクダンの作り方) | strong | retail id 105 is the bomb for the Iron Castle door (AREA11): a dangerous delivery |  | **beta 980203**: Something that goes, boom! *(earlier English wording)*<br>**retail EN**: A bomb that looks like a coconut? |
| 115 | 魔女の探し物 | What the Witch Is Looking For | retail (same): What the Witch Lost... | 114: What the Witch Lost... (魔女の探し物) | evidence | same JP title; retail AREA18 (witch Mizuno) |  | **Dengeki D5**: What is needed to make the medicine?? *(differs from retail)*<br>**beta 980203**: The witch is in deep trouble···? *(earlier English wording)*<br>**retail EN**: Find and return the Crystal Balls and Mirror to the witch. |

### Evil Pigs (AREA14)

1 titles: 1 in retail, 0 cut; 1 evidence.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 161 | 本当の魔ブタ | The Real Evil Pig | retail (same): The Real Evil Pig | 160: The Real Evil Pig (本当の魔ブタ) | evidence | same JP title; retail AREA14 s7 boss |  | **beta 980203**: Another Magic Pig inside the Magic Pig? ! *(earlier English wording)*<br>**retail EN**: Seal the powers of the final Evil Pig! |

### Pig Town: a place two titles name (no retail area)

2 titles: 0 in retail, 2 cut; 2 evidence.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 92 | ブタの町 | Pig Town | cut; Tomba! 2 has a similar idea (theme only) | 91: untitled everywhere; JP hint: "so light it floats up (unrelated)" | evidence | the title names Pig Town; no retail area, no other data | theme only: Pig Tribe / Clown Circus | **retail EN**: Those who are too light will float into the air. |
| 110 | ブタの町のブタメガネ | The Pig Glasses of Pig Town | cut; Tomba! 2 has a similar idea (theme only) | 109: untitled everywhere; JP hint: "glasses that see through anything" | evidence | the title names Pig Town; no retail area, no other data | theme only: Pig Nose (panel) | **retail EN**: Glasses that see through everything and anything. |

### Title cluster: the island (67, 69-73)

6 titles: 0 in retail, 6 cut; 6 unknown.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 67 | 盗まれた島の絵本 | The Stolen Island Picture Book | cut | 66: untitled everywhere, never triggered | unknown | cluster: consecutive with 69-73, all name 'the island'; no data |  | **none**: No description in any build. Retail id 66 has no hint text in any build (placeholder only). |
| 69 | 島のヒミツその1 | Secret of the Island, Part 1 | cut | 68: untitled everywhere | unknown | cluster: Secret of the Island 1-5 are consecutive; no data |  | **none**: No description in any build. Retail id 68 has no hint text in any build (placeholder only). |
| 70 | 島のヒミツその2 | Secret of the Island, Part 2 | cut | 69: untitled everywhere | unknown | cluster: Secret of the Island 1-5; no data |  | **none**: No description in any build. Retail id 69 has no hint text in any build (placeholder only). |
| 71 | 島のヒミツその3 | Secret of the Island, Part 3 | cut | 70: untitled everywhere | unknown | cluster: Secret of the Island 1-5; no data |  | **none**: No description in any build. Retail id 70 has no hint text in any build (placeholder only). |
| 72 | 島のヒミツその4 | Secret of the Island, Part 4 | cut | 71: untitled everywhere | unknown | cluster: Secret of the Island 1-5; no data |  | **none**: No description in any build. Retail id 71 has no hint text in any build (placeholder only). |
| 73 | 島のヒミツその5 | Secret of the Island, Part 5 | cut | 72: untitled everywhere | unknown | cluster: Secret of the Island 1-5; no data |  | **none**: No description in any build. Retail id 72 has no hint text in any build (placeholder only). |

### Title cluster: light, sun and cold (85-89)

5 titles: 0 in retail, 5 cut; 5 unknown.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 85 | 光がほしい | I Want Light | cut; Tomba! 2 has a similar idea (theme only) | 84: untitled everywhere | unknown | cluster 85-89: light, sun, cold, melting, snowball; no data | theme only: Too Dark to See! | **none**: No description in any build. Retail id 84 has no hint text in any build (placeholder only). |
| 86 | おひさまポカポカ… | The Sun Is Nice and Warm... | cut, id reused for the Deep Jungle pig bag | 85: The Deep Jungle Pig (ジャングル) | unknown | cluster 85-89; no data |  | **none**: No description in any build. Its id 85 became the Deep Jungle pig bag in retail. *(its retail hint, for the other event: "Seal the powers of the Evil Pig in the bag.")* |
| 87 | 寒い!裸? | Cold! Naked? | cut, id reused for Healing Herbs for Baron | 86: Healing Herbs for Baron (ケガに効く ナオリ草) | unknown | cluster 85-89; no data |  | **none**: No description in any build. Its id 86 became Healing Herbs for Baron in retail. *(its retail hint, for the other event: "Find the Healing Herbs to heal Baron's wounds...")* |
| 88 | 溶かす、溶ければ… | Melt It, and If It Melts... | cut, id reused for Delicious Knowledge Fruit; Tomba! 2 has a similar idea (theme only) | 87: Delicious Knowledge Fruit (おいしい百識の木の実) | unknown | cluster 85-89; no data | theme only: Melt the Giant Ice! | **Dengeki D5**: Melt it and a path appears. *(differs from retail)*<br>**retail EN, other event**: "Find the fruit from the Tree of Knowledge for the dwarf healer." *(the id's retail hint, for Delicious Knowledge Fruit)* |
| 89 | 雪玉ゴロゴロその先は… | Where the Rolling Snowball Goes... | cut, id reused for Seaweed for Your Health; Tomba! 2 has a similar idea (theme only) | 88: Seaweed for Your Health (海草でみんな元気) | unknown | cluster 85-89; no data | theme only: Collect Snow | **Dengeki D5**: Become a snowball and roll down. *(differs from retail)*<br>**retail EN, other event**: "Find the seaweed to keep the Dwarf Village healthy." *(the id's retail hint, for Seaweed for Your Health)* |

### Title cluster: sky (137-138)

2 titles: 0 in retail, 2 cut; 2 unknown.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 137 | おもいは地、かるいは天 | Heavy to the Earth, Light to the Sky | cut; Tomba! 2 has a similar idea (theme only) | 136: untitled everywhere; JP hint: "weight decides the path?" | unknown | cluster 137-138: 'light to the sky', the cloud; no data | theme only: Use the Seesaw! | **JP hint (translated)**: Weight decides the path you take? |
| 138 | 雲の中身 | What's Inside the Cloud | cut | 137: untitled everywhere; JP hint: "an important person in the clouds!?" | unknown | cluster 137-138; no data |  | **JP hint (translated)**: An important person inside the clouds!? |

### Title cluster: caves (143-144)

2 titles: 0 in retail, 2 cut; 2 unknown.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 143 | 毒毒どうくつのナゾ | Riddle of the Poison Cave | cut | 142: untitled everywhere | unknown | cluster 143-144: two caves; no data |  | **none**: No description in any build. Retail id 142 has no hint text in any build (placeholder only). |
| 144 | 水晶のどうくつ | The Crystal Cave | cut; Tomba! 2 has a similar idea (theme only) | 143: untitled everywhere; JP hint: "the magic fades and the three towns become one (the Civilization reunion)" | unknown | cluster 143-144; no data (its JP hint is the Civilization reunion) | theme only: Crystal Panel Blocks a Hole! | **beta 980203**: 3 towns come together when the spell is broken. *(earlier English wording)*<br>**retail EN**: Three towns come together when the spell is broken. |

### Title cluster: light (152-153)

2 titles: 0 in retail, 2 cut; 2 unknown.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 152 | 光と影 | Light and Shadow | cut, id reused for Take Out; Tomba! 2 has a similar idea (theme only) | 151: Take Out (息子よ、どこに…) | unknown | cluster 152-153: light; no data | theme only: Find Another Reflector! | **none**: No description in any build. Its id 151 became Take Out in retail. *(its retail hint, for the other event: "Bring Yan his lunch.")* |
| 153 | 光る石を探せ | Find the Glowing Stone | cut | 152: untitled everywhere | unknown | cluster 152-153; no data |  | **none**: No description in any build. Retail id 152 has no hint text in any build (placeholder only). |

### Title cluster: grand finale (166-168)

3 titles: 0 in retail, 3 cut; 3 unknown.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 166 | 「大団円1」 | "Grand Finale 1" | cut | 165: untitled everywhere; JP hint: "before you know it, everyone is happy!" | unknown | cluster 166-168: 'grand finale 1-3'; no data |  | **retail EN**: We are all happy now! |
| 167 | 「大団円2」 | "Grand Finale 2" | cut, id reused for A Hungry Monkey | 166: A Hungry Monkey (サンタの好きなもの…) | unknown | cluster 166-168; no data |  | **none**: No description in any build. Its id 166 became A Hungry Monkey in retail. *(its retail hint, for the other event: "Find some food for the monkey.")* |
| 168 | 「大団円3」 | "Grand Finale 3" | cut, id reused for Peach Flower Gas | 167: Peach Flower Gas (小さなブタ…マゴブタ？) | unknown | cluster 166-168; no data |  | **none**: No description in any build. Its id 167 became Peach Flower Gas in retail. *(its retail hint, for the other event: "A Baby Koma Pig?")* |

### Data for more than one area

6 titles: 3 in retail, 3 cut; 6 evidence.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 46 | APスロットマシーン | AP Slot Machine | cut | 45: untitled in PAL (JP ＡＰスロットマシーン), never triggered; JP hint: "the more AP, the better the item" | evidence | same JP title in two lists: Baccus (X05) and Dwarf Village (X02) |  | **retail EN**: The greater the AP points, the better the item. |
| 64 | ポンプ岩のナゾ | Riddle of the Pump Rocks | retail (same): The Pump Rocks | 63: The Pump Rocks (ポンプ岩のナゾ) | evidence | same JP title; retail code sets it in every area; JP lists X00, X01, X03, X09, X10 |  | **beta 980203**: All the Pump Rocks have secrets. *(earlier English wording)*<br>**retail EN**: Jump on all of the Pump Rocks to feed the Tree of Knowledge. |
| 68 | 解読?らくがき岩 | Decode the Graffiti Rock? | cut | 67: untitled in PAL (JP 解読？らくがき岩), never triggered; JP hint: "decode the letters of the graffiti rock" | evidence | same JP title in the forest (X01) and Civilization (X16, X18) lists; graffiti plaques exist in AREA01 / AREA04 rooms |  | **retail EN**: Decipher the code on the Doodle Rocks. |
| 75 | 電気うなぎを捕まえろ! | Catch the Electric Eel! | cut | 74: untitled in PAL (JP 電気うなぎを捕まえろ！), never triggered | evidence | same JP title in the jungle (X10) and Iron Town / Civilization (X17, X18) lists; unused item 108 Electric Eel |  | **retail EN**: Catch the dangerous electric eel. |
| 117 | 道具が強くなる!? | Tools Get Stronger!? | retail (same): Power Up for Tools?! | 116: Power Up for Tools?! (道具が強くなる！？) | evidence | same JP title; retail code in AREA10 and AREA18 (JP X10 / X18) |  | **beta 980203**: Tool receive a power up!? *(earlier English wording)*<br>**retail EN**: Tool receives a power up!? |
| 120 | おいしい?万能魚 | Tasty? The Mighty Fish | retail (same): Mighty Fish Food | 119: Mighty Fish Food (おいしい？万能魚) | evidence | same JP title; retail code in AREA00, AREA04, AREA10, AREA13 |  | **beta 980203**: Is that fish edible? *(earlier English wording)*<br>**retail EN**: Catch a Mighty Fish? |

### Area unknown

27 titles: 0 in retail, 27 cut; 27 unknown.

| # | Japanese | English | Fate | Retail id: title (JP retail) | Level | Argument | Tomba! 2 | Description (source) |
|---|---|---|---|---|---|---|---|---|
| 0 | 少年の旅立ち | A Young Boy Sets Off | cut | none: no retail id (retail ids start at PrePre 8 index 1) | unknown | no retail id and no area data; the title (the boy sets off) only reads as the opening |  | **none**: No description in any build. No retail id: the list starts with the boy setting off. |
| 29 | 大脱出 | The Great Escape | cut, id reused for Familiar Looking Mansion | 28: Familiar Looking Mansion (見た事あるやしき) | unknown | its neighbours are forest (28) and Phoenix Mountain (30); the theme (an escape) is all there is |  | **Dengeki D5**: The door has shut and you cannot get out. *(differs from retail)*<br>**retail EN, other event**: "I've seen this mansion before...but where?" *(the id's retail hint, for Familiar Looking Mansion)* |
| 37 | そこ、どいて | Move Over, Please | cut, id reused for What's a Funga? | 36: What's a Funga? (フンガのタイコ) | unknown | neighbours Phoenix Mountain (36) and Mushroom Forest (38); no link to the Funga event shown |  | **none**: No description in any build. Its id 36 became What's a Funga? in retail. *(its retail hint, for the other event: "Bring him a Funga.")* |
| 79 | 進め!きかんしゃ | Go, Locomotive! | cut, id reused for Find Charles!; Tomba! 2 has a similar idea (theme only) | 78: Find Charles! (サンタを探せ！) | unknown | neighbours Civilization (78) and forest (80); the machine theme is all there is | theme only: Quick! The Trolley! | **none**: No description in any build. Its id 78 became Find Charles! in retail. *(its retail hint, for the other event: "Find Charles and return the miner's hat")* |
| 84 | お城の食事会 | The Castle Banquet | cut; Tomba! 2 has a similar idea (theme only) | 83: PAL #126 "The Rusty Steel Car" (JP 動かない鉄の車), never triggered | unknown | Dengeki's hint for id 83 is still a dinner party, but no area; neighbours forest and the cold cluster | theme only: Cooking with Tomba! | **Dengeki D5**: A dinner party with lots of feast. *(differs from retail)*<br>**JP hint (translated)**: How do you move the iron car? *(retail JP, for the later event The Rusty Steel Car)* |
| 93 | 白いケムリの家 | The House of White Smoke | cut | 92: untitled everywhere | unknown | next to Pig Town (92) on one side only; no data |  | **none**: No description in any build. Retail id 92 has no hint text in any build (placeholder only). |
| 94 | コトバをおぼえよう!Ⅲ | Let's Learn the Language! III | cut; Tomba! 2 has a similar idea (theme only) | 93: untitled everywhere | unknown | theme only (a third language, after the dwarves and the Masakari) | theme only: Use Pig Suit to Talk! | **none**: No description in any build. Retail id 93 has no hint text in any build (placeholder only). |
| 107 | 森の案内人 | The Forest Guide | cut | 106: untitled everywhere; JP hint: "nobody beats me in a footrace!" | unknown | neighbours Civilization (106) and Pig Town (110, three titles on); no data |  | **retail EN**: Who's the fastest of them all? |
| 108 | うまい果実、まずい果実 | Tasty Fruit, Nasty Fruit | cut; Tomba! 2 has a similar idea (theme only) | 107: untitled everywhere | unknown | no data | theme only: Normal Fruit / Strange Fruit | **none**: No description in any build. Retail id 107 has no hint text in any build (placeholder only). |
| 109 | 挑戦!勇気の大樹 | Challenge! The Great Tree of Courage | cut; Tomba! 2 has a similar idea (theme only) | 108: untitled everywhere | unknown | no data | theme only: Mythical Tower of Courage | **none**: No description in any build. Retail id 108 has no hint text in any build (placeholder only). |
| 111 | 王の本性 | The King's True Nature | cut | 110: untitled everywhere | unknown | next to Pig Town (110) on one side only; no data |  | **none**: No description in any build. Retail id 110 has no hint text in any build (placeholder only). |
| 113 | 固める、止まった時に… | Harden It, When Time Stops... | cut, id reused for The Mermaid's Necklace; Tomba! 2 has a similar idea (theme only) | 112: The Mermaid's Necklace (人魚の思い出) | unknown | neighbours area 0 (112) and forest (114); theme only | theme only: Sea Anemone's Ice | **Dengeki D5**: A hidden power that keeps things from moving. *(differs from retail)*<br>**retail EN, other event**: "A beautiful necklace that belongs to the Mermaid." *(the id's retail hint, for The Mermaid's Necklace)* |
| 116 | 花よりダンゴ | Dumplings over Flowers | cut, id reused for A Safe Mushroom? | 115: A Safe Mushroom? (まともなキノコ) | unknown | neighbours Civilization (115) and two areas (117); no data |  | **Dengeki D5**: Words set something moving! *(the id's hint in November 1997; it does not echo the title; differs from retail)*<br>**retail EN, other event**: "A mushroom that returns you to normal." *(the id's retail hint, for A Safe Mushroom?)* |
| 118 | トンネルはどこだ | Where's the Tunnel? | cut | 117: untitled everywhere; JP hint: "where is the new road..." | unknown | neighbours 117 (two areas) and 119 (jungle); JP hint 'where is the new road' only |  | **retail EN**: Where does the new road lead... |
| 121 | トビウオ アクロバット | Flying Fish Acrobatics | cut, id reused for Let's Make Candy! | 120: Let's Make Candy! (ドロップを作ろう！) | unknown | next to the mermaid (122) on one side only; theme (sea) only |  | **Dengeki D5**: Win the contest and you get something good. *(differs from retail)*<br>**retail EN, other event**: "Make magic candy by combining the six items!" *(the id's retail hint, for Let's Make Candy!)* |
| 123 | 海族の宝物 | Treasure of the Sea Folk | cut | 122: untitled everywhere; JP hint "find the pirates' treasure" | unknown | next to the mermaid (122) on one side only; JP hint 'the pirates' treasure' |  | **retail EN**: Find the thief's treasures!! |
| 128 | 体力を上げましょー | Let's Build Up Stamina | cut, id reused for The 5 Golden Items | 127: The 5 Golden Items (黄金の５大アイテム) | unknown | neighbours jungle (127) and forest (129); no data |  | **none**: No description in any build. Its id 127 became The 5 Golden Items in retail. *(its retail hint, for the other event: "Collect the five rare items.")* |
| 130 | 風になりたい | I Want to Be the Wind | cut, id reused for Green Hidden Powers | 129: Green Hidden Powers (かくされた力その３) | unknown | the same title survives unused in JP AREA19's list (no table entry); neighbours 129 (forest) and 134 (mermaid) |  | **none**: No description in any build. Its id 129 became Green Hidden Powers in retail. *(its retail hint, for the other event: "Find the Jewel of Wind to unleash your remaining hidden powers.")* |
| 131 | 危ない!落ちたら死ぬぞ | Careful! Fall and You Die | cut | 130: untitled everywhere; JP hint: "make up your mind and cross!" | unknown | no data |  | **JP hint (translated)**: Make up your mind and cross, come on! |
| 132 | ほこらの中に何がある? | What's in the Shrine? | cut | 131: untitled everywhere | unknown | no data |  | **none**: No description in any build. Retail id 131 has no hint text in any build (placeholder only). |
| 133 | 世紀の大発明 | The Invention of the Century | cut, id reused for Take Two of These (the witch's cold) | 132: Take Two of These (魔女がカゼひいた…) | unknown | theme (an invention) only |  | **none**: No description in any build. Its id 132 became Take Two of These (the witch's cold) in retail. *(its retail hint, for the other event: "The cute witch needs something for her cold.")* |
| 142 | 雪がふったら… | When the Snow Falls... | cut; Tomba! 2 has a similar idea (theme only) | 141: untitled everywhere | unknown | snow theme like 87-89, but not next to them; no data | theme only: Snow Fireflies | **none**: No description in any build. Retail id 141 has no hint text in any build (placeholder only). |
| 148 | 水を止めなきゃ! | I Have to Stop the Water! | cut; Tomba! 2 has a similar idea (theme only) | 147: untitled everywhere | unknown | neighbours forest (147) and none before the jungle (151); no data | theme only: Pour the Water In! | **none**: No description in any build. Retail id 147 has no hint text in any build (placeholder only). |
| 149 | 塔のもりびと | The Tower Keeper | cut; Tomba! 2 has a similar idea (theme only) | 148: untitled everywhere | unknown | no data | theme only: Mythical Towers | **none**: No description in any build. Retail id 148 has no hint text in any build (placeholder only). |
| 150 | 海を渡る方法 | How to Cross the Sea | cut | 149: untitled everywhere | unknown | neighbours forest (147) and jungle (151); no data |  | **none**: No description in any build. Retail id 149 has no hint text in any build (placeholder only). |
| 155 | 人魚の王を救え | Save the Mermaid King | cut; Tomba! 2 has a similar idea (theme only) | 154: untitled everywhere | unknown | neighbours mansion (154) and forest (157); theme (sea) only | theme only: A Mermaid in the Water Tank! | **none**: No description in any build. Retail id 154 has no hint text in any build (placeholder only). |
| 156 | 海底通路の行き先 | Where the Undersea Passage Leads | cut | 155: untitled everywhere | unknown | theme (sea) only |  | **none**: No description in any build. Retail id 155 has no hint text in any build (placeholder only). |

## Tomba! 2

Checked directly: the Tomba! 2 USA disc (SCUS-94454) has its event, item and place names as plain ASCII in
`MAIN.EXE`. No JP Tomba! 2 disc was available, so the matches compare English T2 titles with Japanese T1 titles.
**same name** = the T2 title says the same thing; **theme only** = the same theme or mechanic, nothing more: it
is not evidence that the content moved. The T2 names below were re-read from `MAIN.EXE` on 2026-11-27.

| PrePre 8 | T2 (USA) | Match | Notes |
|---|---|---|---|
| 8 かくれ里のヤン | "Yan of the Hidden Village" (event) and "Hidden Village" (place) | **same name** | T1 retail renamed it Hide and Go Seek in English; JP T1 kept the title |
| 28 動かない恵みコゾウ | "Unmoving Blessed Priest" | **same name** | also T1 retail id 27 (The Broken Fountain); T2 reuses the title |
| 44 泣き笑いのドア | "The Crying Door", "The Laughing Door"; Crying Forest | **close name** | retail T1 has a laughing door (Smile!, AREA03 s1) and a crying door (Cry Baby, AREA04 s0) |
| 126 水門をあけろ! | "Open the Water Gate" | **same name** | retail T1 reused the id for The Thief's Door |
| 85, 88, 89, 113, 142, 144 (cold area) | Kujara Ranch: "Collect Snow", "Collect Snow Fireflies", Evil Ice Pig, "Melt the Giant Ice!", "Sea Anemone's Ice" / "Burning Dregs" | theme only | 88 溶かす vs "Melt the Giant Ice!" is the closest pair |
| 85 光がほしい, 152 光と影 | "Too Dark to See!", "Find Another Reflector!", Reflector | theme only | |
| 92, 94, 110 (Pig Town) | Pig Tribe, "A Pig Tribe Clown Statue!", Clown Circus, "Use Pig Suit to Talk!", Pig Nose (a panel "with images of pig noses") | theme only | Language III = pig language |
| 109 挑戦!勇気の大樹, 149 塔のもりびと | Mythical Towers of Wisdom / Strength / Courage | theme only | |
| 79 進め!きかんしゃ | "Quick! The Trolley!", Turbo Trolley, Coal-Mining Town | theme only | |
| 105 エサ燃料って何? | "The Fuel that Burns Well" (Power Coal) | theme only | also T1 retail (Food for Fuel?) |
| 99 こわれない歯車? | round / triangle / hexagon gear wheels, "Wind it Up!" | theme only | also a T1 Civilization event |
| 84 お城の食事会 | "Cooking with Tomba!", "The Cook's Special Menu" | theme only (loose) | |
| 122 歌を忘れた人魚, 155 人魚の王を救え | "The Broken-Up Mermaid Harp!", "A Mermaid in the Water Tank!", Water Temple | theme only | |
| 124 水に沈んだ村 | Water Temple | theme only (loose) | |
| 47 子ども?大人? | Tiny Tomba / Mini Tomba, Minitta Mushroom | theme only (loose) | shrinking |
| 108 うまい果実、まずい果実 | Normal Fruit / Strange Fruit | theme only (loose) | |
| 137 おもいは地、かるいは天 | "Use the Seesaw!", "Use Rock Crabs for Balance!" | theme only (loose) | weight puzzle |
| 144 水晶のどうくつ | "Crystal Panel Blocks a Hole!", Large Mine | theme only (loose) | |
| 158 友達10人 | "Proof of friendship" items from the 100-Year-Old Man, Yan, Mizuno, the Dwarf | theme only (loose) | |
| 159 金色のカイロウ, 162 黄金郷 | Golden Tower | theme only (loose) | |
| 24 / 114 Baron | "Baron Turned to Stone" | T1 character reused | also T1 retail |
| Candies | "Collect Ice Candy" (item Ice Candy, "a carnival favorite"), "Witch's Concoction" | theme only | see Candies |

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

- Every **weak** row: position only, right about three times in four on the retail-matched titles.
- Every **strong** row: the retail id and area are data, the shared theme is a reading.
- 43 涙が止まらない: the same title as retail 43 (Baccus), against the id rule's 42 (the mansion's crying door).
- The demos' area blocks and the JP tables are title lists: a title in a list shows where the event could be titled,
  and the lists are only about three-quarters per area (see above).
- The five kanji read from their slot alone (雲, 砦, 毒, 勇, 裸): plausible readings, not confirmed by a retail title.
- The JP title tables of AREA01 and AREA18 were read through the AREA13 and AREA11 lists (tools/jp_text.py skips the
  AREA01/A008 and AREA18/A003 title blocks); where those lists differ, the JP title shown was picked by hand.
- Tomba! 2: English USA titles only; "theme only" matches are not proof that the content moved.
