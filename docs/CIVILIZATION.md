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
  - AREA16/CLUT07 rows 48 and 54 as TCRF's "Female Stonetown Resident" and the other Stone People: **refuted**. Row 54
    is the red-shirt resident of the FMV (A08600 figure, see below), and the Female Stonetown Resident uses row 21.
  - The TCRF names, and reading AREA11 as TCRF's "Lumberjack Town" (the hard-hat worker belonging to the Factory, the old
    man to the town), are our identification.
  - **AREA11/CLUT02 rows 4, 36 and 68** (the same rows are in AREA17/CLUT03, AREA17/CLUT06 and AREA18/CLUT02):
    row 4 = TCRF's "Lumberjack Factory Worker" (the hard-hat worker of SYSTEM/A06100), row 36 = TCRF's "Male Lumberjack
    Town Resident" (the old man), row 68 = the same old man "cursed" (darker). **Confirmed by TCRF colours**: all 14
    colours of each TCRF sprite are in that single row (by colour set; TCRF's frames differ slightly, so not
    pixel-exact). Rows 32 apart belong to one set of resident palettes; AREA16/CLUT07 shows the same layout (16/48/80,
    21/53).
  - **AREA17/CLUT03 row 1** (= AREA17/CLUT06 rows 1 and 129, AREA18/CLUT02 row 6): the Iron Men (orange riveted
    suit, white sleeves, green trousers, black spiky hair). **Confirmed: the same row the retail side-view Iron Town NPC
    uses**, proven from the side-view room's data: AREA17 room list 3 (section 2) loads `AREA17/B202` at VRAM (0, 256)
    and `AREA17/CLUT03` at (128, 480); its SPR table `AREA17/D205` (kind 0x33, slot 3) draws the spiky-haired Iron Men
    of B202 (frames 94-112) with the part CLUT word 0x7809 = (144, 480) = row 1 (frames 113-130, the turbaned men,
    use 0x7849 = row 17). The handler that draws them is not pinned (it must leave `b0d` 0 so the part word is used);
    `updateScriptActor` subtype 10 of the area overlays also sets `w08 = 0x7809`. B202 is the same character as the
    A08000 figures but drawn again larger; no frame is pixel-identical.
  - **Community FMV crops with palette strips (images 138-140 of the project).** Each strip is a real disc row,
    15/15 colours exact (shown as 5-bit << 3), so these are not FMV-derived palettes:
    - red-shirt, curly-haired Iron Town resident (BUNMEI.STR, frame not identified):
      `0000 0421 7FFF 56B5 318C 4AFF 3639 2174 21BC 10D6 0010 01B6 0174 0132 6358 4AF0` = **row 54** of AREA16/CLUT06/07 and AREA18/CLUT02,
      **row 49** of AREA17/CLUT03/06 (all discs; row = 16-entry block of the file);
    - purple-dressed resident (BUNMEI.STR, frame not identified):
      `0000 2A17 7FFF 4210 318C 2108 52DD 3A59 6593 5571 454F 011B 00B3 006B 0218 0173` = **row 38** of AREA16/CLUT06/07 and AREA18/CLUT02, **row 33** of AREA17/CLUT03/06;
    - bald, brown man inside a building: `0000 0421 4210 294A 6B5A 1E39 012F 1193 56B5 4A52 2D6B 5EF7 35AD 6B5A 0000 7FFF`
      = row 15 of the CLUT files of AREA00, 01, 02 and other areas (a row every area shares, not a Civilization row).
    - blue-hat, red-cloaked figure seen from behind on the Iron Town lawn (image 143; BUNMEI.STR, frame not identified).
    These colours fit the **AREA18 figure page `SYSTEM/A08600`** (4 bpp in its first 64 words; tested with every resident row of
    AREA18/CLUT02, judged by eye; FMV renders are 3D, so this is not a silhouette proof):
    - 139 = the big curly-haired figure of A08600 (rows y 144-191) with row 54: red hair, red/salmon clothes;
    - 140 = the same figure with row 38: orange hair, purple dress;
    - 143 = the wide-hat cloaked figure (y 96-141, which has back views); the closest row is AREA18/CLUT02 row 22 (blue-grey
      hat, orange trim), but no row gives a red cloak, so this one is unresolved.
    These residents are drawn only on A08600 (AREA18's page), not on the surviving Iron Town / Stonetown pages. Row 54 of
    AREA16/CLUT07, the old guess for the Female Stonetown Resident, is therefore the red-shirt resident's palette; the guess
    was wrong (that subtype uses row 21).
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

### Scenery colours, the FMV and community reference images

- **The 8 bpp scenery cut-outs have no surviving palette.** The pages hold room interiors, walls, gates, fences and the
  windmill. We fitted community colour renders of SYSTEM/A06100, A08000 and A08600 (index → colour, exact page match)
  against every 256-colour window, at 16-entry steps, of every file on the PAL and USA discs. Nothing came close: the
  best mean error was over 60 per colour. The real scenery CLUTs went with the lost section data.
- **The only colour reference is the FMV.** `MOVIE/BUNMEI.STR` (文明, "civilization"), identical on PAL and USA, is the
  purified-Civilization flyover. It shows Lumberjack Town with the windmill, the hard-hat worker and the old man, Stonetown
  with its green-tiled roofs, and Iron Town with the round iron gate and the "Iron the No.1" sign. `BUNMEI2.STR` follows
  with the machine room and the gears. `python3 tools/str.py frames game/disc/Tombi.bin BUNMEI OUT 12 25` extracts
  stills (never committed).
- **Only BUNMEI2 is played.** The movie id is `MOVIE_ID` (0x1F8001CD); `moviePlayerTask` plays file
  `MOVIE_FILE_IDS[D_80077F3C[id]]`, so id 11 is BUNMEI.STR (file 323) and id 12 is BUNMEI2.STR (324). Nothing stores 11:
  the PAL writers set 0-9, 0x14, 0x15, and x014's section-clear table `D_80125684` holds 10, 13-19. BUNMEI2 is id 12,
  set only by `waterfallElderHintScene` (x017, `sb` at 0x80119204), the elder's 10,000-year scene that ends with event
  0x49. Leftover: that scene first calls `cdSeekStream(0xB)` (0x801191A0), a pre-seek to BUNMEI, then plays 12. JP
  (SLPS-01144 x17) also uses 12. BUNMEI.STR is on the disc but unused.
- **Community reference images.** A TCRF contributor coloured the reload pages and assembled scenery views (seven
  images, plus two "screenshots" of Iron Town and Lumberjack Town). They are reconstructions made from BUNMEI.STR frames
  with a custom-derived CLUT, not pre-release screenshots and not in-game palettes. We use them as visual reference
  only, and they are not in the repository.
- **The community VRAM files are the PAL pages.** The seven `.000` files (USA names) are, byte for byte:

| USA name | PAL name | Content |
|---|---|---|
| SYSTEM/A00147 | SYSTEM/A06100 (SYS list 61) | Lumberjack Town page |
| SYSTEM/A00153 | SYSTEM/A07600 (SYS list 76) | Stonetown page |
| SYSTEM/A00154 | SYSTEM/A07601 (SYS list 76) | Stonetown page, second part |
| SYSTEM/A00157 | SYSTEM/A07702 (SYS list 77) | 128 x 256 page at (0x180, 0) |
| SYSTEM/A00163 | SYSTEM/A08000 (SYS list 80) | Iron Town page |
| SYSTEM/A00175 | SYSTEM/A08600 (SYS list 86) | AREA18 page (interiors of all three towns) |
| SYSTEM/A00176 | SYSTEM/A08601 (SYS list 86) | AREA18 page, second part |

  They are the same SYS reload pages under the USA disc's file numbering, not USA-only data.

### Which drawing each subtype is (evidence)

The animation scripts survive in I0NFO2 (`TOPDOWN_Ann_ACTOR_DEFS[sub].anims`, 8-byte steps `{u16 frame, -, -, u16 control}`).
The idle/walk/talk code reads scripts `animFrame + 0/4/8` of table 0. Per subtype:

| Town | Sub | w1e (page) | Frames used by its scripts | Drawing | Status |
|---|---|---|---|---|---|
| AREA11 | 0 | 0x01 | 0..29 (idle, walk, talk), plus 30..50 in extra scripts 12..23 | hard-hat worker, Lumberjack Factory Worker (the old man is on the same page, frames 30..50) | page proven; figure consistent |
| AREA11 | 1, 2, 3, 4 | 0x0A, 0x0B, 0x10, 0x11 | 36..89, 75..128, 108..196, 204..240 | on pages no surviving file fills | lost |
| AREA16 | 0 | 0x01 | 30..80 | first figure group of A07600 (Female Stonetown Resident) | page proven; figure consistent |
| AREA16 | 1 | 0x01 | 60..110 | second group (the boy) | page proven; figure consistent |
| AREA16 | 2 | 0x01 | 90..129 | third group (the strongman, Male Stonetown Resident) | page proven; figure consistent; palette pixel-exact |
| AREA16 | 3 | 0x11 | 0..29 | page at VRAM y 256 | lost |
| AREA17 | 0 | 0x01 | 0..55 | the spiky-haired resident of A08000 | page proven; figure consistent |
| AREA17 | 1, 2, 3, 4 | 0x0A, 0x0B, 0x10, 0x08 | 41..96, 82..135, 115..147, none | lost pages | lost |

- **Page (proven).** A part is drawn from page `part.tpage + w1e`. In the retail top-down tables (AREA05/08/19) the actor
  parts use relative page 0. The surviving SYS reload pages start at VRAM x 0x40, which is page 1, and the 4 bpp
  sprite area is their first 64 words. So only subtypes with w1e 1 can draw from them; every other subtype's page is
  filled by no surviving file.
- **Figure (consistent).** In AREA05/08 the frames of SPR_DATA[11] are packed on the page in frame-id order, row by row,
  with mirrored frames reusing the texels at u - 1. The AREA16 subtypes 0/1/2 start at frames 30/60/90, in the same
  order as the three figure groups on A07600 from the top: hooded women, the boy, the strongman. The strongman's colours
  match TCRF pixel-exact. AREA11 subtype 0 uses frames 0..29 first, and the hard-hat worker is the first group on
  A06100.
- **Not shown.** The facing frame drawn on the sheets is our choice (the record's field +4 picks the real one).
  Residents whose page is lost are drawn as grey capsules with a question mark.

### Music of the cut sections

Short answer: no dedicated town theme survives, and none is missing. Each cut section 0 still has a music entry, and it
points at the same track as the rest of its area. `tools/music_refs.py` prints the whole table from your MAIN0.

- **How music is picked (proven, MAIN0 code).** Music is not in the `SYS/LDnARxx` lists. Those lists carry only the
  small `0xB2`/`0xB3` sub-pieces inside the area bundle. `loadAreaSectionResources` (0x8001D1BC) reads a sound-set
  number from `PTR_DAT_80077c4c[area + AREA_VARIANT][section]`. `loadSoundSet` (0x8004FB68) then runs
  `SOUND_SET_LOAD_LISTS[set]` (0x8007D1B4). This is 0x34 load lists stored inside MAIN0: one WVD (kind 0x93) plus one
  SEQ (kind 0xB0). `openAreaBgm` (0x8001EC80) plays whatever SEQ was loaded. It changes only the volume (per area, plus
  section tweaks for AREA06/09) and the reverb. No code chooses music per section.
- **I0NFO2 does not touch music (proven).** None of the `topdown*` handlers sets a BGM. The only sound-control call is
  `stopBgm(1)` in `topdownPlayerDropExitStep` (0x800EAAA0), which fades out when an exit leads to another area. Working
  top-down sections use the plain area table: AREA05 s0 set 0x0F `S_AR050` (same as s1..s3), AREA08 s0 set 0x13
  `S_AR080` (same as s1..s3), AREA19 s0 set 0x2B `S_AR192`.
- **What would play if a cut section were entered (proven from the table, not run):**

| Section | Row (byte) | Set | Files | Shared with |
|---|---|---|---|---|
| AREA11 s0 | 0x80077C10 | 0x1B | `SND0110.WVD` + `S_AR110.SEQ` | AREA11 s1..s3 (Factory) |
| AREA16 s0 | 0x80077C34 | 0x27 | `SND0160.WVD` + `S_AR160.SEQ` | AREA16 s1..s7 (Stonetown) |
| AREA17 s0 | 0x80077C3C | 0x28 | `SND0170.WVD` + `S_AR170.SEQ` | AREA17 s1..s7 (Iron Town) |
| AREA18 s0 | 0x80077C44 | 0x29 | `SND0181.WVD` + `S_AR181.SEQ` | AREA18 s1 (hub); s2/s3 use 0x2A `S_AR1802` |

- **No unused track (proven).** All 44 sound sets are reached from the area table, from the extra effect-bank tables at
  0x80077C9C/CA4/CB0, or from boot/title code. Every `SOUND/*.SEQ`/`*.WVD` belongs to a reached set. There is no XA
  audio on the disc. The only streamed audio is inside `MOVIE/*.STR`.
- **Names (inference).** Files are named after an area and the first section that uses them (`S_AR003`, `S_AR023`,
  `S_AR147`...). So `S_AR110`/`S_AR160`/`S_AR170` carry the section-0 name of the cut towns. The same naming applies to
  every area's default track (`S_AR050`, `S_AR080`), so this does not show a town-only theme. AREA18 is the exception:
  its first track is `S_AR181`, not `S_AR180`. That may mean an `S_AR180` for the reunited town was planned. No table,
  file or string names one.
- **JP disc (SLPS-01144, `game_jp/disc/tomba_jp.bin`).** The banks sit in the area folders: `AREA11/SND0110.WVD`,
  `AREA16/SND0160`, `AREA17/SND0170`, `AREA18/SND0181` and `SND0182`. Their sizes match PAL byte for byte, and there is
  no `SND0180`. We did not trace the JP set table. The JP and 980203 beta executables have no byte copy of the PAL rows.

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
Which subtypes use the surviving page, and which figure each is, is set out under "Which drawing each subtype is"; residents on lost pages are grey capsules. The picture size of each town is an
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
- **Sprites**: `SYSTEM/A06100` (SYS reload list 61), drawn for subtype 0 only (Lumberjack Factory Worker, AREA11/CLUT02 row 4, confirmed by TCRF colours); subtypes 1-4 are on lost pages.
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
- **Sprites**: `SYSTEM/A07600` (SYS reload list 76), drawn with real palettes: AREA16/CLUT07 row 53 for Male Stonetown Resident (subtype 2, confirmed against TCRF), row 37 for the boy (subtype 1, confirmed by TCRF colours: all 14 colours of TCRF's sheet are in that row, also in AREA16/CLUT06, AREA17/CLUT03/06 and AREA18/CLUT02), and for the Female Stonetown Resident (subtype 0) **row 21** (`0000 0421 7FFF 4AFD 3238 29B2 6EB6 5E33 4DD0 3D6E 22FC 1E56 1DD1 4210 2108 19FB`, also in AREA16/CLUT06, AREA17/CLUT03/06 and AREA18/CLUT02): real disc row, found by matching TCRF sheet's appearance (blonde woman, lilac dress); TCRF colours are shifted (up to 9 levels). Superseded: a custom CLUT derived pixel-exact from the TCRF sheet with `tools/clut_from_sprite.py` (24 frames, 0 silhouette mismatches, all 15 indices with one colour); not found on any disc (best disc row: mean error 30): `0000 0000 7FFF 3A78 25D2 194F 5233 41B0 314D 1D0C 1677 0DD1 096E 1D4C 10EA 21D2`.
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
- **Sprites**: `SYSTEM/A08000` (SYS reload list 80), subtype 0 drawn with the real palette AREA17/CLUT03 row 1 (confirmed: the row the side-view Iron Men of AREA17/D205 use); subtypes 1-4 are on lost pages.
- **Missing**: background picture, cell map (collision), sprite tables.
- **Regenerate**: `python3 tools/civ_sheets.py --area 17 --lang all --out build/civ`


## Port test (`--civ-test`)

A dev flag of the native port (`port2/`, port only; the console build is unchanged) that runs AREA11 s0 with its own
code (NPC set 0D, `topdownSectionStepC`, the AREA11 actor and dialogue handlers) over a stand-in map: AREA08 s0's
picture, cell map, sprite tables and I0NFO2 (rules in `port/fixes.txt`, gated on `port_civ_test`). The residents walk
their patrols and talk (Square, facing them). They wear the AREA08 villager, because the town's own sprite tables are
lost. Their PAL text slots hold "···??", so the test data puts short Spanish glosses of the Japanese lines there.

```
python3 tools/civ_test_data.py OUT/civdata                      # symlinked copy of data/ + rebuilt MFNT110S / LD5AR11
PORT_STALE_FULL=warn cmake -S port2 -B OUT/bp && cmake --build OUT/bp -j 8
OUT/bp/tombi2 --headless --skip-movies --data OUT/civdata --lang 5 --civ-test --civ-spawn 100,294 --frames 1760 \
  --poke 900:1F8001B4:1 --poke 1250:8009C960:B:2 --poke 1250:8009C962:0:2 \
  --pad-script 1000:start,1300:circle:3,1600:left:1,1610:square:2x120/8 --dump-frames OUT/shots --dump-every 20
```

The pokes open the debug area select on AREA11 s0. `--civ-spawn 100,294` puts Tomba on the patrol line of resident 9,
and Square talks to him (frame 1740 shows his line). `tools/civ_test_data.py` keeps the translations as text and
writes them in priority order while the WFM stays under 26540 bytes (`--cap`). Bigger builds showed garbage glyphs, so
six of the 18 lines fit (`#249 #250 #251 #252 #254 #257`) in the disc file. The script also writes `civ_MFNT110S.WFM` with all 18 lines; `--civ-wfm OUT/civdata/civ_MFNT110S.WFM` makes the port load that bank straight into memory before a resident talks, so every line plays (checked: with the retail data and this option, resident 9 says his translated line).

## Playable towns (`--civ-town`, port only)

The three rebuilt towns run in their own area slots (AREA11, AREA16 and AREA17 section 0, the real cut sections),
with the town's own code (NPC sets 0D/0E/0F, StepC/D/E exits, the actor and dialogue handlers). Everything is
port-only and opt-in; the console build is unchanged (rules in `port/fixes.txt`, gated on `port_civ_test`).

```
python3 tools/civ_rebuild.py --out OUT/maps [--refdir IMAGES] [--community-cluts CLUTGEN]   # pictures, cells, rooms
python3 tools/civ_test_data.py OUT/civdata              # civ_MFNT110S/160S/170S.WFM (Spanish glosses of the JP lines)
OUT/bp/tombi2 --lang 5 --civ-town 17 --civ-maps OUT/maps --civ-wfm OUT/civdata [--civ-collision]
OUT/bp/tombi2 --lang 5 --civ-route --civ-maps OUT/maps --civ-wfm OUT/civdata      # through the hub, in normal play
```

- The launcher (tools/port_launcher.py, "Patches and mods") offers it as section "0 — rebuilt town" of the ★ areas in
  the area select, plus checkboxes for `--civ-collision` and `--civ-route`. On first use it builds the maps and texts
  in-process from its own data/ (civ_cache/ next to tombi-port.json; rebuilt when the generator sources, the bundled
  layout or MAIN0 change). No disc image is needed: tools/area000.py reads the ISO layout from data/manifest.txt and the
  BUNMEI.STR mosaic comes from data/MOVIE when TOMBI_FS points at a launcher data folder.
- `--civ-town 11|16|17` is `--civ-test` plus the debug area select opened on that town's section 0 (Start, then
  Circle, as the pokes of the old command did). Tomba starts just inside the exit from Civilization Town
  (AREA11 world (530, 410), AREA16 (-95, 524), AREA17 (223, 540); `--civ-spawn X,Y` overrides it). The camera limits
  follow the rebuilt picture (x max = W - 288, y max = H - 112).
- `--civ-maps DIR` loads `DIR/AREAnn/map8F.tim` and `map8E.bin`, and the room view `map8F_in.tim`.
- `--civ-collision` swaps in the debug pictures `map8F_cells.tim` / `map8F_in_cells.tim`: the cell map tinted over
  the picture (red solid 9, orange diagonal 1-4, magenta one-sided wall 5-8, cyan trigger, every cell outlined), so
  the fit can be checked while walking.
- `--civ-wfm DIR` gives each town its translated bank (`civ_MFNT110S.WFM`, `civ_MFNT160S.WFM`, `civ_MFNT170S.WFM`);
  a file instead of a folder still works for AREA11.
- `--civ-route` (opt-in): the hub AREA18 s1 sends exits 0/1/2 to 11:0, 17:0 and 16:0 instead of 11:1, 17:1 and 16:4,
  and Stonetown's top exit (to the cut 16:1) continues to 16:4. The towns' own exits already lead on to 11:1 / 17:1
  and back to 18:1. Checked: walking right from the hub enters the rebuilt Stonetown. Needs `--civ-maps`.

**Residents.** AREA11 subtype 0 (the hard-hat worker, AREA11/CLUT02 row 4), AREA16 subtypes 0/1/2 (rows 21/37/53
of AREA16/CLUT07) and AREA17 subtype 0 (AREA17/CLUT03 row 1) are drawn from their surviving page with their real
row. The tables that order the frames are lost; the worker (frames 0..29) and the woman (30..56) animate with a rebuilt table: the frame order comes from the community GIFs of these residents (Tomba Unused Animations), whose frames match the page pixel-exact (worker: every frame; woman: idle and talk, walk inferred from the page layout and the worker's mirrored walk-back/front frames). The boy, the strongman and the Iron Town resident stay one standing figure. The other subtypes wear the
AREA08 villager (its page loaded at VRAM x 512) in one real resident row of the town (AREA11 36, AREA16 54,
AREA17 49), because the villager's own rows are not in the town's CLUT block. Scenery and props with lost tables
(roofs, signs, the lift, the platforms) are not drawn; their logic runs.

**Rooms.** The engine hides the map layer while a room is active (Tomba on a trigger cell inside house n) and draws
the room's furniture as objects, whose tables are lost. The port keeps the layer and builds it from `map8F_in.tim`
instead: black, with each house rectangle showing its matched interior (layout `interiors`). Back outside, the town
picture returns. The residents of room n appear as the engine shows them.

**Collision** (`tools/civ_rebuild.py`, layout `towns.N.collision`): editable shapes in picture px, traced over the
v2 pictures: `{"t": "wall", "poly": [...]}`, `{"t": "line", "pts": [...], "w": px}` (fences, railings), `{"t":
"rect", "r": [x, y, w, h]}`, `{"t": "water"}` (blocks like a wall: the top-down engine has no water or drop shape)
and `{"t": "floor"}` (carves, in order). Each item has a `note`. The raster becomes retail-style cells, because the
engine's 9 is a corner and not a fill (`topdownResolveCornerTile` pushes the probe out by the quadrant and gives up
when the next cell is blocked too, so a body walked into along its edge lets Tomba slide through): a solid cell with
one free side becomes the one-sided wall that pushes towards it (5 left, 6 up, 7 right, 8 down), half-covered corner
cells become diagonals 1-4, corners and the inside stay 9, as in AREA05/08/19. Then the houses (walls on the code's
trigger rectangle, or on `body` where a room-0 resident stands in its front yard, a door gap and a trigger interior),
the picture edge (shut except at the exit rectangles), the residents (every spawn and patrol box carved to floor) and
a reach check (flood fill from the entry spawn, layout `spawn`; a resident or exit that cannot be walked to is
reported). What is traced: Iron Town's rock, pillar fences, fortress walls, lamp posts, the key tower, the blocks of
the front building with a passage from the gate to the dome, the outer wall with the round gate, the lamps and the
bucket; Lumberjack Town's hedges and log fences, the spire, the big house, the windmill base, the deck house, bushes
and the saw-horse; Stonetown's rampart, the house wings, the broken pillar, the rock dome, the arches and the well.
These are our reading of the reconstructed pictures, not original data.

**Checked headless** (screenshots, frames dumped with `--dump-frames`): in each town, walking with the overlay on and
off, entering a house (AREA17 H5, AREA16 H1, AREA11 H3: the room view and its occupant appear) and a dialogue
(AREA17 resident 6 "¿Quieres subir?", AREA16 resident 7 "¿Entro o no entro...?", AREA11 resident 9).

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
| Palettes | resident rows survive (AREA16/CLUT07 row 53 confirmed, row 37 (the boy) confirmed by TCRF colours; 48/54, AREA11/CLUT02 4/36/68 confirmed by TCRF colours; AREA16 48/54, AREA17/CLUT03 1 confirmed from the side-view SPR table AREA17/D205) | load the area CLUT file whose rows 48..54 hold them; the TIM carries the picture's own palette |
| Text | PAL has "···??", JP has everything | translate the JP lines into the PAL slots (they already sit at the indices the code uses) |

In the port this is a data task, not a code task. The engine already runs AREA05/08/19 s0/s2, and loading I0NFO2 at
the overlay base works (docs/PORT.md). The cheapest test is to load AREA08 s0's picture, cell map and sprite tables
under AREA11 s0 with set 0D. The NPCs, houses and dialogue would then appear on the lake's deck (`plan 11 --from 8:0`
shows the overlap) before any art is made.

## USA disc (SCUS-94236)

Checked against the USA disc (*Tomba!*) with `tools/usa_compare.py USA_IMAGE USA_FS`. **Result: the USA disc has none of the
cut top-down data either.** It is the same build of the assets as PAL, renamed, so it brings back nothing PAL lost.

- **Layout.** One load-list set (`SYS/LDARnn.BIN`, `LDSYS.BIN`; PAL has LD0..LD8, one per language) and the file table inside
  `SCUS_942.36` at file offset 0x699A0 (1054 entries, same format as MAINn). Files were renumbered (PAL `SYSTEM/A07600.000`
  = USA `SYSTEM/A00153.000`, A07601 = A00154, ...), so a name diff is meaningless: 367 "USA-only" and 1392 "PAL-only" names in
  AREAnn + SYSTEM are almost all the same bytes under another name. The comparison is therefore by content, every load-list block
  and every whole file (GAM unpacked) against all of PAL.
- **The files named USA-only in AREA11/16/17/18** (`A003.GAM`, `B404.GAM`, `D303.000`, `D305.GAM`, `D401.000`, `D402.GAM`,
  `B103.GAM`, `Xnn.BIN`, `INFO2.BIN`) belong to the side-scrolling sections or the area list: kinds 0x10 GRPX, 0x30/0x34 SPR,
  0x54, 0x60 WPP and 0xF0 code. Their blocks are either byte-identical to PAL blocks or are text/code (below).
- **Section 0 load lists are empty on USA too:** AR11 `[12, 0, 21, 12, 0, 0]`, AR16 `[11, 0, 0, 0, 12, 13, 15, 13]`, AR17
  `[12, 0, 10, 13, 11, 11, 13, 0, ...]`, AR18 `[11, 0, 16, 12]` records per list (list 1 = section 0).
- **No new 0x8E / 0x8F / 0x3B..0x3E block anywhere.** The 302 USA blocks with no byte-identical PAL twin are kinds 0xF0 (code:
  `Xnn.BIN`, `INFO2.BIN`), 0x54 / 0xD0 / 0xD1 / 0x34 / 0x39 (English text, fonts, the HUD word sprites), 0x70, a few APD
  placement blocks 0x80..0x86 of side-scrolling sections, and 17 SYSTEM GRPX pages (title / menu graphics in English). The
  only 8 bpp TIMs on the disc are the working top-down pictures (AREA05 D003/D203, AREA08 D002/D202, AREA19 D005/D206, AREA02
  D005) plus the one inside X13.BIN; no ISO directory gap holds data (the gaps are directory sectors, the tail is zeros).
- **Orphan files.** Not in the SCUS table: AREA11 CLUT01/04/05, AREA16 CLUT01..03, AREA17 CLUT01/07..11, AREA18 CLUT01 and the
  per-area `INFO2.BIN` copies. The CLUTs are byte-identical to PAL (AREA11/16/17/18 CLUT01 is the same file in all four areas).
- **`INFO2.BIN`** (USA name of `I0NFO2.BIN`, one identical copy in SYSTEM and in every top-down area, 136672 bytes vs PAL 137436)
  is another build of the same overlay: the NPC_SET_0D/0E/0F/10 records of the cut towns are there with the same contents,
  0x340 bytes lower (e.g. PAL 0x8010009C = USA 0x800FFD3C); only the embedded pointers differ.
- **Female Stonetown Resident palette.** No USA block improves the match with TCRF's (colour-adjusted) sprite: the best rows are
  the same ones as on PAL (weighted mean nearest-colour distance about 24, ignoring black/white/grey), and the best row from
  USA-only data is worse (about 35). The page itself (`A07600` = USA `A00153`) and AREA16/CLUT07 are unchanged, so the rows 48 / 54
  guess stands.

**1998-02-03 English preview CD-R** ([BETA_980203.md](BETA_980203.md)): checked the same way, nothing new either. The
section-0 lists of AREA11/16/17/18 are empty, there is no new 0x8E/0x8F/0x3B..0x3E block and no new 8 bpp picture, the
only beta-only CLUT rows are HUD rows, and the town dialogue slots are already "···??".

**SLPM-80165 Japanese demo** ([JPDEMO.md](JPDEMO.md), "Civilization check"): nothing either. There are no AREA16/17/18
folders and no AREA11/16/17/18 load lists at all (the exe's named file table only lists SYSTEM, MOVIE, AREA00 and
`AREA15/0000.WSS`); `AREA11` holds only the unreferenced 294-byte `CA11.BIN`, a data table whose first 204 bytes match a
table at file offset 0x30D58 of PAL `X016.BIN` (side-scrolling AREA16 code, not top-down data). The only 8 bpp picture is
`AREA08/D002.000`, pixel-identical to PAL `AREA08/D002` (the known AREA08 map). No 32-byte run of the reload pages
A06100 / A07600 / A07601 / A08000 / A08600 / A08601 occurs anywhere on the disc; the demo-only SYSTEM pages are village,
pig, elder and menu sprites. No Civilization text (only the item names Stone/Iron Boomerang).

## Palette catalog (all discs)

`tools/clut_catalog.py catalog OUT` collects every palette on every disc we have and `tools/clut_catalog.py test OUT ...`
fits them against the lost colours. Sources: the `CLUTnn` files (and the bare `SYSTEM/DSPCLUT.TIM`), the small load-list
CLUT blocks (`.000` files of 32..1024 bytes, e.g. PAL `SYSTEM/A00007` 256 x 1, `A00805` 16 x 16), every TIM CLUT at any
4-aligned offset of any file (this includes the 0x8E/0x8F top-down pictures, TIMs inside chunks and in the exe/overlays),
PrePre 8 `CLUT00` (640 x 32) and `SYS001/002/004/006/007`, and a conservative scan for 16-colour runs in the remaining
non-page data (entry 0 = 0, 15 non-zero 15-bit colours, >= 12 distinct, at least two runs in a row). Every source is cut into
16-colour rows and 256-colour windows (16-entry steps) and hashed. SLPM-80165 was added later (8 CLUT files, 40 demo-only rows: AREA00 `CLUTnn` HUD/village rows and 16-colour runs in `AREA03/CA03.BIN`).

| Disc | Distinct 16-colour rows | Only on this disc | Where the disc-only rows are |
|---|---|---|---|
| PAL SCES-01331 | 1981 | 59 | 256 x 1 title/menu palettes `SYSTEM/A00203`, `A00402`, `A00502`, `A09401`; the two TIMs in `SCES_013.31` (CLUT at (128,480) and (320,480)) |
| USA SCUS-94236 | 1980 | 26 | `SYSTEM/A00013`, `A001901` (English menu palettes); 16-colour runs in `X14.BIN` and `DSPSUB3.BIN` |
| JP SLPS-01144 | 1851 | 308 | the same HUD/system rows in nearly every `CLUTnn` (rows 2, 4, 5..7, 16..25 at fixed columns), some area-specific rows, `DSPCLUT.TIM`, the AREA19 `D005` picture CLUT |
| PrePre 8 PCPX-96084 | 269 | 62 | `CLUT00` (59 rows), `SYS006`, two TIMs in `TOMBA.EXE` |
| JP demo SLPM-80165 | 431 | 40 | AREA00 `CLUT01..06` (HUD/village rows), 16-colour runs in `AREA03/CA03.BIN` |

1305 rows are on PAL, USA and JP; 171 also on PrePre 8. No AREA11/16/17/18 `CLUTnn` row is disc-only except the JP
HUD/system rows, which are on every area.

**Tests, no match.**
- Female Stonetown Resident (TCRF sprite, weighted mean nearest-colour distance over its non-grey colours, every row on
  every disc): best 24.1 (a row shared by PAL/USA/JP, AREA00/CLUT06), best JP-only row 26.9, best PrePre 8 row 29.8. No
  disc-only palette improves on the AREA16/CLUT07 rows 48 / 54 guess.
- Scenery pages, community renders as index → colour reference, pixel-weighted mean error of every 256-colour window:
  A08000 72.9, A08600 67.4 (a 256 x 1 system palette), A06100 82.4; the best PrePre 8 windows are worse (85.4 / 72.3 /
  94.8), and so are the best SLPM-80165 windows (A08000 83.6, A08600 67.5 with `SYSTEM/A00009` = PAL
  `A00201`, A06100 95.7); the demo's best sprite row (24.1) is the shared AREA00/CLUT06 row. Renders with the best windows are noise. The scenery CLUTs are not on any of these discs.

### Wide-tolerance palette search (scenery CLUTs)

After the Female Stonetown Resident's real row turned out to differ from TCRF's colours by up to 9 levels, the scenery
search was redone with a much looser match (`tools/clut_wide/`, scripts only).
- **Corpus:** every file of PAL, USA, JP, the beta, SLPM-80165 and PrePre 8 (both dumps), except `.STR`. GAM files are
  read raw and unpacked; duplicates are removed (3283 unique files). Both byte phases give 272 M words.
- **Layouts:** a 256-entry window at **every** word offset with strides 1, 2, 4 and 16. This covers 256 x N / 512 x 1
  VRAM images, 16 x 16 blocks, interleaved rows and columns. Separately, the best 16-entry block anywhere for each CLUT
  row (non-contiguous assembly).
- **Targets:** the quantized custom CLUTs of `civ_rebuild.py`. Only indices measured by clutgen or the FMV anchors are
  used (interpolated ones are ignored), weighted by how many page pixels use them.
- **Metrics:**
  - raw: per index, the max channel difference in 5-bit levels, and the weighted share within 6 / 9 / 12;
  - corrected: a weighted per-channel affine fit (gain 0.5..2, computed for every offset by FFT correlation), then the
    same shares;
  - windows with fewer than 80 distinct colours, or less than 2 levels of spread per channel, are dropped.
- **Positive control:** the real 256-colour CLUT of AREA08/D002, perturbed like an FMV CLUT (gain 0.85, +2, noise
  sigma 3, 10 % random entries), is found first with 83 / 94 / 96 % within 6 / 9 / 12. The best unrelated window
  reaches 24 / 52 / 64 %.
- **Result (best window per page, raw w6/9/12 -> corrected):**

| Page | Best window | Raw | Gain-corrected |
|------|-------------|-----|----------------|
| A06100 | AREA02/CLUT03 unpacked 0x3DAE (all discs) | 31/52/66 % | 50/74/86 % |
| A07600/01 | JP AREA09/CLUT04 0x1294 stride 2 | 28/44/58 % | 48/70/81 % |
| A08000 | AREA11/CLUT03 unpacked 0x3E1A | 43/69/79 % | 67/84/88 % |
| A08600/01 | USA AREA05/INFO2.BIN 0x130 stride 2; USA X03.BIN 0x2E510 stride 4 | 46/66/79, 42/78/94 % | 67/88/97, 65/94/98 % |

  Every fit hits the gain floor (0.5). It flattens the window towards the target's mean, so the corrected shares
  reward low contrast, not a match. Rendered with these windows, every page is noise. The 16-entry block assembly
  takes each row from a different, unrelated file (CLUT files of other areas, a font, overlays), which is chance.
- **Conclusion:** no real scenery palette exists on any disc we have, even at 12 levels of tolerance with a gain/offset
  correction. The custom FMV-derived CLUTs stay.

## Reconstruction (not original)

`tools/civ_rebuild.py` builds a **best-effort** background picture (kind 0x8F) and cell map (kind 0x8E) for AREA11,
AREA16 and AREA17 s0, from your own disc plus the layout file `tools/civ_rebuild_layout.json`. That file holds only
coordinates and reasons, no pixels. None of the output is original data.

```
python3 tools/civ_rebuild.py [--area 17] [--both | --no-fmv] [--refdir DIR] [--community-cluts DIR] [--preview DIR]
```

Output goes to `build/civ_rebuild/AREAnn/` (`AREAnn_nofmv/` without the FMV layer):

- `map8F.tim`: the 8 bpp TIM exactly as the engine reads kind 0x8F (CLUT at +0x14, pixels at +0x220).
- `map8E.bin`: the cell map as kind 0x8E (`u16 w, u16 h`, then 0x4000 bytes).
- `picture.png`, `cells.png`, `preview.png` (residents, patrol boxes, houses, props and exits overlaid), `source.png`.
- `manifest.json`: CLUT provenance and the warnings from the cell-map checks.

Sizes: AREA11 is 736 x 672 (92 x 84 cells), AREA16 is 544 x 608 (68 x 76), AREA17 is 832 x 704 (104 x 88).

**What is real**
- Every cut-out pixel and its index comes from the SYS reload pages. Page identities: A06100 is Lumberjack Town,
  A08000 is Iron Town, A07600 and A07601 are Stonetown, A08600 and A08601 are AREA18. Each pair shares one CLUT;
  this was confirmed by the community files, which share a palette across each pair.
- The house rectangles, residents, patrol boxes and exit rectangles are read from I0NFO2 and the StepC/D/E code
  at build time.

**What is ours**
- **Colours: FMV-derived, not original.** For each index, the first available source wins:
  1. `--community-cluts`: the community *clutgen* pairs (labelled "community FMV-derived CLUT (clutgen)"), re-run
     with their own method. Each 2x2 block of a `PHOT*.png` is averaged into the index whose `GGDdefault.act` colour
     the matching `CLUT*.png` pixel has.
  2. `--refdir`: the community renders 125/127/128 (an exact index-to-colour mapping).
  3. Our BUNMEI.STR anchors (`fmv_anchors`): page patches registered in the median mosaic of the flyover.
  4. Harmonic interpolation over the page's index adjacency, which comes within 18-26 per channel when tested on
     known pages.

  The result is **quantized to the PS1's 15-bit colour**: each channel becomes `round(c*31/255)`, shown back as
  `round(v*255/31)`, the same rule as `tools/clut_from_sprite.py`. This applies to every scenery CLUT, the town
  picture CLUT and `map8F.tim`, whose CLUT holds those 15-bit words with the same STP rule as before. The change is
  at most 4 and about 2 on average per channel. Some indices collapse onto the same 15-bit colour: 26 (A08000),
  25 (A06100) and 33 (A07600/01). The manifest lists each CLUT as hex 15-bit words (`clut.clut15`), together with
  the maximum and mean change and the collapse count.
  The `1..6.bmp` files in that folder carry only the default palette (`GGDdefault.act`), not a finished CLUT, so they
  are used only to identify the pages. Agreement between sources: clutgen and the renders differ by 23.6 (A08000)
  and 15.1 (A06100) mean per channel; the renders and our FMV anchors differ by 17.2 (A08000). Stonetown has no
  render, so its CLUT is clutgen plus our anchors plus fill.
- **FMV layer.** BUNMEI.STR pans across each town at 1 FMV pixel = 1 picture pixel. The median of the pan (frames
  61-91 Iron, 6-30 Lumberjack, 31-60 Stonetown) removes the sparkles. It is re-indexed into the town CLUT and laid
  under the cut-outs. Its placement:
  - Iron Town is pinned by the code. The dome fills house H1, the gate, dome and top-door axis is world x 223 (the
    centre of both exit rectangles), and the people in the FMV stand on residents #15, #6 and #7.
  - Lumberjack Town and Stonetown are our guesses. The old man is placed on resident #11, and the strong man on
    resident #2.

  The FMV's own people are erased, leaving flat ground patches. `--no-fmv` drops this layer.
- **Placement.** Real cut-outs go where they register on the FMV (the A08000 double-gate cut-out lands on the
  round gate). Elsewhere it is our design:
  - houses outside the FMV are a roof of page tiles, a real wall cut-out tiled along the front, and a door;
  - fences continue the FMV's own fences;
  - the ground is FMV or page tiles.

  Every choice is recorded with its reason in the layout file.
- **Collision.**
  - Shape 9 is used for the map border, for the house blocks and for our "solid" rectangles.
  - House interiors and door gaps are trigger floor (0x80). The engine resets the room whenever the player stands on
    a non-trigger cell, as AREA05 and AREA19 show.
  - The exit rectangles from the code are kept open.
  - Every resident and patrol box is forced onto floor, and each carved cell is reported.
  - No diagonal shapes are used.

### House interiors and community layout (v2, our guesses)

**Layout source: the community assemblies.** We used community images 129-132 (credited here as the layout source)
to see how the page fragments join.
- How they were matched: every 8x8 window of an assembly was matched exactly against our pages, by colour↔index
  relabelling. Votes on the offset give each fragment's page rectangle and its position in the object. The
  rectangles were then grown pixel by pixel while they still agreed.
- Small corrections: isolated slivers were dropped, and pixels of neighbouring cut-outs packed inside the same page
  rectangle are skipped as "cuts". Flat floors that the window matcher skips were found by an offset search; in the
  Stonestown stool room this added A07600 128,122,44,80 and 129,129,104,42.
- What is stored: `assemblies` in the layout lists, for every object, its fragments `[page, sx, sy, w, h, x, y(,
  cuts)]` and how much of the community object they reproduce. All pixels come from your own disc.

There are 19 rooms in all:
- 5 Iron Town rooms (4 chain-drum crank huts and the pipe room);
- 4 Stonestown rooms;
- 3 Lumberjack rooms;
- 7 rooms on the AREA18 page A08600. A08601 holds walls only.

`--rooms OUT` draws each room three ways: the fragments apart, assembled, and assembled with the filled pixels in
magenta. The fill only closes seams and pinholes: a morphological closing of radius 2, filled from the nearest pixel
in the same row or column, 0.2-1.4 % of each room. Each room is clipped to its house rectangle only after it is
assembled.

**Towns.**
- Iron Town takes the full south wall with the round gate and the dome's barrel roof as the community joined them
  in image 131. Both are anchored on the FMV by colour matching.
- Lumberjack Town takes the windmill block (anchored on the FMV), the long fence, and the three house exteriors from
  image 132, set on the code's house rectangles (bottom-aligned).
- Stonestown gets the "Stone the No.1" slab (A07601, Folkin). The code and the FMV disagree about the well: exit 1,
  the hole to the Dried Wishing Well, is about 40 px east of the FMV's keyhole well. The exit cells stay open, as the
  code says.
- Section names follow the D0SPSUB table (0x80108E5C).
- The ground is grid tiles broken up by randomly placed, irregularly masked patches sampled from the FMV and the
  pages.
- The people baked into the FMV are inpainted by diffusion rather than replaced with ground.

**FMV stripes.**
- Each frame's near-black border rows and columns are dropped before the median.
- Thin dark bands, solid or dotted, that remain in the median are replaced by vertical interpolation. A faint
  remnant of one dotted band can remain near the Iron Town dome arch; the community mockup 133 shows the same band.

`aNN_vs_mockup.png` compares our Iron Town and Lumberjack Town with the community mockups 133 and 134.

**AREA17 (Iron Town)**

| House | Assembled room | Confidence | Why |
|---|---|---|---|
| H1 | `room_iron_pipe` (A08000, 1 fragments, filled 0.4 %) | plausible | H1, the dome (93x133): the tall pipe room (75x138), the only iron room as tall as the dome |
| H2 | `room_iron_hut_c` (A08000, 5 fragments, filled 0.8 %) | plausible | Iron Town has four chain-drum crank huts (Folkin) and four houses besides the dome, so every other house gets one. Which hut goes where is a guess, by size: 117x135 for the 126x140 house |
| H3 | `room_iron_hut_d` (A08000, 2 fragments, filled 0.7 %) | plausible | H3 ('the lift used to run on machinery'): a chain-drum hut with a doorway, the smallest (104x117) for the smallest house (115x95) |
| H4 | `room_iron_hut_b` (A08000, 4 fragments, filled 1.3 %) | guess | H4 ('good material! wait a moment'): the chain-drum hut with a plant shelf |
| H5 | `room_iron_hut_a` (A08000, 2 fragments, filled 0.6 %) | guess | H5, the smith with the iron gear: the largest chain-drum hut (118x143), whose coal basket reads as a forge |

**AREA11 (Lumberjack Town)**

| House | Assembled room | Confidence | Why |
|---|---|---|---|
| H1 | `room_wood_logs` (A06100, 1 fragments, filled 1.2 %) | plausible | H1, the two workers ('now I can make something'): the log workshop with tools and a pot |
| H2 | `room_wood_table` (A06100, 4 fragments, filled 0.2 %) | guess | H2: the remaining wooden room (round table on a green rug) |
| H3 | `room_wood_zzz` (A06100, 1 fragments, filled 0.5 %) | plausible | H3, next to resident #4 who says 'a fortune-teller lives in this hut': the ornate octagonal room with the 'ZZZ' sign; same width as the house |

**AREA16 (Stonestown)**

| House | Assembled room | Confidence | Why |
|---|---|---|---|
| H1 | `room_stone_chair` (A07600, 3 fragments, filled 0.6 %) | plausible | H1, the strongman, largest house: the largest stone room |
| H2 | `room_stone_stool` (A07600, 5 fragments, filled 0.8 %) | guess | H2 (#12, no dialogue): the remaining room, drawn clipped |
| H3 | `room_stone_closet` (A07600, 7 fragments, filled 0.9 %) | plausible | H3, top right: the teal room with the inner wooden door = Folkin's locked closet |
| H4 | `room_stone_shelves` (A07600, 4 fragments, filled 1.1 %) | plausible | H4, the craftsman: shelves, jugs and a book |

Nothing derived from the disc, the FMV or the community files is committed. The palettes are rebuilt at every run.

## Folkin's rough notes — verification

Folkin (TCRF) left rough notes on the Civilization towns. Each claim was checked against the decompiled code and the
PAL disc (USA/JP where noted). Verdicts: **confirmed**, **refuted**, **partly** (part holds, part does not),
**unverifiable** (the data needed is not on the disc, or the claim is visual and we cannot pin it down).

**New anchor: the per-section room-name table.** D0SPSUB `0x80108E5C` (20 pointers, one per area) points into a list
of English name pointers (strings in D0SPSUB's 16-bit font code: `0xED`+n = a..z, `0xCD`+n = A..Z, `0xAC` = space).
Read in section order it gives:
- AREA03: s0/s1 Stormy Mountain, **s2 Lava Caves**, s3 Phoenix's Nest;
- AREA11: **s0 Lumberjack Town**, s1 Lumberjack Factory, **s2 Dried Wishing Well**, s3 Factory, s4 Dried Wishing Well;
- AREA16: **s0 Stonestown**, s1..s6 Clock Tower;
- AREA17: **s0 Iron Town**, s1..s10 Iron Castle.

So the cut top-down sections had retail names: Lumberjack Town, Stonestown and Iron Town.

| # | Claim | Verdict | Evidence |
|---|---|---|---|
| 1a | Lava Caves purification FMV differs | **confirmed** | `MOVIE/HONOO2.STR` (炎 "flame", 230 frames) shows rain clouds and rain putting out the lava. HANA, KARAKURI and the others (BAKKASU, ARASHI, JUNGLE, MANTION: 232 frames) show a sparkle/light sweep. There is no HONOO1 on the disc. |
| 1b | The cowboy in the Lava Caves says rain could put out the flames | **confirmed** | `flameBlockedNpcTalk` (X003 0x80132D80) runs in AREA03 sections >= 2. It shows (7,0..3) = MFNT030 #249-#253 (English: "If we weren't inside a cave, maybe some rain could help put out these flames"). The same NPC starts event **0xAC "The Fire Pig Bag"** (`setEventStarted(0xAC)`). |
| 1c | Rain Essence can only be used in the Lava Caves | **confirmed** | Item-use check in D0SPSUB `subScreenItemListStep`: `D_800FECE8[item]` picks a condition list in `D_800FFAF8`, run through `D_800FFBB0`. For item 52 (list 16, 0x800FF290): `area != 3 → no`, `section != 2 → no`, else yes. AREA03 s2 is "Lava Caves" (table above). No X003 code reads `useItemId == 0x34`, so using it there does nothing. JP description: 雨雲を呼んで雨をふらせる奇跡のアイテム ("calls rain clouds and makes it rain"). |
| 1d | The "Fire Pig Bag" event icon is the Lava Caves icon | **refuted** | Icons are `ICONS[event-1]` (D0SPSUB 0x800FE974). Event 0xAC (Fire Pig Bag, CFNT #54) uses frame 0xA6 (uv 0x98,0x68). Event 0x22 (Lava Caves, CFNT #55) uses frame 0x82 (uv 0xE0,0x28). The two cells have different pixels (A00300 words 128..191). |
| 1e | The Green Evil Pig Bag is in the Lumberjack Factory VRAM, so it was found there | **partly** | A pig-faced bag is on `AREA11/B101` (factory s1, VRAM x 904, v 0..24). But the same pixels are on pages of AREA02, 03, 05, 08, 10, 18 and 19 (e.g. `AREA08/B302`, `AREA19/B003`). It is part of a shared object strip, so it proves nothing about the factory. X011 has no copy of the pig-bag portal code (`updateEvilPigBagPortal`: only X002, X010, X016). Its colour is not fixed by any SPR part we found. |
| — | Overall: the Fire Pig, not the Lava Caves, cursed Civilization | **unverifiable** | 1a-1c show that rain at the Lava Caves was planned. In retail, Fire Pig Bag is already the Lava Caves bag (0xAC is started there). Nothing in the code ties it to the Civilization curse. |
| 2 | Iron Wheel can only be used in the Clock Tower | **confirmed (retail)** | Item 62 (list 20, 0x800FF3A0): `area != 16 → no`; `section 3 or 6 → yes`. AREA16 s1..s6 are "Clock Tower". X016 `checkStoneGearsTrigger` / `updateBrokenStoneGearsNpc` consume it (`useItemId == 0x3E`) and `brokenStoneGearsRepair` removes it. The item is used in retail, not cut. |
| 3a | Iron Town is entered from the bottom; the top path leads into the Iron Castle | **confirmed** | `topdownSectionStepE`: exit 0 at y > 0x23A goes to AREA18 (the hub); exit 1 at y < 0x50 goes to AREA17 s1, named "Iron Castle". Resident #11: "go straight down to leave". |
| 3b | A crank left of the "Iron the No.1" sign | **partly** | A08000 holds a crank wheel with a handle in 6 rotation frames (page px 817..896, 152..202 = VRAM words 472..512). Its position next to the sign needs the lost map. |
| 3c | Sign-falling-over sprites | **unverifiable** | A08000 has two signboard cut-outs on a post (px 690..728, 128..143), but no tilt sequence we can read. |
| 3d | Chain lifts with 4 crank huts, with interior sprites | **confirmed** | A08000 has four interiors, each with a chain drum on a hexagonal base and the chain running up the wall (px 128..390). Code: two moving platforms (props `wb8` 0xD/0xE, `TOPDOWN_A17_SCENERY_LIMITS`) and the lines "want to go up? go into this hut", "the lift used to run on machinery", "try the lift hut". There are five houses, four of them with a hut room. |
| 4a | A different (cursed) Clock Tower render | **unverifiable** | No clock-tower art identified on A07600/A07601. AREA16 s1..s6 reload pages (A077xx/A078xx/A079xx) are byte-identical to the retail B300..B600 pages. |
| 4b | A "Stone the No.1" sign sprite | **partly** | A07601 has a stone slab with raised letters (px 137..176, 200..240); only "...ton..." can be made out. It is the counterpart of the Iron sign, but not legible enough to be sure. |
| 4c | The well leads to the "Dried up wishing well" area | **confirmed** | `topdownSectionStepD` exit 1 is a 7 x 1 px hole (x 0x6B..0x72, y 0x13F..0x140), not a doorway, and leads to AREA11 s2 spawn 0. AREA11 s2's retail name is "Dried Wishing Well". |
| 4d | A locked closet in the top-right house | **unverifiable** | H3 (219,176)-(299,258) is the top-right house: one type-2 prop (room 3), no resident and no code for a lock. A07600 has a teal room with a wooden inner door (px 280,154), used for H3 in the reconstruction above. |
| 5a | Lumberjack Town has an underground section under the floor | **confirmed (text)** | JP lines (4,3) "there's a maze under the floor" and (4,4) "you can only reach that hut from under the floor" (`topdownA11ActorOpenDialogue` cases 6/7). |
| 5b | The factory is reached by the top road | **confirmed** | `topdownSectionStepC` exit 1 at y < 0x4F (x 0xC7..0x107) goes to AREA11 s1, "Lumberjack Factory". Resident #10: "the workshop entrance is up there". |
| 5c | A rock in the Lumberjack Factory | **unverifiable** | No rock/blast object found in X011 or the factory pages. |
| 6 | Two Female Stonetown Residents (#1, #2), a Stonetown Kid, a Male; the Male has top-down and side sprites | **partly** | NPC set 0E uses four subtypes. **0** = the hooded woman (#3, #5, feminine lines: #241 あたしも). **1** = the kid (#7, #8, childish lines, the secret door). **2** = the strongman (#4; scripts 16-23 are lifting anims, frames 120-129). **3** = 8 residents (#1, #2, #6, #9-#13) on a lost page (box 16x32, male speech おれ). A07600 has one woman frame set (frames 30-59). An actor's colour comes from its SPR parts, so one subtype gives one palette: the code has room for **one** female type. A second woman can only be the same drawing in another palette (rows 48/54 of AREA16/CLUT07, or TCRF's own CLUT, see image list), and no surviving code uses it. The strongman's top-down frames (front, back, side) are on A07600; its side-scroller frames are on the retail page B201 (AREA10/16/17/18). |
| 7 | The secret-door line («Abrí en secreto la puerta…») is resident 8 of AREA16 | **confirmed** | Actor 7 (#8 on the sheets: sub 1 kid, b0c 8, diagonal patrol, spawns after the first step) uses row b0c 8 = (3, 0/9/13/13). (3,9) = MFNT160 #239 = JP block #216: へへ・・・こっそり、鉄人の町と木人の町のトビラ・・・あけてきちゃった・・・ないしょだよ ("hehe... I secretly opened the door between the Iron Men's and the Wood People's towns... it's a secret"). At progress 2/3 it becomes #243 (JP #220): "they found out my prank... but it's no fun when everyone fights". |

Also found: item 109 (Black Water) passes the use-check only in AREA18 s2 with the player's H between 128 and 192
(list 22, 0x800FF3E8). No code reacts to it.

**Images** (not committed; regenerate from your own disc). The Stonetown and Iron Town pages are 8 bpp with no
surviving CLUT, so they are coloured with the FMV-derived CLUT of `tools/civ_rebuild.py`, which is **not original**.
Resident sprites use real CLUT rows where known:
- `stone_sign_A07601.png`
- `crank_huts_A08000.png` (the four chain-drum huts)
- `cranks_signs_A08000.png` (crank wheel frames, posts, signboards)
- `rails_A08000.png`
- `pig_bag_factory_B101.png` (CLUT02 rows as candidates; the real row is unknown)
- `stonetown_residents_palettes.png` (A07600 with AREA16/CLUT07 rows 48, 54, 37, 53 and TCRF's derived CLUT)
- `icons_lava_vs_firebag.png`
- `fmv/sheet.png` (HONOO2 vs HANA vs KARAKURI)
