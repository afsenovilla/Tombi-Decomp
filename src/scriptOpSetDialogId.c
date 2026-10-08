// FUNC 8003aabc 44 MAIN0
// MATCHING 8003aabc 44
// Ported from psx_tomba (scriptop.c, scriptOpSetDialogId); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpSetDialogId(void)
{
    unkstruct_8009E458* q = D_8009E458;

    D_8009BCAA = *(s32*)((u8*)q + 0x1190);
    asm("");
    q->pc++;
}
