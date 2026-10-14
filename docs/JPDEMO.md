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

## Making the demo a second matching target

**Status:** `config/jpdemo/slpm80165.yaml` and `config/jpdemo/x00.yaml` split the exe (text 0x80014C38..0x80069FC4)
and X00 (whole file at the estimated base 0x800E7698). `config/jpdemo/symbol_addrs_*.txt` are committed, generated
by `tools/xmatch.py --csv` + `tools/jpdemo_syms.py` (PAL names at demo addresses). `python3 tools/build_report.py
--version jpdemo` writes `build/jpdemo/report.json` (one unit per demo function, no C yet, so 0 %), and the
`report-jpdemo` CI job uploads it as `SLPM_801.65_report` for decomp.dev, from the `GAME_FILES_URL_JP_DEMO` secret.

- **Plumbing (1-2 days):** add `config/jpdemo/{slpm80165,x00,dspsub,dspsub3,optsub00}.yaml`. The exe is a normal
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
