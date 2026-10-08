// FUNC 80045b28 100 MAIN0
// MATCHING 80045b28 100
// Ported from psx_tomba (actor1.c, applyObjectPush); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void applyObjectPush(u8* arg0, u8* arg1)
{
    s32* q;

    arg1[0x69] = 0;
    if (func_80051284() == 1) {
        q = *(s32**)(arg0 + 0x40);
        *q = *q + (*(s16*)(arg1 + 0x80) << 8);
    }
}
