// FUNC 8006eed8 32 MAIN0
// MATCHING 8006eed8 32
// Ported from eternal-silver-sssc-decomp decomp/SLUS_006.28/src/libsnd_ssstart_2.c (MIT, (c) 2026 Bash Alarmist), derived from Sony's PSY-Q libraries.
// The twin SsStart2 (8006eef8) was already ported from psx_tomba (src/SsStart2.c).
// Ghidra's FID named this address SpuInitHot; the bytes are libsnd SsStart: _SsStart(1).
#define SKIP_ASM
#include "common.h"

void _SsStart(s32 use_vsync);

void SsStart(void) { _SsStart(1); }
