# Assets: evaluation of hansbonini/tombatools on the PAL disc

First step of the assets stage (PROGRESS.md, plan step 6). Tool: https://github.com/hansbonini/tombatools
(Go, MIT, commit 003f6bd 2025-08-10), cloned **outside** the repo to `/opt/psyq/tombatools` and built with
`/usr/local/go/bin/go build` (go 1.24; only deps cobra + yaml.v3; no `os/exec` or `net` use; its unit tests pass).
Outputs went to `/tmp/tombatools_out` (nothing from the tool or the disc is in the repo).

**Update (first assets block): WFM is now lossless.** With our fork (`/opt/psyq/tombatools-pal`, branch `pal`, commit `5e897a4`,
based on upstream 003f6bd; not vendored here) `wfm decode` -> `wfm encode` is **byte-identical for 369/369** WFM files and the
Spanish files decode with accents. See "WFM lossless workflow" below; the sections after it are the original evaluation of upstream.

## WFM lossless workflow (fork `tombatools-pal`)
Fork: `/opt/psyq/tombatools-pal` (git branch `pal`, commit `5e897a4`, on top of upstream `003f6bd`), MIT like upstream.
It is a local clone, not published anywhere yet: **it should be offered upstream (hansbonini/tombatools) as a PR** (the changes are
backwards compatible: the old re-authoring encoder still runs for YAML files without the new fields).

Results (disc = PAL Spanish, `game/disc/fs`):
- **Round trip: 369/369 byte-identical** (`tools/wfm.py --check`; also with `--fonts es` only). Before: 0/369.
- **Unknown glyphs in the 41 Spanish `*S.WFM` files** (19 distinct files): `[80xx]` tokens **9,069 -> 22** (distinct files: 5,627 -> 18);
  glyphs without a character: 0 of 2,476 (19 files). The 22 left are one phrase, "Zumo de plátano": the 8 px font has two different
  bitmaps for `Z`, and text cannot say which one, so the second stays `[80xx]` (still lossless).
- Other languages still show `[80xx]` for their own special letters (B D F G H I P W): only `es` was built. The round trip does not depend on it.
- `[C0xx]` control codes stay as raw tokens (1,436 in the S files; C050, C056, C053, C06B... are the most frequent, meaning still unknown);
  their bytes are preserved. Event titles in CFNT999S have no space glyphs in the data (`Pulseradelabuelo`): the original file has no space word,
  so nothing is lost, there is simply nothing to recover.

What the fork changes (about 600 lines, `pkg/lossless.go` is new):
- `decode` also writes into `dialogues.yaml`: `header_padding`, `reserved` (the 128-byte block, hex), `trailing` (bytes after the last dialogue),
  and `glyphs:` (the whole glyph table in file order: clut/height/width/handakuten, 4bpp bitmap as hex, and the recognised `char`).
- `encode` uses those (if present) and rebuilds header, glyph table + pointers, dialogue pointers and the dialogue stream exactly (no padding invented).
  Unknown characters are an error instead of being silently dropped.
- Text rules for losslessness: `[80xx]` glyph index and `[C0xx]` (any `[XXXX]`) are raw words; a glyph is written as a character only if the encoder
  resolves that character (for the dialogue `font_height`/`font_clut`) back to the same glyph index; `\n` = FFFD, `\n\n` = FFFB, and `[FFFD]`/`[FFFB]` when
  that would be ambiguous; `terminator: 0` = dialogue without terminator (last one of a few files); text before a `box` keeps its order; text that YAML would
  mangle (leading newlines) is written double-quoted.
- `--fonts a,b` (names under `fonts/` or paths; env `TOMBATOOLS_FONTS`) selects the reference sets, no more hard-coded `fonts/br` (legacy encode
  uses the first dir). Default: every set under `fonts/`. `0059_2.png` style names (suffix after `_`) allow two bitmaps for one character.
- `fonts/es` (239 PNGs, 8/16/24 px): every glyph bitmap of the Spanish files named by code point, derived from the PAL files (glyph PNGs dumped,
  the missing ones identified from the ASCII art and from the surrounding Spanish text). Added over eu/us: á é í ó ú Á É Í Ó Ú ñ ¿ ¡ à ª º Q q K Z ( ) / - "
  a middle dot (U+00B7, the `...` dot) and a down-arrow icon (U+2B07). No `ü` occurs in the Spanish text.
- Unit tests kept passing (`go test ./...`); new: `pkg/lossless_test.go` (synthetic WFM round trips with and without font mapping, text token rules).

