# Overlays (code loaded at runtime)

## AREA00/X000.BIN (343,888 bytes)
- It is not a PS-X EXE: it has no header. First dword = `0x1D` (29), followed by a table of
  absolute pointers (`0x800E9ECC`...) that point inside the file itself.
- **Load address: `0x800E8028`** (end `0x8013C0F8`). Evidence: with that base, the
  header pointers land on valid code (`jal`/`j` + `nop`, offset 0x1EA4) and 46 of
  the 218 targets that MAIN0 calls in that region fall right after a `jr ra; nop`
  (neighboring bases give 0-4).
- The other ~172 targets called by MAIN0 fall in X000 data regions: they belong to
  another overlay loaded into the same region (to be identified; other `X*.BIN`).
- `AREA00/A000.GAM` (178 bytes): `"GAM\0"` header + small tables. Probably
  level parameters, not code.

## Importing into Ghidra
Import `X000.BIN` as **Raw Binary**, language MIPS 32 little endian,
base address `800e8028`.

## What X000.BIN does (analyzed from its decompilation, 531 functions)
- It is behavior code for level objects (enemies/scenery): it constantly
  calls MAIN0's animation functions (`AnimAdvance` 252 times,
  `AnimLoadDuration` 171, `AnimJump` 135), `Rand`, `SfxPlay`, `ObjAlloc` and the
  trigonometric tables.
- It calls 156 MAIN0 addresses; **71 of them were not functions in MAIN0** (Ghidra did not
  create them because nothing in MAIN0 calls them). List in `notes/main0_missing_functions.txt`.
  The most called: 8003FD78 (64 times), 8001FD94, 8001E5F4, 8003FACC.
