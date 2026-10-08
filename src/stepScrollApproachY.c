// FUNC 80028468 112 MAIN0
// MATCHING 80028468 112
// Ported from psx_tomba (inventory.c, stepScrollApproachY); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 stepScrollApproachY(u8* self)
{
    s32 t;
    s32 cur;

    switch (*(s8*)(self + 0x71)) {
    case 0:
        t = *(s8*)(self + 0x73) << 8;
        cur = *(s32*)(self + 0x24);
        if (t < cur) {
            *(s32*)(self + 0x24) = cur - 0x80;
            return 0;
        }
        return 1;
    case 1:
        t = *(s8*)(self + 0x73) << 8;
        cur = *(s32*)(self + 0x24);
        if (cur >= t) {
            return 1;
        }
        *(s32*)(self + 0x24) = cur + 0x80;
        return 0;
    }
    return 0;
}
