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
