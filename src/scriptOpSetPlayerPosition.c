// FUNC 80039db8 80 MAIN0
// MATCHING 80039db8 80
// Ported from psx_tomba (scriptop.c, scriptOpSetPlayerPosition); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetPlayerPosition(void)
{
    D_800A53D8[1] = *(s32*)((u8*)SCRIPT_CTX + 0x1190);
    D_800A53AE = *(s32*)((u8*)SCRIPT_CTX + 0x1194);
    D_800A53DC[1] = *(s32*)((u8*)SCRIPT_CTX + 0x1198);
    SCRIPT_CTX->pc++;
}
