# MAIN0.EXE memory map (imported with ghidra_psx_ldr)

| Block | Start | End | Size | Permissions | Has data |
|---|---|---|---|---|---|
| .rdata | 80010000 | 800163F3 | 0x63F4 | R | yes |
| .text | 800163F4 | 800778EF | 0x614FC | R X | yes |
| .data | 800778F0 | 80098C43 | 0x21354 | R W | yes |
| .sdata | 80098C44 | 80098FFF | 0x3BC | R W | yes |
| .sbss | 8009BC48 | 8009BC7F | 0x38 | R W | no |
| .bss | 8009BC80 | 800A3FDF | 0x8360 | R W | no |

The gaps (80099000, 800A3FE0-801FFFFF) are RAM with no contents. There are also the
hardware blocks (1F800000+) and `GTEMAC` (20000000, GTE macros).

## Functions (notes/functions_main0.csv)
- 1,822 in total. 153 are GTE macros at `20000000-20000263` (not game code).
- 1,343 are inside `.text`: 881 named (Psy-Q libraries) and 462 unnamed `FUN_`.
- 326 `FUN_` are at `800E8214-80134E60`: RAM with no contents in the EXE. **They are not false
  positives**: they are **overlay** code that the game loads from disc at runtime.
  43 MAIN0 functions call 218 distinct targets in that region. We still need to identify
  which disc file is loaded there (candidates: `X000.BIN`... in `AREAxx`).
- The game code left to decompile is those 462 functions (~208 KB).

## Relation to the comparison between MAINx
`.text` barely changes across MAIN0..8 (1-3%); what changes (20-70%) is `.data`
(from 800778F0). In other words, the code is shared and each MAINx carries its own data.