Using it from this repo (`tools/wfm.py`, Python 3, no Go source in the repo; `TOMBATOOLS` = path of the fork, default `/opt/psyq/tombatools-pal`;
the binary is built with `go build` on first use or when the sources are newer):
```
tools/wfm.py --check                       # round trip all game/disc/fs WFM, prints "identical: 369/369" (exit 1 if not)
tools/wfm.py --check game/disc/fs/AREA05   # or one dir / one file
tools/wfm.py decode game/disc/fs/AREA05/MFNT050S.WFM -o build/wfm   # -> build/wfm/AREA05/MFNT050S/{dialogues.yaml,glyphs/}
tools/wfm.py decode                        # the whole tree into build/wfm (do not commit it)
tools/wfm.py encode build/wfm/AREA05/MFNT050S/dialogues.yaml out.WFM
tools/wfm.py decode ... --fonts es         # only the Spanish reference set (default: all sets)
```

Decoded Spanish samples (`AREA05/MFNT050S.WFM`; before: `RAT[8072]N`, `rat[8076]n`, `[8053]Guardar`, `S[805C]`):
```yaml
- text: |+
    RATÓN DESAPARECIDO
    Ha desaparecido un ratón.
    Ha desaparecido un ratoncito.[C052]⧗
```
```yaml
- text: "Ciudad Baco[C052]⧗\n\n[PROMPT]¿Guardar DATOS?  \nSí⮊✕ No⮊○⧗\n\n"
```

Not done: other languages' glyphs, meaning of the `[C0xx]` codes, GAM bit-exact packing (below), anything outside WFM.

# Original evaluation of upstream (before the fork)

## What the tool covers
| Command | Purpose |
|---|---|
| `wfm decode <f.WFM> <dir>` | glyph PNGs + `dialogues.yaml` (text, control codes, boxes) |
| `wfm encode <yaml> <out.WFM>` | rebuild a WFM from YAML |
| `gam unpack/pack` | the game's LZ ("GAM" header: magic, size) |
| `cd dump <bin> <dir>` | ISO9660 dump of a raw .bin (also reads FLA ids/MSF) |
| `fla recalc <orig.bin> <mod.bin>` | rewrite the file-link-address table after resizing files (not tested) |

Gotchas: `wfm decode`/`encode` read a **relative** `fonts/` directory of reference glyph PNGs (shipped in the repo as
`fonts/{br,eu,us}/{8,16,24}/...`), so they must run with cwd = the tool dir. The encoder hard-codes `fonts/br`.
Decoded text is obtained by hashing each glyph bitmap against that library; glyphs without a match come out as `[80xx]`.

## Commands used
```
cd /opt/psyq/tombatools   # needed for fonts/
./tombatools wfm decode $FS/AREA00/MFNT000S.WFM /tmp/tombatools_out/wfm/AREA00_MFNT000S.WFM
./tombatools wfm encode <dir>/dialogues.yaml out.WFM ; cmp $FS/AREA00/MFNT000S.WFM out.WFM
./tombatools gam unpack $FS/AREA00/A001.GAM a.UNGAM ; ./tombatools gam pack a.UNGAM a.rt ; cmp ...
./tombatools cd dump game/disc/Tombi.bin /tmp/tombatools_out/cd
```

## Results
**WFM decode: 369/369 OK** (all `CFNT999?` and `MFNT0nn?` of AREA00..19, 9 language suffixes B D F G H I P S W;
we are `S` = Spanish). No crashes, every file yields glyphs and a dialogue count equal to the header
(e.g. CFNT999S: 71 glyphs, 136 dialogues; MFNT050S: 318 dialogues). Structure (WFM3 header, glyph table, dialogue
pointer table, 128-byte reserved block = "special" dialogue ids) is understood correctly.

**Text quality (Spanish): partial.** Plain ASCII letters/digits decode. Accented letters, `ñ`, `¿`, `¡` and many
punctuation glyphs are not in the reference fonts, so they stay as `[80xx]` (about 9,000 such tokens across the `S`
files; the `us`-like B/H files have ~500-1000, the other languages similar to S). Since the code is just a glyph index
of that file, each file needs its own glyph-to-character table. Examples seen: `[8072]`=Ó, `[8076]`=ó, `[800D]`=Í,
`[805C]`=í. Spaces in event titles (CFNT999S) are not recovered (`Pulseradelabuelo`). No `¿`/`¡` appears in any decoded text.
Control codes appear as `[C0xx]` (e.g. C050, C04C, C056 are the most frequent) plus structured items `box`, `f6`,
`tail`, `color`, `pause`, `fff2`, `[PROMPT]`, `⧗` (wait for input); a few `[C0xx]` are not known to the encoder.

