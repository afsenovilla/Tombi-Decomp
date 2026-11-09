# USA release (Tomba!, SCUS-94236)

> **100 % matched on 2026-11-09**: every function of the retail disc, SYSTEM overlays and Psy-Q library included.

## Matching target

`--version scus94236` (alias `usa`, macro `VERSION_SCUS_94236`), `config/scus94236/`, decomp.dev version
`SCUS_942.36`. Same layout as the Japanese retail (docs/JP.md):

- boot exe `SCUS_942.36` (sha1 `81cbc79f0230aeb4252e058039f47ac95a777f5a`, entry 0x8006B58C, 0x88000 bytes at
  0x80010000); text 0x800163B0 (`main`) .. 0x80076E38 (end of `SpuGetAllKeysStatus`);
- `SYSTEM/X00.BIN` .. `X19.BIN` (no X07, X12, X15), load address 0x800E7388 (jal targets on X00 prologues; PAL
  0x800E8028, JP 0x800E9AF8). Every `X<nn>.BIN` starts with the same 0x2DE6C bytes as `X00.BIN` (`USA_SHARED` in
  `tools/versions.py`); `config/scus94236/x<nn>.yaml` splits only the area part (vram 0x801151F4), so the shared code
  is counted once, in `x00`.
- SYSTEM code overlays `DSPSUB.BIN`, `DSPSUB2`, `DSPSUB3`, `GOVER`, `INFO`, `INFO2`, `INFO3`, `OPTSUB00` (programs
  `dspsub` .. `optsub00`, `USA_SYSOVL` in `tools/versions.py`, paired with PAL `d0spsub` .. `o0ptsub0` by `pal_prog`):
  raw blobs at the same base 0x800E7388 (header pointers = PAL - 0xCA0). `config/scus94236/<prog>.yaml` splits header
  (bin), code, trailing data (bin); code ranges DSPSUB 0x794-0x962C, DSPSUB2 0x20C-0x4E0C (the memory-card low-level
  functions come first in USA), DSPSUB3 0x6EC-0x6820, GOVER 0x1C-0xC6C, INFO 0x1EC-0x6730, INFO2 0xAE8-0x16848, INFO3
  0x454-0x6C84, OPTSUB00 0x38-0x1FC0. Symbols: `xmatch.py` on the SYSTEM files (it now also reads the PAL SYSTEM
  symbols; run `tools/gen_symbols.py` first) + `jpdemo_syms.py`, which now rewrites only the programs in the csv.

Symbols: PAL names at USA addresses (`tools/xmatch.py` + `tools/jpdemo_syms.py --version scus94236`), the task table
hints in `symbol_addrs_manual.txt` (copied from JP), `data_units.txt` (`tools/jp_data_units.py --version scus94236`:
entry/jump tables splat took for functions) and `reuse.txt` (`tools/jpdemo_reuse.py --version scus94236`). The local
game dir is `game_usa/fs/` (`bash tools/disc_fetch.sh USA`; git-ignored). CI: job `report-usa` of `report.yml`
(secret `GAME_FILES_URL_USA`, artifact `SCUS_942.36_report`) and the USA disc in `jp-sweep.yml`.

## How to reproduce

```
bash tools/disc_fetch.sh USA
python3 tools/gen_symbols.py
python3 -I tools/xmatch.py game_usa/fs/SCUS_942.36 game_usa/fs/SYSTEM/X*.BIN --csv build/scus94236/xmatch.csv
python3 -I tools/jpdemo_syms.py build/scus94236/xmatch.csv --version scus94236
python3 tools/build_report.py --version scus94236 --objdiff /root/.cargo/bin/objdiff-cli
python3 tools/jp_data_units.py --version scus94236 --write
python3 tools/jpdemo_reuse.py --version scus94236
python3 tools/build_report.py --version scus94236 --objdiff /root/.cargo/bin/objdiff-cli --no-split
python3 tools/demo_sweep.py --version scus94236 --report [--fix]
# SYSTEM overlays only (keeps the other symbol files):
python3 -I tools/xmatch.py game_usa/fs/SYSTEM/{DSPSUB,DSPSUB2,DSPSUB3,GOVER,INFO,INFO2,INFO3,OPTSUB00}.BIN --csv build/scus94236/xmatch_sys.csv
python3 -I tools/jpdemo_syms.py build/scus94236/xmatch_sys.csv --version scus94236
```

