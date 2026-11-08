# USA release (Tomba!, SCUS-94236)

## Matching target

`--version scus94236` (alias `usa`, macro `VERSION_SCUS_94236`), `config/scus94236/`, decomp.dev version
`SCUS_942.36`. Same layout as the Japanese retail (docs/JP.md):

- boot exe `SCUS_942.36` (sha1 `81cbc79f0230aeb4252e058039f47ac95a777f5a`, entry 0x8006B58C, 0x88000 bytes at
  0x80010000); text 0x800163B0 (`main`) .. 0x80076E38 (end of `SpuGetAllKeysStatus`);
- `SYSTEM/X00.BIN` .. `X19.BIN` (no X07, X12, X15), load address 0x800E7388 (jal targets on X00 prologues; PAL
  0x800E8028, JP 0x800E9AF8). Every `X<nn>.BIN` starts with the same 0x2DE6C bytes as `X00.BIN` (`USA_SHARED` in
  `tools/versions.py`); `config/scus94236/x<nn>.yaml` splits only the area part (vram 0x801151F4), so the shared code
  is counted once, in `x00`.
- The SYSTEM overlays (`DSPSUB*`, `INFO*`, `OPTSUB00`, `GOVER`) are not split yet.

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
```

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

What is left is mostly in the boot exe (83 %): functions that differ from PAL by more than the automatic edits.