Sample (`AREA05/MFNT050S.WFM`, dialogue, 16 px):
```yaml
- text: |+
    RAT[8072]N DESAPARECIDO
    Ha desaparecido un rat[8076]n.
    Ha desaparecido un ratoncito.[C052]⧗
```
```yaml
- text: "Ciudad Baco[C052]⧗\n\n[PROMPT][8053]Guardar DATOS?  \nS[805C]⮊✕ No⮊○⧗\n\n"
```
```yaml
- id: 0            # AREA00/MFNT000S, menu 8 px
  content: [ {box: {height: 29, width: 88}}, {text: "Usado[8005]a[8006] \n"} ]
```

**WFM round trip: 0/369 byte-identical.** `encode` always succeeds but is a re-authoring tool, not an inverse:
it regenerates the glyph set only from the `fonts/br` PNGs for characters present in the text and **drops** every
unmapped `[80xx]` and unknown `[C0xx]` (log: "Skipping unmapped byte"). Result: fewer glyphs (CFNT999S 0x47 -> 0x39),
different pointer tables, 184/369 files change size, the rest same size but ~75% of bytes differ. Not usable to
rebuild the disc losslessly as is.

**GAM: 816/816 unpack OK** (AREA00..19 `*.GAM` and `SYSTEM/*.GAM`; e.g. A000.GAM 178 bytes -> 0x4000 unpacked).
`pack` then `unpack` returns the identical data for 816/816 (semantic round trip OK) but **0/816 byte-identical**
packed files: the tool's compressor is different (always smaller, e.g. A002 142,971 -> 124,138). For a bit-exact
rebuild we need either a matching compressor or to keep original `.GAM` bytes and only re-pack modified ones.

**`cd dump`: OK**, 2287 files, byte-identical to our `game/disc/fs` (300 spot-checked, 0 differences). Not needed
because we already have `fs/`, but its FLA/MSF parsing is the piece that could help to rebuild the image.

## Not covered
`*.000` (e.g. AREA00/A004.000 273 KB, SYSTEM/A0xxxx.000 512 B pieces; 202 in SYSTEM + per-area), `X000..X800.BIN`
(overlay code, already handled by the decomp), `SYSTEM/*.BIN` (D?SPSUB*.BIN etc., 225), `DSPCLUT.TIM`,
`SUBMAP.WMD`, `SOUND/*.WVD` (SPU/VAB-like banks), `MOVIE/*.STR` (standard XA/STR, ffmpeg/jPSXdec), `CLUT*.GAM`
contents (unpack OK, inner format unknown), `EXE/MAIN*.EXE` and the inner structure of unpacked GAM
(tilemaps, sprites, scripts). The 0x00-led data inside UNGAM is unparsed.

## Plan to adapt for PAL
1. Keep our own copy (outside the tool): write a thin Python script in `tools/` (later) that calls
   `tombatools` for `gam unpack` and `cd dump` only; do not vendor its source.
2. (DONE in the fork, see top) Fix the text side: build a per-file glyph table by matching glyph bitmaps against the `us/br/eu` fonts and adding
   Spanish ones (á é í ó ú ñ ¿ ¡ Ó Í ...) as new reference PNGs (`00E1.png`, `00F1.png`, ... named by codepoint);
   add a `fonts/es` set and make the font dir and encode dir selectable (patch needed: hard-coded `fonts/br`).
3. (DONE in the fork, 369/369) Make WFM encode lossless: keep unmapped `[80xx]` as raw glyph indices (or take the glyph table from the original
   file and only rewrite the dialogue stream), and emit unknown `[C0xx]` verbatim. Then the target is
   decode+encode == original for all 369 WFM; check with `cmp` in a script.
4. GAM: implement the original LZ compressor variant so `pack(unpack(x)) == x` (try matching the greedy/lazy
   choices on the 816 samples), or store only the original GAM bytes plus a manifest of modified ones.
5. Add a script that rebuilds `fs/` from decoded assets and compares SHA-1 against the original `Tombi.bin`, and use
   `fla recalc` only if a file size changes.
6. Then reverse the remaining formats in order of size/impact: `*.000`, `X?00.BIN` data tables, `SYSTEM/*.BIN`,
   `WVD`, and wrap `MOVIE/*.STR` as opaque.
