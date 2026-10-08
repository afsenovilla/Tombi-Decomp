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

## The other area overlays (X001..X019)

The Spanish release ships 17 unique area overlays (`X1nn`..`X8nn` are the other languages and are ignored):

| File | Used by | Size | SHA-1 |
|------|---------|------|-------|
| `AREA00/X000.BIN` | AREA00 | 343,888 | `ce168475f667f2ca8842eabad2aec829e301c5c5` |
| `AREA01/X001.BIN` | AREA01, AREA07 | 360,316 | `923f812dcfd7da18ee0f7017e5b45a5e38ee3015` |
| `AREA02/X002.BIN` | AREA02 (AREA19 also ships a copy) | 228,576 | `646aa8b5ad97f70b299b982b3260f9f731a142bb` |
| `AREA03/X003.BIN` | AREA03 | 338,504 | `440ae76f5012e3778b908743d7d030c350caf7d3` |
| `AREA04/X004.BIN` | AREA04, AREA12 | 318,180 | `f01c882217efee45bf76aba54ee4fadfa6c25665` |
| `AREA05/X005.BIN` | AREA05 | 206,616 | `0d2778c0d4b3e5b5dfc40fc46053e46d9d785b17` |
| `AREA06/X006.BIN` | AREA06 | 241,940 | `1bd5b648feb75a580c14db6715e057cb035c1db1` |
| `AREA08/X008.BIN` | AREA08 | 198,028 | `5e098dccfd123af09967ca778c83223e8a21716d` |
| `AREA09/X009.BIN` | AREA09 | 299,500 | `1b44ea24890128c3d3789b0badb252135ba4dec1` |
| `AREA10/X010.BIN` | AREA10 | 306,772 | `5093e40399319f5987668bfb1d042713216d356e` |
| `AREA11/X011.BIN` | AREA11 | 214,788 | `016559470c5a253deb5a38f685ff312c39c2cb46` |
| `AREA13/X013.BIN` | AREA13 | 208,200 | `a4e8668eca85a32d6d0cdfa05ddffd6ad40bfbb0` |
| `AREA14/X014.BIN` | AREA14 | 272,256 | `4ee916eb3215ccae64773e1a9a547ab8dbcd1ff5` |
| `AREA16/X016.BIN` | AREA16 | 210,692 | `7c93915bc943e8d9d8fa5d5efbd02f7e563650e4` |
| `AREA17/X017.BIN` | AREA17 | 214,648 | `59c5964cbb4536bc62b1a65262d963fab1ad3d33` |
| `AREA18/X018.BIN` | AREA18 | 216,364 | `83eca42eb30944bb0c61056632b2ef7bb9891b34` |
| `AREA19/X019.BIN` | AREA19 | 190,180 | `94060c10f7b6e3af1adc55a0c9cff4b81890988c` |

AREA15 has no code overlay. In the repository the files live in `game/AREAnn/X0nn.BIN`; the program name used in
`// FUNC` headers and the tools is `X0nn`.

### Load address: 0x800E8028 for all of them
Same method as for X000, run for every overlay and for every base from `0x800E7FE8` to `0x800E8068`:
- all 17 files start with the same header (`0x1D` and the same 29 pointers `0x800E9ECC`...); with base
  `0x800E8028` the pointers land on the `jal`/`j` entry stubs at offset `0x1EA4`;
- the overlay's own `jal` targets: with `0x800E8028`, 75-97 % of them fall right after a `jr ra` (e.g. X001:
  296 of 354, X019: 168 of 173); the best other base gets at most 5;
- MAIN0 `jal` targets in `0x800E8028..0x80140000` that fall after `jr ra; nop`: 12-59 per overlay at
  `0x800E8028`. Of the 332 distinct targets, 241 are a function start of at least one area overlay; most of
  the remaining 91 fall in the shared header/data area (`0x800E8028..0x800E9ECC`), so they are either data words
  of MAIN0 or belong to yet another kind of overlay.

### Shared prefix
The first 188,028 bytes (`0x800E8028..0x80115EA4`) are **byte-identical in all 17 files**: header, entry stubs,
the ~300 common object handlers (`0x800E9F74..0x80114A48`, 173,668 bytes of code) and a data block
(`0x80114A48..0x80115EA4`). Per-area code starts at `0x80115EA4`. X000 continues with its own code like the others.
So every function matched in that range of X000 is matched in every overlay by a plain copy.

### Function boundaries (`tools/areas.py bounds` -> `notes/functions_x0nn.csv`)
Columns `address,size,name,twin`. Built in this order:
1. *Twins*: every matched `src/*.c` (MAIN0 and X000) and every Ghidra function of X000 is searched in the
   overlay with relocation fields masked (jal/j targets, `lui` immediates and the `%lo` of instructions based on
   a `lui` register); a hit at a plausible start (jal target, after `jr ra`, `addiu sp,sp,-N`, after data) keeps
   the twin's exact boundaries. `twin` = `MAIN0:addr` / `X000:addr`.
2. *Linear sweep* from every internal `jal` target and from code that directly follows a function
   (`addiu sp,-N` or referenced by a data word): the function ends at the first `jr ra` that no forward branch
   skips; a `jal` into the middle of a function splits it (as Ghidra does).
3. *Gap filling*: uncovered ranges are scanned for valid code that ends in `jr ra` with every branch inside it
   (pointer-looking words such as `lb rt,imm(zero)` count as data). This finds the leaf handlers that are only
   reached through pointers built with `lui/addiu`.
Run on X000, the heuristic part alone agrees with Ghidra on 464 of 531 functions; the differences are Ghidra
thunks (jal into the middle of a function) and ~75 small functions Ghidra missed.
Functions without a MAIN0/X000 twin that are identical (masked) to a function of an earlier overlay have
`twin` = `X0nn:addr` of the first occurrence.

### Twins (`tools/areas.py twins` -> `src/x0nn/*.c`)
For each overlay function whose twin is a matched `src/*.c`, the twin's source is compiled once to get its
relocations; the symbol addresses are read from the two retail copies (field minus our addend), and every
symbol whose address differs in the overlay is renamed (`FUN_/func_/D_...` names get the new hex address,
other functions the overlay's own name from the csv, other data `D_<addr>`). The file gets
`// FUNC <addr> <size> X0nn` and is kept only if `tools/ncheck.py` prints MATCH (then `// MATCHING` is added).
File names follow the twin (hex names get the overlay address); a name used twice in one overlay gets an
`_<ADDR>` suffix. Inside the shared prefix the addresses are the same as in X000, so those copies are
identical to the X000 sources.

Tools that know the overlays: `tools/ncheck.py` / `tools/matchcheck.py` (PROG `X001`..`X019`, base
`0x800E8028`; ncheck resolves symbols with MAIN0's table plus the overlay's own: `config/symbol_addrs_x0nn.txt`,
`notes/functions_x0nn.csv`, `src/x0nn/`), `tools/gen_symbols.py`, `tools/build_report.py` (`config/x0nn.yaml`),
`tools/progress.py` (section "Area overlays" of `docs/PROGRESS.md` and `notes/todo_areas.csv`), `tools/fn.py`.
`tools/build_full.py` still links only MAIN0 and X000.
