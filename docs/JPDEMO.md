# Japanese demo disc (SLPM-80165)

Survey of the Japanese pre-release demo ("Ore! Tomba" taikenban, boot `SLPM_801.65`) against PAL SCES-01331, for
decomp reuse and cut-content hunting. Like `game/`, the demo image and anything extracted from it stay local and
are never committed. Numbers come from `tools/xmatch.py` (added for this survey) and one-off scripts that compared
masked code windows and GAM chunks.

## Disc layout

| Demo | Contents | PAL counterpart |
|---|---|---|
| `SLPM_801.65` | main exe, 0x79000 bytes at 0x80010000, entry 0x8005E98C | `MAIN0.EXE` (0x89000 bytes, entry 0x8006C044) |
| `SYSTEM/X00.BIN` = `AREA00/X00.BIN` | the shared overlay (identical copies) | `X000.BIN` |
| `SYSTEM/DSPSUB.BIN`, `DSPSUB3.BIN`, `OPTSUB00.BIN` | sub-screen overlays | `SYSTEM/D?SPSUB*.BIN`, `O?PTSUB0.BIN` |
| `SYSTEM/A000nn.*` (23 files) | system GAMs/data; `A00013/14/16.000` are text banks in **WFM1** format | PAL `SYSTEM/A0xxxx` + per-area `CFNT/MFNT*.WFM` (**WFM3**) |
| `AREA00/` | only playable area (A/B/D/CLUT GAMs) | PAL `AREA00` |
| `AREA15/0000.WSS` | one WSS script | PAL `AREA15/0000.WSS` (different) |
| `MOVIE/` | `100JP`, `BOY`, `MIST`, `OP_UTA`, `TOMBA` | PAL has `BOY`/`MIST`; `100JP`, `OP_UTA`, `TOMBA` are demo only |
| `AREA01..14/CA*.BIN`, `AREA08/*` | **not referenced by the exe**, see below | |
| `ZZZ/DUMMY.DAT` | padding | identical to PAL |

The demo exe has its own file table: about 70 entries of `" /DIR/NAME"` (15 chars) + LBA + size near file offset
0x5C330. It lists only `SYSTEM`, `MOVIE`, `AREA00` and `AREA15/0000.WSS`. PAL uses a table without names
(`tools/cdfiles.py`).