Data measure (90.88 %): docs/JP.md "Data measure" (same pipeline, `src/data/scus94236/`).

## First figures (2026-11-08)

USA is much closer to PAL than JP is (xmatch: 61 % of the exe bytes and 85-91 % of every overlay pair with PAL by
masked hash).

| Step | Code | Functions |
|---|---:|---:|
| first report, shared tree as is | 77.35 % | 2855 / 3571 |
| tables taken out of the code units (`data_units.txt`) | 92.17 % | 2855 / 3209 |
| `reuse.txt` | 93.09 % | 2879 / 3209 |
| `demo_sweep.py` (131 installed: 104 pairings, 13 branches, 14 USA-only files) | 93.95 % | 3010 / 3209 |
| `demo_sweep.py --fix` (39 more, PAL C edited by the tools/jp_autofix.py edits) | 95.66 % | 3049 / 3209 |
| second `demo_sweep.py --fix` after the JP Psy-Q asm files (24 more) | 95.79 % | 3071 / 3209 |
| psx_tomba ports (35) + hand-written Psy-Q asm (64 functions in 50 files) | 97.47 % | 3152 / 3218 |
| X13 updateSceneSequence (PAL file, `FPS` 60 for NTSC) | 97.65 % | 3154 / 3218 |
| message box 8002FA24 (psx_tomba) + 30 near-100 % functions (15 reuse the JP branch, 15 USA files: NTSC screen
  height = PAL - 8, pad struct one byte earlier, scratchpad bytes that are RAM globals in USA) | 99.12 % | 3185 / 3218 |
| library cleanup: one file per BIOS/libcard unit, FGO_01_OBJ_160, DecDCTvlc(Size), libcard patch; three PsyqStamp /
  vlc_size data words moved to data_units.txt | 99.22 % | 3199 / 3215 |
| the last 8 game functions (PAL/JP twins with USA names; drawObjectSpriteRotScaled with the 0xe0 screen
  height) | 99.77 % | 3207 / 3215 |
| libsnd `_SsContDataEntry` (0x430) and `_SsVmSetVol` (0x584) from psx_tomba, one function each in all versions | 99.94 % | 3209 / 3213 |

What is left is mostly in the boot exe (83 %): functions that differ from PAL by more than the automatic edits.

## SYSTEM overlays (2026-11-08)

| Step | Code | Functions |
|---|---:|---:|
| before (exe + X overlays only) | 99.94 % | 3209 / 3213 |
| SYSTEM overlays added, PAL pairing from `symbol_addrs_<prog>.txt` | 96.93 % | 3589 / 3681 |
| `reuse.txt` lines of the SYSTEM programs (`jpdemo_reuse.py`, only those lines appended) | 97.10 % | 3594 / 3681 |
| `demo_sweep.py` (28 pairings) | 97.11 % | 3622 / 3681 |
| `demo_sweep.py --fix` (7 more: 6 branches in PAL files, 1 USA file) | 97.52 % | 3629 / 3681 |

Per overlay after the sweep: GOVER 100 %, INFO3 98.0 %, INFO2 96.5 %, INFO 87.8 %, DSPSUB3 66.3 %, DSPSUB 65.6 %,
OPTSUB00 64.0 %, DSPSUB2 44.9 %. The 52 functions left are mostly the large sub-screen / memory-card / title drawing
functions, where the English text layout differs from PAL by more than the jp_autofix edits; no PAL pair was found
for many of them (splat `func_`), so they need hand work (PAL C as the starting point).

| Hand passes (2026-11-08/09) | Code | Functions |
|---|---:|---:|
| SYSTEM hand pass (DSPSUB*, OPTSUB00, INFO*, from the PAL C) | 99.45 % | 3673 / 3681 |
| dspsub2 800E954C (PAL 800E999C port), both overheadBuildMapStrip (JP trick), libapi psyq_8005ba74/bc10 | 99.81 % | 3678 / 3681 |
| libsn `__main` / `__do_global_dtors` (hand-written Sony asm, from the JP files) | 99.83 % | 3680 / 3681 |
| dspsub 800EEE84 (twin of PAL D0SPSUB 800EFD6C) | **100 %** | **3681 / 3681** |
