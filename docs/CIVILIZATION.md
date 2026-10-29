# The cut top-down towns of the Land of Civilization

*Versión en español: [CIVILIZACION.md](CIVILIZACION.md).*

Section 0 of AREA11, AREA16, AREA17 and AREA18 was meant to be a top-down town. It would use the same engine as the
Baccus Village and Baccus Lake overviews (`SYSTEM/I0NFO2.BIN`). The code for these towns is on the PAL disc and fully
matched in `src/i0nfo2/`. The NPC and prop placements are also on the disc, as data inside I0NFO2. The section
data is missing: the load lists are empty, so there is no picture, cell map or sprite table. The resident palettes, however, survive in the areas' CLUT files (see Palettes). This page
covers two things:

1. Every data format the top-down engine reads. Each one is checked against a working section with
   `tools/topdown.py`.
2. What each cut section expected: its actors, behaviours, sprite tables, flags, events, exits and Japanese dialogue.

Like `game/`, nothing taken from the disc is committed: no images, text dumps or extracted blocks. Run the tool on
your own copy of the disc.

Addresses: MAIN0 is PAL `SCES_013.31`. I0NFO2 functions and data are at their run address (base `0x800E8028`, file
`SYSTEM/I0NFO2.BIN`, 0x218DC bytes, 196 functions, 100 % matched). Text references are given as `(k, dx)`, the bank
and the index passed to the message functions. The dialogue number shown on screen is `base[k] + dx`, where `base[k]`
is the u16 table at `WFM + 0x10`. For the town WFMs these bases are:

| File | k=1 | k=2 | k=3 | k=4 | k=5 |
|---|---|---|---|---|---|
| `AREA11/MFNT110S.WFM` | 207 | 208 | 227 | 249 | - |
| `AREA16/MFNT160S.WFM` | 207 | 208 | 230 | 248 | - |
| `AREA17/MFNT170S.WFM` | 207 | 208 | 257 | 269 | 270 |
| `AREA18/MFNT180S.WFM` | 207 | 209 | 223 | - | - |

## Which places these areas are