Overlay bases come from where jal targets land on prologues, so they are estimates: the first word of each overlay
is a small ID (X00=7, CA01..CA14=9..0x16, INFO=0xA, DSPSUB=0x17, DSPSUB3=0x19, OPTSUB00=0x1A), and an entry-pointer
table follows it in the code overlays. X00, DSPSUB, DSPSUB3 and OPTSUB00 load at 0x800E7698 (PAL X000: 0x800E8028).
All CAnn files load at **0x8010F560**, which is inside X00's range. So the demo seems to load an area overlay over
the tail of X00. PAL ships whole `X0nn.BIN` files (X000 shared code plus that area's code) instead.

## Code comparison

Masked match (relocations and non-`$sp` immediates ignored, whole functions only):

| Demo file | PAL functions found | bytes covered | same function has C in repo |
|---|---|---|---|
| exe | 834 (831 from main0, i.e. 58% of the 1443 sized main0 functions) | 148 KB / 496 KB (30%) | 616 funcs, 118 KB |
| X00.BIN | 164 (146 x000) | 62 KB / 293 KB (21%) | 156 funcs, 62 KB |
| CA01.BIN | 109 (94 x001) | 43 KB / 138 KB (31%) | 101 |
| CA03/04/09 | 13 / 17 / 27 (x003 / x004 / x009) | 8-23% | 9 / 13 / 23 |
| INFO, DSPSUB, DSPSUB3, OPTSUB00 | only a few tiny helpers | under 1% | none |

- Only about 145 exe functions are byte-identical, because most addresses moved. In the exe, demo minus PAL
  addresses is mostly -0xB000 or -0xE000. Matched functions span 0x8001572C..0x80069F3C.
- Looser 8-word window overlap: exe vs MAIN0 60%, X00 vs X000 53%, INFO vs `I0NFO` 49%, OPTSUB00 vs `O0PTSUB0` 28%,
  DSPSUB vs `D0SPSUB` 19%. Most unmatched functions are edited versions, not new code. Relaxing the mask further
  (all immediates) adds only 3 exe functions and 11 X00 functions, so the remaining differences are real
  instruction changes.
- The PsyQ library `$Id` strings (bios.c 1.81, intr.c 1.74, sys.c 1.129) are the same as PAL, so it is likely the
  same SDK and compiler.
- The demo exe has code that PAL does not have, found from strings that PAL lacks:
  - **Debug / test menu**: `CDROM LOAD TEST`, `MEMORY CARD : ACCESS TEST`, `MUSIC PLAY TEST`, `SOUND OUTPUT TEST`,
    with reverb modes (ROOM, STUDIO small/medium/large, HALL, SPACE ECHO, PIPE ECHO, DELAY, ECHO), BGM/voice/pitch
    controls and a `(%2d) %s : %6d Bytes` file-load listing. Code is around 0x8004FFEC..0x80050D08.
  - `PL 1 SCR 2 TMP 3 DSP 4` (around 0x8002A900) and `X hosei=%04d` (X correction, around 0x8002AC9C): on-screen
    debug.
  - Demo flow strings: `TO BE CONTINUED...`, `TRY AGAIN...`, `NEW  START`, `NOW LOADING`, `PUSH [START] BUTTON`,
    `MOVIE START`, `OPTION MODE`.

## Cut-content leads

1. **Unreferenced area overlays `AREA01..14/CAnn.BIN`.** These are early per-area code blobs that the exe never
   loads. Matching ties most of them to the PAL area overlays with the same number: CA01 to X001 (68% window
   overlap), CA02 to X002, CA03 to X003, CA04 to X004, CA05 to X005, CA06 to X006, CA09 to X009, CA14 to X014.
   Exceptions:
   - **CA07** has about 4 KB of code that matches no PAL overlay (8% at most). In PAL, AREA07 has no code of its
     own and reuses `X001`. This is the strongest lead for cut area-7-specific logic.
   - **CA10** (616 B) looks like X000 code, not X010.
   - **CA11** (294 B of data) best matches **X016** (68%). This hints that areas were renumbered before release.
   - CA08 is 130 B of data that matches X008. CA12 and CA13 hold only the ID word (empty areas). In PAL, AREA12
     reuses X004.
2. **`AREA08/INFO.BIN`**: an early debug info overlay (49% overlap with PAL `I0NFO.BIN`). It contains printf formats
   that PAL lacks, for player and enemy state: `plX/plY/rot/spd`, `spx/spy/r0..r2`, `be/atk/ata/dir/ps`,
   `ecp/ehf/edi`, and `EVE`/`TAL` value dumps.
3. **`AREA08/SND002_.WVD`**: a sound bank whose name has no PAL counterpart (PAL has `SND001_`, `SND0020`,
   `SND0023`). Unreferenced. Worth checking with `tools/audio.py`.
4. **`AREA08` GAMs** (A003, B000, B001, CLUT01, D002.000, D003): unreferenced. B001 and D003 partly match PAL AREA08
   (80% and 69% of chunks), so they are an early AREA08 room set.
5. **Demo AREA00 room data that PAL lacks.** The demo has D001..D503 (six room groups D0..D5), `A005.000` and
   `A006.GAM`. Chunk comparison of the unpacked GAMs against all 816 PAL GAMs:
   - D003/D103/D203 (296 KB each) and D402/D503: 0-2% of chunks found in PAL.
   - B400: 0% found.
   - A002: 0.5% found.
   - B000/B100/B200: about 40% found.
   - B500: 82% found.

   These are early versions of AREA00 rooms and graphics, worth rendering with `tools/room.py` and
   `tools/extract_sprites.py` once the GAM/D-file formats are confirmed to be the same. PAL AREA00 has
   B301/B401/B502/B503/D504 and the A?06 set, which the demo lacks.
6. **Movies** `100JP.STR`, `OP_UTA.STR` (opening with vocals) and `TOMBA.STR` (likely a demo promo or attract
   video) have no PAL equivalent.
7. **Text**: the three demo text banks use the older WFM1 container (PAL: WFM3). `tools/wfm.py` targets WFM3, so
   dumping the demo script text needs a WFM1 reader. Possible leads are early or cut dialogue.

Not done yet: `tools/objects.py` and `tools/room.py` are wired to PAL overlays and symbols. Object and event lists
for the demo need the X00 symbol mapping from `tools/xmatch.py --csv` first.

## Findings from the extracted files

Tools: `tools/jpdemo_text.py` (WFM1 text dump + match against retail JP), `tools/jpdemo_rooms.py` (demo WMD rooms
rendered with the PAL renderer), `tools/audio.py wvd` (now accepts the demo's header-first WVD layout).

- **Text.** WFM1 = WFM2 without the 128-byte group table (glyph pointers at 0x10). Blocks: `SYSTEM/A00013` (= A00016,
  201 item descriptions), `SYSTEM/A00014` (200 event titles, most still `イベント０xx` placeholders), and inside the
  unpacked `AREA00/A006.GAM` and `AREA08/A003.GAM`. 54 new kanji were added to `tools/jp_charmap.json`. Against retail JP,
  21 dialogues have no close match: the elder's end-of-demo speech, an elder line offering to **appraise unknown items**
  (no such feature in the final game), early place names (始まりの村, 始まりの森, ふるいけ), AREA08 lines about a broken
  machine that "the guy in red clothes" could fix, and reworded item descriptions.
- **AREA08 is early AREA02.** 158 of the 228 dialogues of `AREA08/A003.GAM` match retail `AREA02/A003.GAM`, so the
  unreferenced AREA08 set is the area later renumbered to 02 (compare CA11 -> X016).
- **Rooms.** Demo `D003/D103/D203` (identical) are the village WMD in an older packet layout (GPU order: colour+cmd,
  uv+clut, uv+tpage, uv, uv, then `x y z pad` vertices; some sectors add a fifth group of untextured quads). Rendered
  with `B?00` + `CLUT0?` at PAL positions, it is the PAL `D001` village with differences: houses without thatched
  roofs, a different house by the river, a missing pillar. `D302/D402/D503` and `D?01` are WHM collision, `D?02.000`
  and `A004.000` are WVD sound banks (not pictures). Demo `B300` = PAL `B300`+`B301` in one file. Demo `B400` is a
  different sprite page (effects and small creature sprites). `AREA08/D003` is not a WMD.
- **CA07** is an early `updateAreaScroll` for area 7: per-room exit/camera checks against the player position
  (`0x1F80016A/16E/172`) with the same thresholds as `src/x001/updateAreaScroll.c` (0xB6E, 0x109B, 0x779, 0x7FC, 0xFAB).
  PAL AREA07 runs X001, which kept this table, so it is not cut logic.
- **Sound.** `AREA08/SND002_.WVD`: 52 samples, 39 found in PAL banks (`SND_INI`, `SND0020`, ...), 13 demo-only.

Insertion into the PAL port: the text lines are easy (WFM3 encoder exists; a translated appraisal/extra line could be
attached to an NPC through the script/WSS side). The early village model is renderable but would need the old packet
layout converted to PAL WMD (straightforward: same vertices/uv, reorder fields) and its WHM/textures loaded together;
cosmetic only. The debug/test menu in the demo exe remains the most valuable code lead; CA07 is not.

## Civilization check

Same checks as for PAL, USA, JP, PrePre 8 and the 1998-02-03 beta (docs/CIVILIZATION.md). **Result: nothing of the
cut top-down towns.**

- **No AREA16/17/18 folders, no load lists.** The exe's named file table covers only SYSTEM, MOVIE, AREA00 and
  `AREA15/0000.WSS`, so there is no AREA11/16/17/18 section-0 list (empty or not) and no 0x8E/0x8F/0x3B..0x3E kind to
  read. `AREA11/CA11.BIN` (294 bytes, unreferenced) is data: its first 204 bytes equal a table at file offset 0x30D58
  of PAL `AREA16/X016.BIN` (side-scrolling overlay; part of it also in X017/X018), not a top-down block.
- **8 bpp pictures** (`tools/beta980203.py tim8 /dev/shm/jpdemo/fs`): only `AREA08/D002.000`, 544x448,
  pixel-identical to PAL `AREA08/D002`/`D202` (the file differs only in its header/other chunks; 147 of 149 2 KB
  blocks are on PAL).
- **Blocks without a twin.** Every file (GAM unpacked) hashed whole and in 2 KB blocks against PAL, USA, JP, PrePre 8
  and beta: apart from the known AREA00/AREA08/CAnn/exe differences, the demo-only data are the SYSTEM pages
  `A00007/12/18/20/21/22` (menu, pigs, elder, Kemu, village people). No 32-byte run (with more than 4 distinct
  bytes) of the reload pages A06100 / A07600 / A07601 / A08000 / A08600 / A08601 occurs in any demo file.
- **Palettes** (`tools/clut_catalog.py`, `jpdemo` disc): 431 distinct rows, 40 demo-only (AREA00 `CLUTnn`
  HUD/village rows, 16-colour runs in `AREA03/CA03.BIN`). Female Stonetown Resident: best row 24.1, the shared
  AREA00/CLUT06 row; no demo-only row improves it. Scenery pages, best demo windows: A08000 83.6, A08600 67.5
  (`SYSTEM/A00009` = PAL `A00201`), A06100 95.7, all worse than the retail best (72.9 / 67.4 / 82.4).
- **Text** (`tools/jpdemo_text.py`): no town or Civilization lines; the only related words are the item names
  ストーンブーメラン / アイアンブーメラン.

## Making the demo a second matching target

**Status:** `config/slpm80165/slpm80165.yaml` and `config/slpm80165/x00.yaml` split the exe (text 0x80014C38..0x80069FC4)
and X00 (whole file at the estimated base 0x800E7698). `config/slpm80165/symbol_addrs_*.txt` are committed, generated
by `tools/xmatch.py --csv` + `tools/jpdemo_syms.py` (PAL names at demo addresses). `python3 tools/build_report.py
--version slpm80165` writes `build/slpm80165/report.json` (one unit per demo function), and the
`report-jpdemo` CI job uploads it as `SLPM_801.65_report` for decomp.dev, from the `GAME_FILES_URL_JP_DEMO` secret.

**C reuse (40.9 % of demo code, 1098 of 1832 functions; 39.6 % / 1034 before the sweep below):** the demo report compiles C for demo functions with the
PAL toolchain (`tools/ncheck.py` build: same compiler, `// FLAGS`, `// CC`, `// AS gnu`, `// HEADERS` per file) and
renames the object's function to the demo symbol, so objdiff matches it (relocations ignored; objects are not
linked). Sources, by priority:
- the shared `src/` tree (README, "How the versions share code"), compiled with `-DVERSION_SLPM_80165`: demo
  versions of PAL functions (changed constants, struct offsets, pad bits) are `#if defined(VERSION_SLPM_80165)`
  branches of the PAL file with a `// VERSION SLPM_80165 <prog> <demo addr> <size>` line (167 of them), and
  demo-only functions are `src/<name>_slpm80165.c` files guarded the same way (119); 2 more PAL files carry an `INCLUDE_ASM_V` demo branch. They were the files of the
  former `src/jpdemo/`, folded in by `tools/version_share.py fold slpm80165`.
  Common demo differences: d-pad bits shifted left by 8, `PlayerSetAnimIfChanged` written inline, a 20-byte item
  table instead of PAL's per-item pointers, `GetClut` inlined, object fields at +0xd0/+0xcf where PAL has
  +0xe4/+0xe3, no per-object +0x8c table, area switches cut down to the demo's areas 0-2. Small demo-only
  functions (stubs, setters, wrappers) are written from the demo code with a `// Demo-only ...` comment.
- `config/slpm80165/reuse.txt`: demo addr -> PAL module + address, written by `tools/jpdemo_reuse.py` (exact compare
  except relocated bits; needs `game/` and a previous demo build). The PAL file is found by address through
  `tools/units.py`, so PAL renames do not break it.
- fallback: the xmatch pairing in `symbol_addrs_*.txt` (same lookup by PAL address).
- Scratchpad: `config/slpm80165/symbol_addrs_manual.txt` tells splat never to symbolize 0x1F800000-0x1F8003FF (and
  the task table words at 0x801FD800), so the demo target always has literal addresses there. The C side keeps PAL
  names: build_report assembles every PAL scratchpad symbol (`config/slpm80165/spad.txt`, from `tools/jpdemo_reuse.py
  --spad` after a PAL `build_full.py`) and any `D_1F80xxxx` named in a demo file as a `.set` constant. This also
  exposes demo functions that only matched because a different scratchpad address hid behind a relocation.
- `tools/jpdemo_syms.py` drops tiny PAL library labels (`*_OBJ_<offset>`) that xmatch pairs with an epilogue inside
  demo game code: they cut the real function short (14 such rows).
- `config/slpm80165/symbol_addrs_manual.txt` also names 31 X00 functions that splat merged into the previous one
  (`jr ra`, then a new stack frame; reached only through pointer tables).

**Automatic sweep (`tools/demo_sweep.py`, 2026-11):** the reusable pass that raises the demo figure without hand
work, runnable from CI after `build_report.py --version slpm80165` (and for `beta980203` / `pcpx96084` when their
game files are present; it skips a version without a report). For every unmatched demo function with no demo C
yet it compiles, for the demo (`-DVERSION_SLPM_80165`, scratchpad names as constants, like the report), every
source that already matches the same function elsewhere: the PAL file of its symbol-table / `reuse.txt` pair (PAL
body and the branches of the other versions, SLPS_01144 first), the JP-only twins of that PAL function
(`src/[x0nn/]<name>_slps01144.c`), the PAL and JP functions of the same name, and, for the 848 splat `func_<addr>`
functions that have no pair at all, the PAL C functions and the JP functions with version C of the same size whose
masked words (`tools/xmatch.py`) are at least 90 % similar. Each body is tried with its file's flags and the common
variants (`-O2 -G0`, `-O2 -G0 -fno-strength-reduce`) and compared strictly (`tools/jpcheck.py` dump: relocation
addends ignored like objdiff, branch targets kept). A match is installed the way the versions share C: a symbol-table
pair (`PAL <module> <addr>`, named after the PAL function only when the pair is sure: symbol-table pair or an
identical masked body of 16 instructions or more, otherwise `func_<addr>_slpm80165`) when the PAL file as is
builds it, a `reuse.txt` line when the table already names it, a `// VERSION` + `// FLAGS.SLPM_80165` header when
only the flags differ, a `#if defined(VERSION_SLPM_80165)` branch of the PAL file when free, else a guarded
`src/<name>_slpm80165.c`. `--fix` adds the slow `tools/jp_autofix.py` edits (literals shifted by the immediates of
the diff, pad bits, a statement moved or deleted) on the best near candidate (size within 15 %, masked ratio 0.8 or
more) of each function still unmatched. The run is idempotent (installed functions are skipped) and `--report`
rebuilds the report; `--dry-run` only prints.

First run: 1069 -> 1098 functions, 40.33 % -> 40.87 % (`build_report` of the same tree before and after; the 39.6 %
above is the older stamp). 28 came from the exact pass (21 pairings of Psy-Q getters / GTE setters and
`clampCameraYOffsetToObjBounds`, 7 JP-twin files: `StartRCnt`, `_padStopCom`, `a00TornadoStep`, `updateNpcCharles`
and three JP-only helpers whose demo copies are byte for byte the JP ones), 1 from `--fix` (`func_80128D94`, the
JP `func_80131EF4` with two literals shifted; 12 minutes for its 48 candidates). The ceiling is structural, not a
tool gap: of the 690 unmatched demo functions, 606 have no same-size function in the whole C pool at even 50 %
masked similarity and only 54 have a near-size one at 80 % or more (`--show` lists the residual diff line count),
so the rest is edited early code (the 70 % / 79 % instruction-level difference measured above) that needs hand
matching per function, as for the retail JP build.

**Fuzzy twins + fix pass (2026-11-09).** `tools/demo_pair.py` ranks, for every unmatched function of a demo build, the
closest functions that already have matching C in ANY version (PAL, JP retail, USA and the three demo builds
themselves; about 18000 functions), by instruction skeleton (opcodes only: registers, immediates and relocated
addresses ignored), callee overlap through the symbol-table pairs and call-graph neighbours (the k-th unknown callee
of a version function paired with PAL is proposed as the twin of the PAL function's k-th callee). Any size; a
hashed opcode 4-gram prefilter (numpy) keeps 60 per function before difflib; 45 s for a version; output
`build/<ver>/pairs.json`. `tools/demo_sweep.py` now compiles the best 10 twin bodies (each as its own version
builds it: its `#if defined(VERSION_<SERIAL>)` branch or the PAL body) next to the old candidates, and `--fix`
starts from the closest compiled candidate of each function (at most `--fix-max` 40 changed lines, smallest
first, `--budget` minutes per version) with two new edits before the `tools/jp_autofix.py` ones: scratchpad
renames (a PAL scratchpad name the build keeps elsewhere becomes `#define NAME D_1F80xxxx`, read off the diff
offsets) and, for PrePre 8, `// ASPSX 2.34` (docs/PREPRE8.md). CI (`jp-sweep.yml`) runs it with `--fix --budget 20`.
Run of 2026-11-09 (exact + `--fix --budget 25`, reports before -> after, objdiff):

| Build | functions | code |
|---|---|---|
| SLPM-80165 | 1153 -> 1160 of 1832 | 42.60 % -> 42.84 % |
| BETA_980203 | 1503 -> 1602 of 1915 | 60.72 % -> 69.56 % |
| PCPX-96084 | 543 -> 617 of 1424 | 17.89 % -> 20.29 % |

Three installs that `jpcheck --strict --norm` called IDENTICAL scored 90-99.9 % in objdiff and were taken out: the
checker's norm form writes every store through `$at` as `X(at)`, so a scratchpad store at another offset passes
(open: compare `$at` stores of `lui at,0x1f80` literally). What blocks the rest, by the closest compiled candidate
(diff of its disassembly against the target): SLPM-80165 is mostly far from every twin (early code rewritten);
the beta is mostly structural (instructions added or removed: an inserted call, a removed check), the cheapest
hand targets; PrePre 8 has 304 functions with no twin at all (debug/test code, unrolled renderers) and 221 far
ones. The 12-byte Psy-Q syscall stubs of SLPM-80165 (`OpenEvent`, `CloseTh`, ...) miss only because splat gives
them 16 bytes (the alignment `nop` after `jr`); a symbol size fix would take them.

**Hand pass, round 2 (2026-11-10): 1272 -> 1313 of 1719 functions, 48.06 % -> 50.11 % of code.** From the closest
twins of `build/slpm80165/pairs.json`, each draft compiled against the demo target and fixed by hand. Demo-wide
differences that came up again and again (worth trying first on the next function):
- small player helpers written inline in the x00 player steps: `PlayerSetAnimIfChanged` (demo form in
  `src/func_800EC020_slpm80165.c`), `setFacingFromPad` with the d-pad bits 0x2000 / 0x8000 (PAL 0x20 / 0x80) and
  `playerSetAngleFromSlope`; as `static __inline__` helpers they give the same code as the hand-inlined body;
- `playerSetFallLean` is not called: the demo clears +0x8C instead; fixed buttons (0x40, 0x80) where PAL reads
  `BUTTON_MASK_*`; the object byte +0xE3 is +0xCF;
- the CD file table has 24-byte entries (names included), the second display environment is at +0x2090
  (PAL +0xD10), and the work vector / camera matrix of the actor projection live in the scratchpad (0x1F800060 /
  0x1F8000C0);
- the four voice-allocating sound helpers search all 24 voices, store the sound id on the way and queue the
  commands even with no free voice; the collision slope tests take a `short` first argument and use a signed
  `% 8`;
- splat cut the tail of a few library functions into separate "functions" (`SquareRoot0` +8, `ApplyMatrixLV` +12):
  the PAL size in `symbol_addrs_slpm80165.txt` puts them back; a jal target in the middle of
  `a00Variant0EventSpawn` (0x8012A618) split that one until round 3 gave it its size in `symbol_addrs_x00.txt`;
- round 2 left three addresses (0x8003A770, 0x8004B50C, 0x80116F94) with two `// VERSION SLPM_80165` lines in
  `src/`; round 3 removed the duplicates (one VERSION line per address again).

**Hand pass, round 3 (2026-11-10): 1315 -> 1360 of 1717 functions, 50.29 % -> 54.20 % of code** (after the
round's first commit, a6df66c, which had taken it from 50.11 %; the function count drops by one:
`func_8010FD38`, the demo's play-time limit, got its real size; splat had cut it at 0x8010FE8C).
From `tools/demo_pair.py` twins again, this time with the PrePre 8 and beta builds in the pool (their game dirs
must be linked into a worktree, or `demo_pair.py` skips them and the closest twins of the front-end code are
missing). New demo-wide differences, worth trying first:
- **PrePre 8 twins of the front end are often the demo function with other offsets**: text windows with the PAL
  CLUT position (0x170, 0x1F0; PrePre 8 0x3F0, 0x1F8), the task pointer and its flags in the scratchpad
  (0x1F8001D4, 0x1F8001CC/CD/D0/D3; PrePre 8 RAM), object bytes +0xA5 (PrePre 8 +0xA1) and +0xD4/+0xD6 (PrePre 8
  +0xBC/+0xBE);
- `playerMoveByAngleSpeed(o)` is `func_800202FC(o, o->wb6, o->wb2)` + the drift (`D_8009A7F4`/`F6` << 8 added to
  h / y) + `func_800203CC(o)` (stick to ground) in the demo, written out in the step;
- no slope tilt (`SLOPE_TILT_TABLE[o->wb0]` into +0x8C) when a step ends, no `startPadVibration`, no
  `GVARS.i34 = 0`, fixed button bits 0x40 / 0x80 and d-pad bits 0x8000 / 0x2000 / 0xF000 (all three already in
  round 2, they keep coming back); the player fields from +0xD0 are 8 below the beta's (link +0xD0, PAL +0xE4);
- the screen test of the flat-shaded mesh renderers (`func_8002388C`, `func_80023B08`) comes after the colour, with an
  OT window of 0x814 bytes (JP 0xCA0);
- **cross-jumping**: where retail has one call with its argument load left in each path (`li a0,10` in a `j` delay
  slot and again before the `jal`), the source called the function once per path (`func_80026854`,
  `func_800E9A28`, `func_800ED5FC`); an `if (x == 0) o->f = A; else o->f = B;` gives the single store with the
  value in `v0` where a ternary picks another register;
- the strict + norm compare of `demo_sweep._check_file` drops the targets of `j` instructions that carry a
  relocation: a goto-shaped mismatch (a jump to the end instead of to a shared tail) shows only in the objdiff report
  (99.9 %). A diff of the raw `objdump -d` instruction text (relocation addends aside) catches it before the
  report;
- globals that splat names field by field (`D_800A2A1C`, `D_800A2A46`, ...) need one extern per field only for the
  relocation names; objdiff runs with `functionRelocDiffs: none`, so instruction identity is what counts.

Open near misses (drafted, not installed): `func_8001A9EC` (gameplay frame, 16 lines: the scheduler moves
the inventory-flag store ahead of the task pointer load), `func_800182E0` (text grid put, register allocation), the
camera clamp pair `func_8002796C` / `func_8002A02C` (register allocation), and `func_8010FED8_slpm80165` (still cut
in three by splat at 0x801100D0 and 0x801102E4; needs its size, 0x470, and C for the whole function).

- **Plumbing (1-2 days):** add `config/slpm80165/{slpm80165,x00,dspsub,dspsub3,optsub00}.yaml`. The exe is a normal
  PS-X EXE at 0x80010000; the overlays use the bases above, to be confirmed in an emulator. Generate
  `symbol_addrs_*` from `tools/xmatch.py --csv` (PAL names at demo addresses). Then extend `tools/build_full.py` and
  `build_report.py` with a version switch, as planned for retail JP in `docs/JP.md`.
- **Free reuse:** about 616 exe functions and 156 X00 functions (about 180 KB, roughly 23% of demo code) already
  have PAL C. They can be shared by generated wrappers, like `tools/areas.py twins`, when the masked bytes are
  identical. Extern addresses differ, so this needs symbol-based linking instead of hardcoded addresses.
- **Remaining:** about 70% of the exe and 79% of X00 differ at instruction level from PAL. Most are edited versions
  of PAL functions (60% window overlap), so PAL C is a good starting point. Rough estimate: a few weeks of focused
  work per 100 KB of code, with automated diff-and-adapt (`tools/automatch.py`, `permute.py`) doing most of the
  easy cases. The debug menu and the test code are new.
- **Optional:** the unreferenced CAnn and INFO overlays could be split for research, but they are not needed for a
  matching rebuild of the demo, because the disc does not load them.
- **Recommendation:** the demo is a cheaper first non-PAL target than retail JP, because it is smaller and shares
  the SDK. Its main payoff is the debug menu and the cut-content overlays. Retail JP (`docs/JP.md`) would still
  need its own config.
