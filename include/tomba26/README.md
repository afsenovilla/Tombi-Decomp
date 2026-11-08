Headers of psx_tomba at commit d848c00 (2026-11-08) (MIT; see THIRD_PARTY_NOTICES.md), without its psyq/ SDK headers.
One change: include_asm.h also skips the trailing `.include "include/macro.inc"` block under SKIP_ASM.
Used by files with a `// HEADERS tomba26` line (tools/ncheck.py overlays it onto include/tomba), the USA ports first.
