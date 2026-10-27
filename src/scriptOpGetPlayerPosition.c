// FUNC 80039e08 76 MAIN0
// MATCHING 80039e08 76
// Ported from psx_tomba (scriptop.c, scriptOpGetPlayerPosition); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpGetPlayerPosition(void)
{
    *(s32*)((u8*)SCRIPT_CTX + 0x1190) = PLAYER_H[1];
    *(s32*)((u8*)SCRIPT_CTX + 0x1194) = PLAYER_Y;
    *(s32*)((u8*)SCRIPT_CTX + 0x1198) = PLAYER_D[1];
    SCRIPT_CTX->pc++;
}
