// FUNC 80067270 96 MAIN0
// MATCHING 80067270 96
// Portado de psx_tomba (psyq/libcd/c_002.c, StClearRing); licencia MIT del proyecto original.
#define SKIP_ASM
#include "common.h"
#include "psyq/libcd.h"

extern s32 D_8009C334;
extern s16 D_8009C90C;
extern s32 D_8009D544;
extern s32 D_8009D6A0;
extern s32 D_800A2264;
extern s32 D_800A2268;
extern s32 D_800A2524;
extern s32 D_800A3FD8;

void StClearRing(void)
{
    D_800A2524 = 0;
    D_800A2268 = 0;
    D_800A2264 = 0;
    D_8009D6A0 = 0;
    init_ring_status(0, D_800A3FD8);
    D_8009D544 = 0;
    D_8009C90C = 0;
    D_8009C334 = 0;
}
