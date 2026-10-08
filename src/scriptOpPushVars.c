// FUNC 80038640 92 MAIN0
// MATCHING 80038640 92
// Ported from psx_tomba (script.c, scriptOpPushVars); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpPushVars(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* q = (u8*)p;
    u8* s = (u8*)p;
    s32 i;
    s32 v;

    for (i = 0; i < 0x40; i++) {
        v = *(s32*)(s + 0x1090);
        *(s32*)(q + *(u16*)(q + 0x8C) * 4 + 0x90) = v;
        *(u16*)(q + 0x8C) = *(u16*)(q + 0x8C) + 1;
        s += 4;
    }
    p->pc++;
}
