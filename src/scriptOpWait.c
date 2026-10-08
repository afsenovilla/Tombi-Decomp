// FUNC 80038ac4 64 MAIN0
// MATCHING 80038ac4 64
// Ported from psx_tomba (script.c, scriptOpWait); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpWait(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8 v = D_8009C974[p->pc + 1];

    *(s32*)((u8*)p + 0x11D0) = 0;
    *((u8*)p + 0x88) = 2;
    p->pc += 2;
    *(s32*)((u8*)p + 0x11D4) = v;
}
