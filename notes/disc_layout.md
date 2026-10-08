# Disc: Tombi! (PAL, Spanish version)

- Main executable: `SCES_013.31` (651,264 bytes, dated 1998-06-19)
- SHA-1 of the `.bin`: `ec50074b6fd98f611bae11e3de68d6e76ffdd6d4` (not verified against Redump)
- `SYSTEM.CNF`: `BOOT = cdrom:\SCES_013.31;1`, `TCB = 4`, `EVENT = 10`, `STACK = 80200000`

## Directories (extracted with dumpsxiso 2.30)
`AREA00`..`AREA19`, `EXE`, `MOVIE`, `SOUND`, `SYS`, `SYSTEM`, `ZZZ`

`AREAxx` contains `*.GAM`, `*.WFM` (fonts `CFNT*`/`MFNT*`), `*.BIN` (`X000.BIN`...) and `*.000`.
`EXE/` is still pending review: possible code overlays.

## EXE/MAIN0..8.EXE (analyzed with tools/psxexe_info.py)
- All 9 are 563,200 bytes (0x89000 + 0x800 header), load at `0x80010000` and end at `0x80099000`.
- Same entry point `0x8006C044`, except `MAIN5` (`0x8006C02C`).
- Only 57.6% of the bytes are identical across all 9. The variable part grows towards the end:
  ~1-3% different up to `0x8006F800`, 20-70% from `0x8007F800` onwards.
- `MAIN6` and `MAIN8` are almost identical (0.3%). `MAIN5` is the most different (34-41% versus the rest).
- `SCES_013.31` also loads at `0x80010000`, so it looks like a loader that
  replaces its own image with one of the `MAINx` (to be confirmed).
