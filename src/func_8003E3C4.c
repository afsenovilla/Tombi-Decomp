// FUNC 8003af70 36 MAIN0
// MATCHING 8003af70 36
// Ported from psx_tomba (scriptop.c, func_8003E3C4); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_8003E3C4(void)
{
    *(s32*)((u8*)SCRIPT_CTX + 0x1190) = D_8009BCD4;
    SCRIPT_CTX->pc++;
}
