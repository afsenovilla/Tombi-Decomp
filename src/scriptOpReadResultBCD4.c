// FUNC 8003af70 36 MAIN0
// MATCHING 8003af70 36
// Ported from psx_tomba (scriptop.c, scriptOpReadResultBCD4); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpReadResultBCD4(void)
{
    *(s32*)((u8*)SCRIPT_CTX + 0x1190) = GAME_PLAYER_AP;
    SCRIPT_CTX->pc++;
}
