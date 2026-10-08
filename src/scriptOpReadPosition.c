// FUNC 80039e08 76 MAIN0
// MATCHING 80039e08 76
// Ported from psx_tomba (scriptop.c, scriptOpReadPosition); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpReadPosition(void)
{
    *(s32*)((u8*)D_8009E458 + 0x1190) = D_800A53D8[1];
    *(s32*)((u8*)D_8009E458 + 0x1194) = D_800A53AE;
    *(s32*)((u8*)D_8009E458 + 0x1198) = D_800A53DC[1];
    D_8009E458->pc++;
}
