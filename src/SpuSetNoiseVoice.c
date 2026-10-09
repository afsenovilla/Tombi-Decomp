// FUNC 80075ffc 36 MAIN0
// MATCHING 80075ffc 36
// Portado de psx_tomba (psyq/libspu/s_snv.c, SpuSetNoiseVoice); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "libspu_internal.h"

void SpuSetNoiseVoice(s32 a, s32 b) { _SpuSetAnyVoice(a, b, 0xCA, 0xCB); }
