# PS1 Classics (PSN) releases

| Region | PSN code | Title in header | Disc inside | Boot exe |
|---|---|---|---|---|
| Japan | **NPJJ00566** | オレっ！トンバ | SLPS-01144 | `SLPS_011.44` (same id as retail) |
| USA | **NPUJ94236** | Tomba! | SCUS-94236 | `SCUS_942.36` |

Each release has `EBOOT.PBP` (the disc), `KEYS.BIN` (16-byte version key, one per title) and
`DOCUMENT.DAT` (the digital manual). Game files are never committed; CI fetches them from the
`GAME_FILES_URL_PSN_JP` / `GAME_FILES_URL_PSN_USA` secrets (job `report-psn` in `.github/workflows/report.yml`).

## Extracting: `tools/psn_eboot.py`

```
python3 tools/psn_eboot.py info    EBOOT.PBP KEYS.BIN    # works
python3 tools/psn_eboot.py extract EBOOT.PBP KEYS.BIN disc.bin   # experimental, not working yet
```

- `DATA.PSAR` (PBP offset 7) is a `PSISOIMG0000` single-disc image.
- `PSAR+0x400` is a PGD container (`\0PGD`, key index 1, DRM type 1). It is decrypted with
  sceDrmBBCipher type 1 (KIRK keys 0x39/0x63) and the KEYS.BIN version key. **This works for both
  releases.** It holds the disc id (+0x00), the TOC (+0x400, BCD), the compressed size and title (+0xE20)
  and the block index (+0x3C00: 32-byte entries `offset u32, size u16, flag u16, hash[16], pad[8]`).
- Blocks start at `PSAR+0x100000`. Each one holds 16 raw sectors (0x9300 bytes). In both releases every
  block is compressed (none are stored raw) and every flag is 1.
- **Still open:** neither the PSP LZRC decoder nor Hykem's Sony LZ decoder (RPCS3 `Crypto/lz.cpp`) decodes
  these blocks. Both fail at the first match. All blocks, JP and USA alike, start with the same bytes
  (`05..07 ff 84 cf 7?`). The two releases use different keys, so the data looks plain rather than
  encrypted, and the decoder variant or a header step must be the difference. The next step is to compare
  with psxtract's own `lz.c` and ISO loop, which this session couldn't access.
- The block hashes don't equal SHA-1 or MD5 of the retail blocks, or of the compressed data.
- `DOCUMENT.DAT` is also a PGD (DRM type 1), but the title version key doesn't open it. It needs the fixed
  manual key, so it hasn't been examined.

## What can be compared without the image

| | Retail | PSN |
|---|---|---|
| JP sectors | 123057 (`game_jp/disc/tomba_jp.bin`) | 123248 (7703 blocks); lead-out 27:25:18 → 123243 data sectors |
| USA sectors | (no local copy) | 119136 (7446 blocks); lead-out 26:30:30 |
| Tracks | 1 data track | 1 data track (TOC A0/A1 = 01) |

The JP PSN image is about 186 sectors (~430 KB) longer than our retail dump. That could be padding, a
different master, or extra files, and the extra length is the main lead to check once extraction works. A
file-by-file comparison, executable patches (libcrypt/CD timing) and anything about the cut Civilization
towns (AREA11/16/17/18 s0) all wait on the block decoder. Nothing new has been found yet for
`CUT_CONTENT.md`.
