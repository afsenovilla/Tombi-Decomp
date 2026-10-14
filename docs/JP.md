# Japanese version (Ore! Tomba)

Plan for adding the Japanese release to this repository as a second decomp.dev version.

## Inputs

- Your own disc image, provided to CI through the `GAME_FILES_URL_JP` secret (raw `.bin`, 2352-byte sectors) and
  locally in `game_jp/` (ignored by git). Never commit the image or files extracted from it.
- `.github/workflows/report.yml`, job `report-jp`: fetches and extracts the image, names the artifact after the boot
  executable in `SYSTEM.CNF` (`<serial>_report`), and skips the report until `config/jp/` exists.

## Steps

1. Extract the disc (`tools/isoext.py`), record the SHA-1 of every executable and overlay, map the file layout
   (the Japanese release has one language, so no `EXE/MAIN0..8` per language) in `notes/jp/`.
2. Split the executables with splat into `config/jp/` (addresses differ from PAL).
3. Auto-match: hash every Japanese function with relocation fields masked and compare against the matched PAL
   functions; identical ones reuse the PAL source (generated, like `tools/areas.py twins`).
4. Decompile what differs (text/font system with kana/kanji, file loading, older library revisions) into `src/jp/`.
5. `tools/build_report.py --version jp` writes `build/jp/report.json`; `tools/build_full.py` gains a JP target
   that must rebuild the Japanese executables byte-identically.

## Out of scope for now

Adapting the text and graphics editors to Japanese text.

## Status

Retail disc: boot `SLPS_011.44` (sha1 `07d577698ad31ec1a69b9d7b80a7c3f90b650888`), entry 0x80065D80, 0x8E000 bytes at
0x80010000. Same file layout as PAL with two-digit names: `SYSTEM/X00.BIN`..`X19.BIN` (copies in `AREAnn/`; AREA07
uses X01 and AREA12 uses X04, AREA19 also ships X02), plus `INFO*.BIN`, `DSPSUB*.BIN`, `OPTSUB00.BIN`.

- **Overlays** load at 0x800E9AF8 (PAL 0x800E8028), from jal targets on prologues. Every `X<nn>.BIN` starts with
  the same 0x2B680 bytes as `X00.BIN` (the shared code, PAL X000's role); the area part follows at 0x2B680 (vram
  0x80115178) with its own ID word (X01 = 9) and entry table. `config/jp/x<nn>.yaml` splits only that area part
  (the prefix is a `bin` segment), so shared code is counted once, in `x00`.
- **Exe text** 0x80015D1C (`main`) .. 0x8007142C (end of `SpuGetAllKeysStatus`), like `config/main0.yaml`.
- `config/jp/symbol_addrs_*.txt`: PAL names at JP addresses, from `tools/xmatch.py --csv` +
  `tools/jpdemo_syms.py --version jp`. Masked match finds 1059 exe functions (40 % of the bytes) and 348 in X00.
- `config/jp/reuse.txt` (`tools/jpdemo_reuse.py --version jp`) and `src/jp/<prog>/*.c` (header
  `// JP <prog> <addr> <size>`, no FUNC/MATCHING marker) work exactly like the demo pipeline (docs/JPDEMO.md); the
  tools take `--version jp` (programs and bases in `tools/versions.py`; `jpdemo` stays the default).
- `python3 tools/build_report.py --version jp` writes `build/jp/report.json`; CI job `report-jp` uploads it as
  `SLPS_011.44_report` from the `GAME_FILES_URL_JP` secret.

**Progress: 52.6 % of code (819 KB of 1556 KB, 2130 of 3542 functions)**: 49.8 % from PAL C reused as is, the rest
from 81 generated variants in `src/jp/` (PAL C with one to four literal constants changed and verified byte for
byte, e.g. player struct offsets 0xE4 -> 0xD4, object stride 160 -> 152, sound pitch constants). Common JP
differences seen so far: player struct fields after about 0xD0 are 16 bytes earlier, some object structs are 8
bytes smaller, and most remaining functions differ by whole instructions (edited code), so the next step is manual
decompilation of the near-misses (`fuzzy_match_percent` 90 % or more in the report).

Not linked yet: like the demo, the JP objects are compared by objdiff only; `tools/build_full.py` has no JP target.

