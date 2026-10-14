// JPDEMO slpm80165 8002B300 108
// Demo version of PAL main0 8002AE48 (getScrollOffsetX.c): table index x / 360 (PAL (x >> 8) / 360).
// Ported from psx_tomba (inventory.c, getScrollOffsetX); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s16 getScrollOffsetX(void)
{
    s32 x = D_800A38DC;
    s32 v = D_8007D988[x / 360];
    s32 r = (v * 567) >> 12;

    if (x > 0) {
        r = r - 0x14;
    } else {
        r = r + 0x14;
    }
    return r;
}
