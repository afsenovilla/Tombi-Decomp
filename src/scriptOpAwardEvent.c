// FUNC 8003aa68 84 MAIN0
// MATCHING 8003aa68 84
// Ported from psx_tomba (scriptop.c, scriptOpAwardEvent); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void scriptOpAwardEvent(void)
{
    unkstruct_8009E458* p = D_8009E458;

    EVENT id = *(s32*)((u8*)p + 0x1190);

    if (*(s32*)((u8*)p + 0x1194) != 0) {
        awardEventProgress(id, 1, 0);
    } else {
        awardEventProgress(id, 0, 0);
    }
    p->pc++;
}
