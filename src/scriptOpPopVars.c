// FUNC 8003869c 88 MAIN0
// MATCHING 8003869c 88
// Ported from psx_tomba (script.c, scriptOpPopVars); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpPopVars(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* q = (u8*)p;
    s32 i;
    u16 n;

    for (i = 0x3F; i >= 0; i--) {
        n = *(u16*)(q + 0x8C) - 1;
        *(u16*)(q + 0x8C) = n;
        *(s32*)(q + i * 4 + 0x1090) = *(s32*)(q + n * 4 + 0x90);
    }
    p->pc++;
}
