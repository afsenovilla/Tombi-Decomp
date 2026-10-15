// JPDEMO slpm80165 8002B36C 84
// Demo version of PAL main0 8002AEB8 (getScrollOffsetY.c):  table index x / 360 (PAL (x >> 8) / 360).
// Ported from psx_tomba (inventory.c, getScrollOffsetY); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 getScrollOffsetY(void)
{
    s16 v = D_8007D988[D_800A38DC / 360];

    return (v * 1027) >> 12;
}
