// FUNC 80039e08 76 MAIN0
// MATCHING 80039e08 76
// Ported from psx_tomba (scriptop.c, scriptOpGetPlayerPosition); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpGetPlayerPosition(void)
{
    *(s32*)((u8*)SCRIPT_CTX + 0x1190) = D_800A53D8[1];
    *(s32*)((u8*)SCRIPT_CTX + 0x1194) = D_800A53AE;
    *(s32*)((u8*)SCRIPT_CTX + 0x1198) = D_800A53DC[1];
    SCRIPT_CTX->pc++;
}
