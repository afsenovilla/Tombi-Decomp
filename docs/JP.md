# Japanese version (Ore! Tomba)

The Japanese release is a second decomp.dev version built from the shared `src/` tree (see "How the versions
share code" in the README): `--version slps01144` (alias `jp`), macro `VERSION_SLPS_01144`.

## Inputs

- Your own disc image, provided to CI through the `GAME_FILES_URL_JP` secret (raw `.bin`, 2352-byte sectors) and
  locally in `game_jp/` (ignored by git). Never commit the image or files extracted from it.
- `.github/workflows/report.yml`, job `report-jp`: fetches and extracts the image, names the artifact after the boot
  executable in `SYSTEM.CNF` (`<serial>_report`), and skips the report until `config/slps01144/` exists.

## Steps

1. Extract the disc (`tools/isoext.py`), record the SHA-1 of every executable and overlay, map the file layout
   (the Japanese release has one language, so no `EXE/MAIN0..8` per language) in `notes/jp/`.
2. Split the executables with splat with `config/slps01144/` (addresses differ from PAL).
3. Auto-match: hash every Japanese function with relocation fields masked and compare against the matched PAL
   functions; identical ones reuse the PAL source (generated, like `tools/areas.py twins`).
4. Decompile what differs (text/font system with kana/kanji, file loading, older library revisions) as
   `#if defined(VERSION_SLPS_01144)` branches of the shared files (`tools/version_share.py fold`).
5. `tools/build_report.py --version slps01144` writes `build/slps01144/report.json`; `tools/build_full.py` gains a JP target
   that must rebuild the Japanese executables byte-identically.

## Out of scope for now

Adapting the text and graphics editors to Japanese text.

## Status

Retail disc: boot `SLPS_011.44` (sha1 `07d577698ad31ec1a69b9d7b80a7c3f90b650888`), entry 0x80065D80, 0x8E000 bytes at
0x80010000. Same file layout as PAL with two-digit names: `SYSTEM/X00.BIN`..`X19.BIN` (copies in `AREAnn/`; AREA07
uses X01 and AREA12 uses X04, AREA19 also ships X02), plus `INFO*.BIN`, `DSPSUB*.BIN`, `OPTSUB00.BIN`.

- **Overlays** load at 0x800E9AF8 (PAL 0x800E8028), from jal targets on prologues. Every `X<nn>.BIN` starts with
  the same 0x2B680 bytes as `X00.BIN` (the shared code, PAL X000's role); the area part follows at 0x2B680 (vram
  0x80115178) with its own ID word (X01 = 9) and entry table. `config/slps01144/x<nn>.yaml` splits only that area part
  (the prefix is a `bin` segment), so shared code is counted once, in `x00`.
- **Exe text** 0x80015D1C (`main`) .. 0x8007142C (end of `SpuGetAllKeysStatus`), like `config/main0.yaml`.
- `config/slps01144/symbol_addrs_*.txt`: PAL names at JP addresses, from `tools/xmatch.py --csv` +
  `tools/jpdemo_syms.py --version slps01144`; functions without a PAL name are `func_<addr>_slps01144`. Masked match
  finds 1059 exe functions (40 % of the bytes) and 348 in X00.
- `config/slps01144/reuse.txt` (`tools/jpdemo_reuse.py --version slps01144`): JP functions that PAL C builds as is.
- Differing functions: `#if defined(VERSION_SLPS_01144)` branches in the PAL files, with a
  `// VERSION SLPS_01144 <prog> <addr> <size>` line (77 with JP C, 86 with `INCLUDE_ASM_V` for the paired functions
  not matched yet), and 4 JP-only files `src/[x0nn/]<name>_slps01144.c`. They were the 81 files of the former
  `src/jp/`, folded in by `tools/version_share.py fold`; `tools/version_share.py asm slps01144` writes the
  INCLUDE_ASM branches from a report.
- `python3 tools/build_report.py --version slps01144` writes `build/slps01144/report.json`; CI job `report-jp`
  uploads it as `SLPS_011.44_report` from the `GAME_FILES_URL_JP` secret.

**Progress: 50.5 % of code (785 KB of 1556 KB, 2076 of 3542 functions)**, mostly PAL C reused as is, the rest from
the JP branches (PAL C with one to four literal constants changed and verified byte for byte, e.g. player struct
offsets 0xE4 -> 0xD4, object stride 160 -> 152, sound pitch constants). Common JP differences seen so far: player
struct fields after about 0xD0 are 16 bytes earlier, some object structs are 8 bytes smaller, and most remaining
functions differ by whole instructions (edited code), so the next step is manual decompilation of the near-misses
(`fuzzy_match_percent` 90 % or more in the report).

Not linked yet: like the demo, the JP objects are compared by objdiff only; `tools/build_full.py` has no JP target.

