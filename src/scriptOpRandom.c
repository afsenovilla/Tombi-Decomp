// FUNC 80038290 108 MAIN0
// MATCHING 80038290 108
// Ported from psx_tomba (script.c, scriptOpRandom); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpRandom(void)
{
    unkstruct_8009E458* p = D_8009E458;
    u8* script = D_8009C974;
    s32 idx = script[p->pc + 1];

    *(s32*)(idx * 4 + (s32)p + 0x1090) = nextRandom();
    p->pc += 2;
}