The README calls areas 11/16/17/18 "Village of Civilization (…)". The retail text gives better names (save-point
captions are dialogue 207 of each area's `MFNT*S.WFM`):

| Area | Retail place (PAL Spanish text) | Evidence |
|---|---|---|
| AREA18 | **Ciudad de civilización**, the hub (Civilization Town) | save caption #207 "Ciudad de civilización"; signpost #208 "↑ Castillo de hierro → Torre reloj ↓ Serrería" (Iron Castle, Clock Tower, Sawmill); Mizuno the witch #223-#237; exits of s1 lead to AREA11 s1, AREA17 s1, AREA16 s4 and AREA10 |
| AREA16 | **Ciudad de Piedra**, the Stone People's quarter with the **Torre reloj** (Clock Tower) | #207 "Ciudad de Piedra"; #213-#223 clock tower stopped / repaired by the Miner; #248 "Los piñones de piedra están rotos" |
| AREA17 | **Ciudad de hierro**, the Iron Men's quarter with the **Castillo de hierro** (Iron Castle) | #207 "Ciudad de hierro"; #220 "the gear room"; #240-#247 the Evil Pig bag, the 10,000-year Old Man, the raft "en la fábrica" |
| AREA11 | the **Fábrica** (Factory) and the power station, on the Wood People's side | #220 "Estoy al mando de esta fábrica"; #211 coal; #212 "la central eléctrica funciona de nuevo"; #218 the GoGo car; #221-#226 the bomb for the Iron Castle door; #229-#233 the Funga drum |

The landmarks mentioned in the brief:

- **Torre reloj** (Clock Tower) is in AREA16 (#213/#214/#220/#221/#223). AREA10 #258 calls it "la Torre del reloj"
  and AREA18 #208 points to it.
- **Torre de vigilancia** (Watchtower) is not in this group. It is a save point of AREA01/AREA07 (`MFNT010S` #211,
  Bosque de las 100 flores / Dwarf Forest) and is only mentioned from here (AREA11 #213 "un ascensor cerca de la Torre
  de vigilancia en el Bosque de Enanos", AREA18 #225).
- **10,000-year and million-year bells** ("Campana 10.000 años" / "Campana 1 millón años") are item names. They sit
  at #154/#155 of every area's MFNT (the shared item list), so they do not place anything. The 10,000-year Old Man
  lives "near the pool above the waterfall" (AREA17 #243).
- **Máquina de civilización** (Civilization Machine) is an event title (`CFNT999S.WFM` #95, the shared event list),
  next to "¡Rompe la puerta oxidada!" #93, "Necesito una bomba" #94 and "¡Vamos en la balsa!" #97. These are the
  Factory / Iron Castle / raft quests of AREA11/17.

The Japanese text tells us what the cut towns were. AREA11 k4 is the **Wood People's** (木人) village ("wood really
is the best", the raft craftsman's hut, a maze under the floor). AREA16 k3 is the **Stone People's** (石人) town
(the elevator on the wall, the clock tower gear). AREA17 k5 is the **Iron Men's** (鉄人) town ("Welcome... the Iron
Men's town", the lift, Dr. Gantetsu, the Iron Castle). AREA18 k2 is the **reunited town**, after the Evil Pig's wall
falls ("we could all make one country together again... thanks to Tomba", "more than four times the size of the old
Stone People's town"). So the four top-down sections are the three feuding villages plus the epilogue town. The retail
game replaced them with the side-scrolling quarters.

## Step 1: the data formats

### Where the engine runs

The top-down engine replaces the area overlay: the section list loads `I0NFO2.BIN` (kind 0xF0) at the overlay base
instead of `X0nn.BIN`. Retail sections using it, and the sections it was written for:

| Section | Load list (`SYS/LD0ARnn` list s+1) | NPC set (unlayered variant) | Section controller (`D_80079AD0[area+7]`) | Actor / scenery handler |
|---|---|---|---|---|
| AREA05 s0, s2 (Baccus Village) | complete | 0x07 `updateNpcSet07` | `topdownSectionStepA` 0x800E8B28 | `topdownActorHandler` 0x800F05B0 / `topdownA05SceneryHandler` 0x800F07C0 |
| AREA08 s0, s2 (Baccus Lake) | complete | 0x0C `updateNpcSet0C` | `topdownSectionStepB` 0x800E8C98 | `topdownA08ActorHandler` 0x800F18C0 / `topdownA08SceneryHandler` 0x800F1A60 |
| AREA19 s2 (Ciudad oculta) | complete | 0x11 `updateNpcSet11` | `topdownSectionStepBySection` 0x800E9174 | `topdownA19ActorHandler` 0x800F6B4C / `topdownA19SceneryHandler` 0x800F1590 |
| **AREA11 s0** | **empty** | 0x0D `updateNpcSet0D` | `topdownSectionStepC` 0x800E8D98 | `topdownA11ActorHandler` 0x800F2CA8 / `topdownA11SceneryHandler` 0x800F6D5C |
| **AREA16 s0** | **empty** | 0x0E `updateNpcSet0E` | `topdownSectionStepD` 0x800E8ED4 | `topdownA16ActorHandler` 0x800F3D84 / `topdownA16SceneryHandler` 0x800F0C0C |
| **AREA17 s0** | **empty** | 0x0F `updateNpcSet0F` | `topdownSectionStepE` 0x800E9040 | `topdownA17ActorHandler` 0x800F528C / `topdownA17SceneryHandler` 0x800F126C |
| **AREA18 s0** | **empty** | 0x10 `updateNpcSet10` | `0x80116234`: not an I0NFO2 function (see AREA18) | `topdownA18ActorHandler` 0x800F548C / none |

How the pieces are wired, all in MAIN0:

- **Section objects.** `spawnSectionUnlayeredObjects` (0x8004BFE4) walks `PTR_8007BEE4[area][section]`, a list of
  4-byte entries `{type, variant, ?, init}`. Every top-down section lists type 2 (spawner), type 1 (section/fade
  controller) and type 4 with the NPC set variant. Type 4 is `dispatchUnlayeredByVariant` (0x8004D3B8), which calls
  `D_8007C1C8[variant]`: entry 7 is `updateNpcSet07`, entries 0x0C..0x11 are `updateNpcSet0C..11` (I0NFO2).
- **Section controller.** The controller's subtype indexes `D_80079AD0`. The dispatchers sit at entries 9 (AREA02),
  12, 15, 18, 23, 24, 25 and 26, which is `area + 7` for every area that has one. We inferred this from the table
  layout; it is consistent with what each dispatcher calls. Entries 12..26 are the
  per-area dispatchers named `dispatchSectionUpdate/Draw/Input/Scroll/Select/Close/Confirm` in `src/`. Those names do
  not describe what they do: each one picks the I0NFO2 step function for section 0 (and 2) and the X0nn function for
  the other sections. The step function checks the exit rectangles and calls `tryStartItemInteraction` (0x800270A0,
  which arms exit n). Then `cameraEventScrollHandler` (0x8002A4D0) reads the exit record from `D_80079A48`.
- **Actors.** Layer 2 type 0x30 is `updateL2Type30Object` (0x8003BCD0), which switches on the area to the
  `topdownAnnActorHandler` above.
- **Scenery and props.** Layer 3 type 0x1D is the MAIN0 function named `dispatchAreaDialogInit` (0x8002E414). That
  name is also misleading: it is the per-area scenery handler switch.
- **Map, player and shadow.** The map object is layer 7 type 0x0A (`topdownMapObjHandler` 0x800EDA90). The player's
  shadow is layer 3 type 0x3A (`topdownShadowHandler` 0x800E94AC). Item spots are layer 2 type 0x2D and talk spots
  layer 2 type 0x2E.

### Section load list

What a working top-down section loads (AREA08 s0 shown, the others follow the same pattern). Run
`tools/topdown.py lists` to see them all:

| Kind | Slot pointer | Size (AREA05 / AREA08 / AREA19) | Content |
|---|---|---|---|
| 0x10 GRPX | VRAM | B000.., CLUTnn | texture pages at x = 0x40.. and the 256 x 32 CLUT block at (128, 480) |
| 0x8E | `APD_DATA[14]` = scratch `0x1F800390` | 0x4008 each | cell map |
| 0x8F | `APD_DATA[15]` = scratch `0x1F800394` | 0x80620 / 0x3BA20 / 0x2EA20 | the map picture (TIM) |
| 0x62 | `WPP_DATA[2]` | 0xCA3C, the same bytes in every section | Tombi's top-down frames |
| 0x3A | `SPR_DATA[10]` | 0x11DC, the same in every section | Tombi's top-down sprite table |
| 0x3B..0x3E | `SPR_DATA[11..14]` | depends on the area | actor and scenery sprite tables (selected by the `spad` field of the defs) |
| 0x80+n | `APD_DATA[n]` | | placement records (treasure chests, AREA08 lamp posts) |
| 0xF0 | | 0x218DC | `I0NFO2.BIN` |

In AREA08/19, the 8E/8F/62 blocks are sub-records (`type 0xFFFFFFFF`) of one RAW_4 `.000` file. In AREA05 they are
separate `.000` files. Either way the loader stores them by kind, so file packing does not matter.

### Cell map (kind 0x8E): collision and triggers

`topdownLoadMapTiles` (0x800ECA74) reads `*(u16 **)0x1F800390`:

```
u16 w, h          map size in 8 x 8 px cells (AREA05 110 x 77, AREA08 68 x 56, AREA19 48 x 62)
u8  cell[0x4000]  copied to TOPDOWN_MAP_TILES, row-major, w cells per row (w*h <= 0x4000)
```

It also fills `D_800A4668` (`{tp0, clut0, tp1, clut1, tp2, clut2, w, h, w>>2, h>>2, tim, tim+0x220}`):
`D_800A4674` = w, `D_800A4678` = w>>2 (the picture width in 32-pixel blocks), `D_800A4680` = the TIM pixels.

The world-to-cell mapping (`topdownCollideMapTile` 0x800EE18C, `topdownCheckTileTrigger` 0x800EE440):

```
cell = TOPDOWN_MAP_TILES[((x + 0x80) >> 3) + ((y - 0x28) >> 3) * w]   collision (feet)
trig = TOPDOWN_MAP_TILES[((x + 0x80) >> 3) + ((y - 0x30) >> 3) * w]   trigger test, 8 px higher
```

The picture uses the same origin: world (x, y) is picture pixel (x + 128, y - 40). The renders below confirm that
fences and deck edges line up with the walls.

The low 7 bits of a cell give its shape. Bit 7 marks a trigger cell. In the code, `x`, `y` are `& 7` inside the cell
and `a`/`y` are the object's x/y.

| Value | Shape | Push applied (`f & 2` = moving horizontally) |
|---|---|---|
| 0 | free | none (FORMATS.md said "0 = not walkable": that is wrong, 0 is open floor) |
| 1 | diagonal, solid below the "/" line `y >= 7-x` | pushed back along the axis of movement |
| 2 | diagonal, solid above `y <= 7-x` | idem |
| 3 | diagonal "\", solid above `y <= x` | idem |
| 4 | diagonal "\", solid below `y >= x` | idem |
| 5 | wall: push left by `x` | `x -= x&7` |
| 6 | wall: push up by `y` | `y -= y&7` |
| 7 | wall: push right to the next cell | `x += 7 - x&7` |
| 8 | wall: push down to the next cell | `y += 7 - y&7` |
| 9 | corner / solid block: `topdownResolveCornerTile` (0x800EE05C) moves the probe to the nearest edge of the quadrant, and the cell found there decides | |
| bit 7 | trigger cell: sets `o->b68 = 1` for the player | |

**Triggers and rooms.** When the player stands on a trigger cell, `topdownPlayerMainStep` tests their position against
`TOPDOWN_TRIGGER_RECT_LISTS` (0x800FEC9C, one list per area: AREA05, 11, 16, 17, 19; `{x0, y0, x1, y1}` world
rectangles, ended by 0xFF). Inside rectangle i, the current room becomes i + 1 (`D_800A60A3`). Every actor, prop and
scenery object then compares its room (`wb6`, the record's `room` field) with `D_800A60A3`. Objects in the current
room are drawn (`b6b = 0x80`), all others are hidden. With `D_800A60A3 = 0` only room-0 objects are drawn. This is how
house interiors work: the roof is a room-0 prop and the furniture and occupants are room-n objects. The map layer is
hidden whenever a room is active (`topdownRefreshMap` sets `b6b = 0` if `D_800A60A3 != 0`). AREA19 s2 shows it: its
three trigger patches cover the three house doors.

### Map picture (kind 0x8F)

The picture is a plain 8 bpp TIM: `10 00 00 00 09 00 00 00`, then the CLUT block (`0x20C` bytes, x=0 y=0 w=256 h=1),
then the image block (`len, x, y, w words, h`). The game only uses fixed offsets: the CLUT at `+0x14` (`LoadClut` to
VRAM (0x80, 0x1FE)) and the pixels at `+0x220`. The picture is `(w>>2)*32` x `(h>>2)*32` px: AREA05 864 x 608,
AREA08 544 x 448. AREA19 is 384 x 496, 16 rows more than the cell map needs. Its own 256-colour CLUT makes it
independent of the area's CLUTnn.

The picture stays in RAM and is streamed to VRAM. `topdownBuildVisibleTileList` (0x800ED7EC) lists the 12 x 10 blocks
of 32 x 32 px around the camera. `topdownBuildMapStrip` / `topdownUploadMapStrip` (0x800ECC18 / 0x800ED380) copy five
strips into `TOPDOWN_MAP_STRIP_BUFFER` and `LoadImage` them:

| Strip | Source | VRAM (8 bpp, in 16-bit words) |
|---|---|---|
| 0 | 256 rows x 64 words | (0x300, 0) |
| 1 | 256 rows x 32 words | (0x380, 0) |
| 2..4 | 32 rows x 32 words each | AREA05: (0x40, 0x180+y); `GAME == 0x20013` (AREA19 s2): (0x40, y); others: (0, 0x100+y) |

`topdownDrawMapLayers` (0x800EC710) draws them as five `SPRT` (code 0x65) with `TOPDOWN_MAP_LAYER_RECTS` (0x800FECB8),
each with its texture page and clut0..2. This VRAM layout is what an authored section must leave free: the town's
own pages must not overlap (0x300..0x3FF, 0..255) or the strip-2..4 area.

### Actors (layer 2 type 0x30) and props (layer 3 type 0x1D)

The towns do not use APD records for their people. `spawnNpcSet0D/0E/0F/10/11` (0x800FB054...) read two lists from
I0NFO2. `tools/topdown.py npcs` dumps them.

`SpawnA`, 28 bytes, actors, ended by type 0xFF. Each becomes an `allocObjectLayer2` object of type 0x30 with
`b0a = 0xD`, stored in `SCRIPT_OBJECTS[i]`:

| Offset | Field | Object |
|---|---|---|
| 0 | type (0..2) | `_pad0e[0]` |
| 2 | room | `wb6` (0 = outdoors, n = inside house n) |
| 4 | frame | `animFrame` (facing: 0..3) |
| 6 | anim set | `wb4` (index into the def's anim list at init) |
| 8..E | patrol box x0, y0, x1, y1 | `w74..w7a` (absolute in sets 0D/0F/10/11, relative to x, y in 0E) |
| 10 | dialogue case | `wba` (selects the `(k, dx)` in `topdownAnnActorOpenDialogue`) |
| 12 | behaviour | `wb8`: 0 idle, 1 vertical patrol, 2 horizontal patrol, 3 / 4 diagonal patrols, 5 / 6 scripted (nothing) |
| 14, 16 | x, y | world position |
| 18 | subtype | index into `TOPDOWN_Ann_ACTOR_DEFS` |
| 1A | b0c | spawn condition (set 0E) / dialogue table row |

`SpawnB`, 16 bytes, props and scenery (`ObjAlloc` layer 3, type 0x1D): type, room, -, anim, x, y, subtype, b0c.
The 16th prop (index 15) adds `D_8009CFA1` to its anim, which is a progress-dependent prop.

Actor definitions, 12 bytes each, `{u8 box[4]; u16 w1e; s16 spad; void **anims}` (`topdownAnnActorInit`).
`box` is the contact box, `w1e` the CLUT/page offset word, `spad` the `SPR_DATA` slot, and `anims` a table of animation
scripts. The scripts use the script index `animFrame + k`: +0..3 idle facing, +4..7 walking, +8..11 talking
(`TalkStep` uses `b69 + 7`):

| Table | Subtypes | `spad` | `w1e` per subtype |
|---|---|---|---|
| `TOPDOWN_A11_ACTOR_DEFS` 0x800FEEB8 | 5 | 11 (kind 0x3B) | 0x01, 0x0A, 0x0B, 0x10, 0x11 |
| `TOPDOWN_A16_ACTOR_DEFS` 0x800FEEF4 | 4 | 11 | 0x01, 0x01, 0x01, 0x11 (boxes 8/16 and 16/32) |
| `TOPDOWN_A17_ACTOR_DEFS` 0x800FEF64 | 5 | 11 | 0x01, 0x0A, 0x0B, 0x10, 0x08 (subtype 4 has no anim table) |
| `TOPDOWN_A18_ACTOR_DEFS` 0x800FEFA0 | 7 | 11 | all 0x01 |
| `TOPDOWN_A11/16/17_SCENERY_DEFS` 0x800FF054 / 0x800FEDD0 / 0x800FEE04 | 2 / 3 / 3 | 13 (kind 0x3D) | 0x02.. 0x08 |

`tools/topdown.py defs` prints the full table, with the anim table addresses and script counts.

### Sprite tables (kind 0x3B..0x3E, SPR)

The tables use the usual SPR layout (FORMATS.md, "SPR"): `{u16 parts, u16 offset}` per frame, then 16-byte parts
holding u/v, CLUT, page. Tombi's table (0x3A) and the WPP (0x62) are identical in every top-down section and can be
reused as they are. The actor table (0x3B, `SPR_DATA[11]`) and the scenery table (0x3D, `SPR_DATA[13]`) are
town-specific. The anim scripts in I0NFO2 index frames of these tables, so the frame numbering is fixed by the code
even though the tables themselves are lost.

### Palettes

- The picture brings its own 256-colour CLUT inside the TIM.
- Sprites use the area CLUT block (`CLUTnn.GAM`, 256 x 32 words at (128, 480)). The SPR part's CLUT word picks a
  row, and `w1e` adds an offset.
- **The resident palettes survive.** A CLUT row here is 16 u16 entries (file offset row * 32 of `CLUTnn.GAM`; row r
  sits in VRAM at (128 + (r % 16) * 16, 480 + r / 16)). Rows 48..54 of these files hold the villager palettes of the
  side-scrolling quarters (44..47 and 60..63 are flash/tint variants of the same block), and the same rows colour the
  top-down pages:
  - **AREA16/CLUT07 row 53** (also in AREA16/CLUT06, AREA17/CLUT03, AREA17/CLUT06 and AREA18/CLUT02, same row number):
    TCRF's "Male Stonetown Resident" (the strongman). With this row the A07600 4 bpp page at (75, 202), 50 x 52, matches TCRF's "Male
    Stonetown Resident" sprite pixel-exact (100 % of the pixels in 5-bit colour, no silhouette mismatch); no other row
    of the file does. **Confirmed.**
  - AREA16/CLUT07 rows 48 and 54: TCRF's "Female Stonetown Resident" and the other Stone People. Plausible, not
    confirmed.
  - The TCRF names, and reading AREA11 as TCRF's "Lumberjack Town" (the hard-hat worker belonging to the Factory, the old
    man to the town), are our identification.
  - **AREA11/CLUT02 rows 4, 36 and 68** (the same rows are in AREA17/CLUT03, AREA17/CLUT06 and AREA18/CLUT02):
    row 4 = TCRF's "Lumberjack Factory Worker" (the hard-hat worker of SYSTEM/A06100), row 36 = TCRF's "Male Lumberjack
    Town Resident" (the old man), row 68 = the same old man "cursed" (darker). **Confirmed by TCRF colours**: all 14
    colours of each TCRF sprite are in that single row (by colour set; TCRF's frames differ slightly, so not
    pixel-exact). Rows 32 apart belong to one set of resident palettes; AREA16/CLUT07 shows the same layout (16/48/80,
    21/53).
  - **AREA17/CLUT03 row 1** (= AREA17/CLUT06 rows 1 and 129, AREA18/CLUT02 row 6): the Iron Men (orange riveted
    suit, white sleeves, green trousers, black spiky hair). Matched to a retail screenshot of the same character type
    (a side-view Iron Town NPC at the Iron Castle door: all 15 colours of the row occur in it; colour set, not
    pixel-exact).
- Which row each top-down resident uses is not in the surviving code: a resident's CLUT comes from the parts of its
  SPR table (the actor inits clear `b0d`, so `w08` is not used), and those tables are lost. `tools/civ_sheets.py`
  states the file and row it draws each resident group with (`sub_pal`, `PAL_NOTE`).

### The surviving texture pages (SYS lists 61 / 76 / 80 / 86)

`inventoryScreenHandler` (MAIN0) closes the sub-screen with `queueSystemLoadList(D_80077A8C[area][section])`, which
reloads the VRAM the menu overwrote. For a working section this list is an exact copy of the section's first texture
page(s), clipped to x = 0x40..0x200. We checked this byte for byte: `SYSTEM/A03900` equals `AREA05/B000` columns
0..447, `SYSTEM/A04700` equals `AREA08/B000`, and `SYSTEM/A09100` equals `AREA19/B200` columns 0..127.

The cut sections still have their entries, so their first pages survived:

| Section | SYS list | Files | VRAM |
|---|---|---|---|
| AREA11 s0 | 61 | `SYSTEM/A06100` | 448 x 256 words at (0x40, 0) |
| AREA16 s0 | 76 | `SYSTEM/A07600`, `A07601` | 320 x 256 at (0x40, 0) and 128 x 256 at (0x180, 0) |
| AREA17 s0 | 80 | `SYSTEM/A08000` | 448 x 256 at (0x40, 0) |
| AREA18 s0 | 86 | `SYSTEM/A08600`, `A08601` | 320 x 256 at (0x40, 0) and 128 x 256 at (0x180, 0) |

`A06100` holds a 4 bpp page of villager sprites in its first 64 words: a cap-wearing worker in four facings, walk
frames, a hammer and a sitting pose, which fits the raft craftsman. The rest is 8 bpp scenery cut-outs: an octagonal
plaza, house walls and roofs, fences, trees. In other words these are the actor and room/roof sprites the SPR tables
pointed into. The **map picture is not among them**, because it lives in RAM and is streamed to (0x300..) when
needed. Their palettes are the CLUT rows listed under Palettes.

## Step 2: what each cut section expected

`tools/topdown.py plan AREA OUT.png` draws the set on a 32 px grid. It shows actors (red outdoors, magenta indoors),
patrol boxes (green), props (yellow) and house rectangles (blue). Every indoor record falls inside a house rectangle
of the same area, which supports the decoding.

Flag bytes below are `GAME`/`SCRIPT_FLAGS` bytes (the event table is `SCRIPT_FLAGS` = 0x8009CDA4).
`setEventStarted` is 0x8005A8A8. A talk handler that sets `o->wac = E` and returns through `state 2` starts event E
once the box closes.

Three progress variables are set by the spawners:

- `D_8009CFA5` (sets 0D/0F) is 0 at first, 1 once `D_8009CE0A != 0`, and 2 once `D_8009CDF1 == 0xFF`.
- `D_8009CF60` (set 0E) is 0..3, from `D_8009CDF1 != 0`, `D_8009CE0A` and `D_8009CDF1 == 0xFF`.
- `D_8009CFA1` picks the animation of prop 15.

### AREA11 s0: the Wood People's village (NPC set 0D)

- **Size and exits** (`topdownSectionStepC`, `D_80079A48[11][0]` = 0x800798B4):
  - exit 0 at x > 0x22B, y 0x169..0x1CC goes to AREA18 s1 spawn 1;
  - exit 1 at y < 0x4F, x 0xC7..0x107 goes to AREA11 s1 spawn 0 (the Factory side-scroller).
  - The actors span x -21..496 and y 204..581, so the town was about 640 x 560 px (80 x 70 cells).
- **Houses**: three, rooms 1..3 at (-10,191)-(154,283), (-60,278)-(51,379) and (361,419)-(500,520).
- **Actors** (5 subtypes, `SPR_DATA[11]`):
  - room 1: two villagers.
  - room 2: one villager.
  - room 3: one villager (subtype 4, `dlg 11`). `dlg 11` has no talk case, so `OpenDialogue` returns 0 and he says
    nothing through this path. He is probably the raft craftsman of k3 dx10-14, which no I0NFO2 call reaches.
  - outdoors: eight villagers, six of them on patrols (two vertical, three horizontal, one diagonal).
- **Props**: 14. Three type-2 props sit in rooms 1..3, at the house rectangles. The other 11 are outdoor props with
  anims 3..15.
- **Dialogue** (`topdownA11ActorOpenDialogue` 0x800F1C88, by `dlg`):
  - 0 → (3,0)
  - 2/5 → (4,2) "this machine is iron..."
  - 3 → (4,0) the fortune-teller's hut
  - 4 → (4,1) "wood really is the best"
  - 6 → (4,3) the maze under the floor
  - 7 → (4,4) "you can only reach that hut from under the floor"
  - 8 → (4,5 / 10 / 16) by progress: the gear, "go get a coconut", "Karakuri Village sank"
  - 9 → (4,6) the workshop entrance
  - 10 → (4,7) the dangerous old signboard, or (4,11) "you're always so full of energy"
  - 1 sets `D_8009CFA9` (1, or 2 if `D_8009CDF3`)
  - `topdownA11ActorDialogueBank2` (0x800F1BAC) shows (2,4), then (2,5) and starts **event 0x4F**.
  - These are MFNT110 #249-#265. PAL replaced them with "···??". The JP lines are in AREA11 A002 block 0 #220-#249;
    the raft craftsman (#237-#241 "you want a raft? bring wood from under the Wise Old Tree... bang clang... ouch")
    is k3 dx10-14.

### AREA16 s0: the Stone People's town (NPC set 0E)

- **Exits** (`topdownSectionStepD`):
  - exit 0 at x < -0x74, y 0x208..0x211 goes to AREA18 s1 spawn 3;
  - exit 1 at x 0x6B..0x72, y 0x13F..0x140 (a door) goes to **AREA11 s2** spawn 0;
  - exit 2 at x > 0x93, y < 0x4A goes to AREA16 s1 spawn 0 (also cut).
- **Houses**: four (rooms 1..4).
- **Actors**: 13. `b0c` gates spawning: b0c 8 only after the first progress step, b0c 11/12 only before it,
  b0c 15 only at the end.
- **Dialogue** (`topdownA16ActorOpenDialogue` 0x800F2E84):
  - Dialogue cases 0..7, 13 and 14 show `(3, TOPDOWN_A16_ACTOR_DIALOGUE_IDS[b0c][D_8009CF60])` (0x800FEF24,
    `u8[16][4]`). Rows used: b0c 1 → 0, 2 → 1, 3 → 2 (the elevator that does not move), 4 → 3/7/10/14,
    5 → 4/8/11/15, 6 → 5/5/12/16, 7 → 6/6/6/17, 8 → 0/9/13/13. Together they cover MFNT160 #230-#247: "heave ho",
    the elevator, "the other towns", the gear, the generator, "make up with everyone", "beat the Evil Pig".
  - Case 8 → (2,2) or (2,0).
  - Cases 12/15 → (2,0x15) = #229 (Mizuno). Case 12 also starts **event 0x67** when `D_8009CE0B == 0`.
  - Cases 9/10 set `D_8009CFA7`.
- **Scenes**: `npcSet0ESceneStep1` starts **event 0x61**. `npcSet0ESceneStep3` shows (2,1)/(2,2) and starts
  **event 0x3E**.
- **Props**: the prop list switches to `NPC_SET_0E_EXTRA_RECORDS_ALT` when `D_8009CFA8 == 4`. That alternative list
  differs only in prop 5's anim (12 instead of 10), most likely the repaired clock. `topdownA16SceneryUpdate` moves
  prop `wb8 == 5` to (-4/-3, 0x193) with anim 0xB/0xC as `D_8009CFA8` goes to 2/3.

### AREA17 s0: the Iron Men's town (NPC set 0F)

- **Exits** (`topdownSectionStepE`):
  - exit 0 at y > 0x23A, x 0xA7..0x117 goes to AREA18 s1 spawn 2;
  - exit 1 at y < 0x50, x 0xB0..0x10F goes to AREA17 s1 spawn 0.
- **Houses**: five (rooms 1..5).
- **Actors**: 16, seven of them patrolling. **Props**: 35, including two rows of eight identical props (anim 23)
  at y = 302 and y = 84. That is most likely the Evil Pig's wall, or the lift rails.
- **Dialogue** (`topdownA17ActorOpenDialogue` 0x800F4110):
  - Case 0 → (2,0xA); case 1 → (2,7).
  - Case 2 → `topdownA17ActorDialogueBank2`, the smith: (2,5) "you need something?", (2,6) "an iron gear?" plus
    **event 0x63**, (2,9), (2,10).
  - Case 3 → `topdownA17ActorDialogueBank3`, the bomb maker: (3,0) "haven't seen your face", (3,2), (3,3) plus
    **event 0x69**, (3,5) "first I need electricity", (3,8) "now the bomb mixture", (3,0xB) "more bombs?".
  - Cases 5..15 → (5,0..19) by progress: the Iron Men's villagers, the lift, Gantetsu, the Iron Castle, "wood,
    stone and iron: balance is what matters".
- **Scenes**:
  - `npcSet0FSceneStep1` starts **event 0x4C**.
  - `npcSet0FSceneStep2` shows (2,6) and starts **event 0x63**.
  - `npcSet0FSceneStep4` shows (2,7) "good material... wait" and (2,8) "take it", then gives **item 0x3E** (the iron
    gear) with `func_80026C50(0x3E, 1, 1)`.
  - `npcSet0FCheckTriggerSpots` and `topdownA17SceneryUpdate` drive two moving platforms (props `wb8` 0xD/0xE
    between `TOPDOWN_A17_SCENERY_LIMITS`, carrying the player when bit 0x40 is set). These are the lifts.
- **Text**: MFNT170 #213-#218, #257-#268 and #270-#289. They are "···??" in PAL; the JP text has them.

### AREA18 s0: the reunited town (NPC set 0x10), never implemented

- The NPC set has 10 actors with no patrol, no props and dialogue cases 0/1 only.
- `topdownA18ActorHandler` (0x800F548C) only cycles the animation (`wb4 = (wb4+1) & 7`) while pad bit 0x1000 is
  held. It is a sprite viewer, with no talk code at all.
- The section controller entry `D_80079AD0[25]` (`dispatchSectionClose`) calls 0x80116234 for section 0. That is X0nn
  space, not I0NFO2 (in X018 it is not even a function start). A top-down AREA18 s0 would therefore jump into the
  wrong overlay.
- Its exit table (0x800799D0) points to AREA01 s5 and AREA03 s1, which are placeholders.
- Its JP text (MFNT180 k2 dx0-12 = #209-#221, "we're one country again thanks to Tomba") has no caller anywhere.
- This town was planned but never got past test data.

### How section 0 would have been entered (and why it cannot be)

The exit records (`D_80079A48[area][section]`, 8 bytes `{s16 x, s16 y, u8 kind, u8 area, u8 section, u8 spawn}`,
printed by `tools/topdown.py warps`) still describe the planned routes. Section 0 of each town was meant to be the
gate between the hub and the town's side-scrolling sections:

- AREA11 s0 exits to 18:1 and 11:1. AREA16 s0 exits to 18:1, 11:2 and 16:1. AREA17 s0 exits to 18:1 and 17:1.
- **No exit record of any area points to 11:0, 16:0..3, 17:0 or 18:0.** The scan covered the MAIN0 tables of
  areas 2/5/8/11/16..19 and the overlay tables of X000/X001/X003/X004/X006/X009/X010/X013/X014.
- The retail hub instead jumps past them: AREA18 s1 exits 0/1/2 go to **AREA11 s1** spawn 0, **AREA17 s1** spawn 0
  and **AREA16 s4** spawn 0.
- AREA16 s4..s6 are copies of the cut s1..s3, with the same exit tables (0x80079908.. shared). AREA17 s6..s10 likewise
  share the tables of s1..s5.
- AREA10 s0/s4 lead to 18:1 spawn 4, AREA00 s0 exit 2 leads to 18:2, and AREA14/15 lead to 16:5.

So the cut towns are unreachable for three reasons: no door leads in, the load lists are empty, and (for AREA18) there
is no controller. Restoring one means:

1. pointing a retail exit at it, for example AREA18 s1 exit 0 → 11:0 spawn 0, whose exit 1 already continues to
   11:1;
2. filling the load list;
3. leaving everything else as it is.

The town's spawn points are the `GAME_SELECTED_SPAWN_POINT` values its own exit records use when coming back
(AREA11 s1 exit 0 → 18:1 spawn 1 and so on).

## The cut towns, one by one (sheets)

`tools/civ_sheets.py` draws three explanatory sheets per town, in Spanish or English, from your own extracted disc
(`game/disc/fs` or `TOMBI_FS`). The sheets contain disc art (the AREA08 picture and the surviving resident sprites),
so they are never committed: regenerate them locally.

```
python3 tools/civ_sheets.py                                   # all towns, both languages, all sheets -> build/civ/
python3 tools/civ_sheets.py --area 16 --lang en --sheet esquema --out OUT   # one sheet
```

| Sheet | File (ES / EN) | What it shows |
|---|---|---|
| esquema | `civ_esquema_aNN.png` / `civ_esquema_aNN_en.png` | evidence only. Panel A is the real AREA08 s0 picture with the marks its own code places. Panel B is the cut town at the same scale on grey, with the real resident sprites in real in-game palettes, houses, patrol boxes, props and the exits at their code positions. Below: what each mark means, where each exit leads, what each resident says, and where every element comes from |
| explicado | `civ_explicado_aNN.png` / `civ_explicado_aNN_en.png` | a flat plan with drawn house shapes, sprites, exit arrows and a side legend, next to the AREA08 reference |
| perspectiva | `civ_perspectiva_aNN.png` / `civ_perspective_aNN_en.png` | the same data in perspective; the only invented value is the house height |

Marks: houses are the `TOPDOWN_TRIGGER_RECT_LISTS` rectangles, where the roof disappears and the inside shows when
Tomba walks in; red residents stand outdoors, purple ones inside a house; green is the box a resident walks in;
yellow squares are props. The resident lines are short glosses of the Japanese text (the PAL slots hold "···??").
Which sprite of the surviving page stands for which resident subtype is a guess. The picture size of each town is an
estimate from the placements (AREA11 736 x 608, AREA16 544 x 608, AREA17 832 x 704 px with margins).

### AREA11 s0, the Wood People's village

- **Houses** (`TOPDOWN_TRIGGER_RECT_LISTS` 0x800FEC9C, world x0,y0-x1,y1): H1 (-10,191)-(154,283); H2 (-60,278)-(51,379); H3 (361,419)-(500,520).
- **Residents** (NPC_SET_0D, list at I0NFO2 0x80100080; lines via topdownA11ActorOpenDialogue 0x800F1C88 → AREA11/MFNT110S.WFM #249-#265): 12, numbered as on the sheets.

| # | Where (x, y) | Room | Who / what they say (gloss of the JP line) |
|---|---|---|---|
| 1 | (88, 240) | H1 | Lumberjack Factory Worker — House 1: "...Now I can make something really interesting..." (PAL text #227) |
| 2 | (12, 256) | H1 | Lumberjack Factory Worker — House 1: no line, sets a flag |
| 3 | (16, 352) | H2 | Lumberjack Factory Worker — House 2: "It doesn't work, but this machine is iron... I could use it" |
| 4 | (400, 520) | outdoors | Lumberjack Factory Worker — Diagonal patrol: "A fortune-teller lives in this hut, but she has a condition" |
| 5 | (300, 470) | outdoors | Lumberjack Factory Worker — Horizontal patrol: "Wood is the best; stone and iron are cold" |
| 6 | (170, 528) | outdoors | Lumberjack Factory Worker — Horizontal patrol: "It doesn't work, but this machine is iron..." |
| 7 | (172, 424) | outdoors | Male Lumberjack Town Resident — Neighbour: "There's a maze under the floor; don't wander in carelessly" |
| 8 | (50, 320) | outdoors | Lumberjack Factory Worker — Vertical patrol: "You can only reach that hut from under the floor" |
| 9 | (62, 294) | outdoors | Lumberjack Factory Worker — Horizontal patrol: "A gear? Made of wood it would break" → "I'm thirsty, I'll get a coconut" → "Karakuri Village sank" |
| 10 | (280, 236) | outdoors | Lumberjack Factory Worker — Vertical patrol: "The workshop entrance is up there; go take a look" |
| 11 | (496, 344) | outdoors | Male Lumberjack Town Resident — Neighbour: "This sign is old and dangerous" → "You're always so full of energy" |
| 12 | (432, 448) | H3 | Lumberjack Factory Worker — House 3, sitting: probably the raft maker ("Want a raft? Bring me wood from the Wise Old Tree"); the code gives him no dialogue |

- **Exits** (`topdownSectionStepC`, I0NFO2 0x800E8D98; destinations MAIN0 0x80079A48): Civilization Town (AREA18); Factory (AREA11 s1).
- **Props**: 14 (I0NFO2 0x800FFE24); the code does not say what each one is.
- **Sprites**: `SYSTEM/A06100` (SYS reload list 61), drawn with AREA11/CLUT02 row 4 (Lumberjack Factory Worker, subtypes 0, 1, 4) and row 36 (Male Lumberjack Town Resident, subtypes 2, 3): rows confirmed by TCRF colours, the subtype-to-drawing match is ours.
- **Missing**: background picture, cell map (collision), sprite tables.
- **Regenerate**: `python3 tools/civ_sheets.py --area 11 --lang all --out build/civ`

### AREA16 s0, the Stone People's village

- **Houses** (`TOPDOWN_TRIGGER_RECT_LISTS` 0x800FEC9C, world x0,y0-x1,y1): H1 (78,351)-(202,486); H2 (160,264)-(235,343); H3 (219,176)-(299,258); H4 (-25,146)-(51,244).
- **Residents** (NPC_SET_0E, list at I0NFO2 0x80100364; lines via topdownA16ActorOpenDialogue 0x800F2E84 → AREA16/MFNT160S.WFM #230-#247): 13, numbered as on the sheets.

| # | Where (x, y) | Room | Who / what they say (gloss of the JP line) |
|---|---|---|---|
| 1 | (86, 460) | outdoors | Neighbour lifting something: "Heave... HO!" |
| 2 | (144, 332) | outdoors | Neighbour lifting something: "Hup... HAAA!" |
| 3 | (244, 427) | outdoors | Female Stonetown Resident — Neighbour by the lift: "This thing on the wall is supposed to be a lift... but it doesn't move" |
| 4 | (176, 384) | H1 | Male Stonetown Resident — House 1: "Can't get into the other towns? Don't know, don't care" → later talks about the gear and the generator |
| 5 | (-72, 362) | outdoors | Female Stonetown Resident — Vertical patrol: "The folks from the other towns..." → "A gear that won't break? If it were iron..." |
| 6 | (0, 245) | outdoors | Diagonal patrol: "First time in this town?" → "Long ago I tried to go into the river..." |
| 7 | (86, 315) | outdoors | Neighbour: "Should I go in...?" → "You'll beat the Evil Pig who cursed this land, right?" |
| 8 | (239, 106) | outdoors | Diagonal patrol (after the first milestone): "I secretly opened the door between the Iron and Wood towns" |
| 9 | (26, 174) | H4 | House 4, craftsman: "Hmm... I want to make something" / "Come to think of it..." |
| 10 | (140, 192) | outdoors | Neighbour (before the first milestone): no line, sets a flag |
| 11 | (169, 210) | outdoors | Neighbour (before the first milestone): no line, sets a flag |
| 12 | (205, 290) | H2 | House 2: no dialogue in the code |
| 13 | (37, 528) | outdoors | At the end: the boy who saw Mizuno the witch (PAL text #229), starts event 0x67 |

- **Exits** (`topdownSectionStepD`, I0NFO2 0x800E8ED4; destinations MAIN0 0x80079A48): Civilization Town (AREA18); Factory (AREA11 s2), door; AREA16 s1 (also cut).
- **Props**: 7 (I0NFO2 0x801001D4 / 0x801001D8); the code does not say what each one is.
- **Sprites**: `SYSTEM/A07600` (SYS reload list 76), drawn with real palettes: AREA16/CLUT07 row 53 for Male Stonetown Resident (subtype 2, confirmed against TCRF), rows 54 (subtype 0) and 48 (subtypes 1, 3) for the rest (guesses).
- **Missing**: background picture, cell map (collision), sprite tables.
- **Regenerate**: `python3 tools/civ_sheets.py --area 16 --lang all --out build/civ`

### AREA17 s0, the Iron Men's village

- **Houses** (`TOPDOWN_TRIGGER_RECT_LISTS` 0x800FEC9C, world x0,y0-x1,y1): H1 (177,203)-(270,336); H2 (454,280)-(580,420); H3 (373,100)-(488,195); H4 (-62,75)-(81,197); H5 (-151,257)-(0,422).
- **Residents** (NPC_SET_0F, list at I0NFO2 0x80100784; lines via topdownA17ActorOpenDialogue 0x800F4110 (+ DialogueBank2/3) → AREA17/MFNT170S.WFM #213-#218, #257-#268, #270-#289): 16, numbered as on the sheets.

| # | Where (x, y) | Room | Who / what they say (gloss of the JP line) |
|---|---|---|---|
| 1 | (7, 152) | H4 | House 4: "I hear you fixed the stone clock tower" |
| 2 | (27, 140) | H4 | House 4: "Good material! Wait a moment..." |
| 3 | (-75, 371) | H5 | House 5, the smith: "Need something? An iron gear?" → gives you the gear (item 0x3E) |
| 4 | (223, 123) | outdoors | The bomb maker: "Haven't seen your face before..." → "To make bombs I need electricity" |
| 5 | (223, 0) | H1 | House 1: no dialogue in the code |
| 6 | (175, 424) | outdoors | Neighbour: "Want to go up? Go into this hut" |
| 7 | (256, 450) | outdoors | Horizontal patrol: "Welcome, this is the Iron Men's town" |
| 8 | (500, 363) | H2 | House 2: "Iron is sturdy and lasts" → "You even fixed the generator?" |
| 9 | (370, 310) | outdoors | Diagonal patrol: "Why are we always at each other's throats?" |
| 10 | (300, 313) | outdoors | Horizontal patrol: "Everyone and their wood and stone..." → "Wood, stone and iron: balance is what matters" |
| 11 | (290, 191) | outdoors | Neighbour: "Use me as a landmark and go straight down to leave" |
| 12 | (440, 152) | H3 | House 3: "The lift used to run on machinery" |
| 13 | (300, 108) | outdoors | Horizontal patrol: "Ride the lift and take the middle path to the Iron Castle" |
| 14 | (90, 120) | outdoors | Diagonal patrol: "That old Gantetsu won't let me go to the Castle" → "Sorry I made fun of you" |
| 15 | (128, 232) | outdoors | Diagonal patrol: "This land is under the Evil Pigs' spell too" |
| 16 | (70, 290) | outdoors | Diagonal patrol: "If you want something iron, try the lift hut" → "Are you a good swimmer?" |

- **Exits** (`topdownSectionStepE`, I0NFO2 0x800E9040; destinations MAIN0 0x80079A48): Civilization Town (AREA18); inside Iron Town (AREA17 s1).
- **Props**: 35 (I0NFO2 0x80100368); the code does not say what each one is.
- **Sprites**: `SYSTEM/A08000` (SYS reload list 80), drawn with the real palette AREA17/CLUT03 row 1 (matched to a retail screenshot; that every resident uses it is a guess).
- **Missing**: background picture, cell map (collision), sprite tables.
- **Regenerate**: `python3 tools/civ_sheets.py --area 17 --lang all --out build/civ`


## Validation (tools/topdown.py)

The tool rebuilds working sections from disc data alone:

- `tools/topdown.py render 8:0 out.png --grid` draws the Baccus Lake overview. The 544 x 448 TIM uses its own CLUT,
  the cell shapes are tinted on top (bit 7 outlined), and APD records and set 0C actors are marked. The walls follow
  the deck edges exactly.
- `render 19:2` gives the same picture for AREA19 s2. Its trigger cells cover the three house doors, and the blue
  house rectangles from `TOPDOWN_TRIGGER_RECT_LISTS` enclose them.
- `render 5:0` (Baccus Village) and `grid AREA:SEC` (the cell map alone, plus a value histogram) cover the rest.
- `info`, `lists`, `npcs`, `defs` and `warps` print the tables quoted on this page.

## Step 3 (authoring the towns): feasibility

| Piece | Status | Work |
|---|---|---|
| Code (all handlers, NPC/prop placement, dialogue, events, exits) | complete in I0NFO2 and matched | none for AREA11/16/17. AREA18 needs a controller (any of StepA..E logic with its own rectangles) |
| Load list | empty | write a list like AREA08 s0's: GRPX pages + CLUT, 8E, 8F, 62 + 3A (copy them from any top-down section), 3B..3E, APD, I0NFO2 |
| Cell map 8E | missing | author it. It is a byte grid with 10 shapes plus a trigger bit and is easy to paint over a picture. The constraints are fixed by the placements: houses at the trigger rectangles, exits at the StepC/D/E rectangles, and patrol boxes free of walls |
| Map picture 8F | missing | the hard part. One pre-rendered 8 bpp picture per town, about 640 x 576 (AREA11), 448 x 544 (AREA16) and 704 x 480 (AREA17). It can be painted, or assembled from AREA05/08/19 and the scenery cut-outs in the SYS reload pages |
| Actor sprites 3B | missing table, pages partly survive | the 4 bpp villager page of A06100 (and the matching parts of A07600/A08000) has the frames. A new SPR table must lay its frames out in the order the I0NFO2 anim scripts expect, or new anim scripts must be supplied |
| Scenery 3D / roofs | missing table, cut-outs survive in the 8 bpp part of the reload pages | the same work as 3B. The roofs must cover the house rectangles |
| Palettes | resident rows survive (AREA16/CLUT07 row 53 confirmed; 48/54, AREA11/CLUT02 4/36/68 confirmed by TCRF colours; AREA16 48/54, AREA17/CLUT03 1 matched to a retail screenshot) | load the area CLUT file whose rows 48..54 hold them; the TIM carries the picture's own palette |
| Text | PAL has "···??", JP has everything | translate the JP lines into the PAL slots (they already sit at the indices the code uses) |

In the port this is a data task, not a code task. The engine already runs AREA05/08/19 s0/s2, and loading I0NFO2 at
the overlay base works (docs/PORT.md). The cheapest test is to load AREA08 s0's picture, cell map and sprite tables
under AREA11 s0 with set 0D. The NPCs, houses and dialogue would then appear on the lake's deck (`plan 11 --from 8:0`
shows the overlap) before any art is made.
