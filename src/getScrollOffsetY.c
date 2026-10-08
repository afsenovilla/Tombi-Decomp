// FUNC 8002aeb8 88 MAIN0
// MATCHING 8002aeb8 88
// Ported from psx_tomba (inventory.c, getScrollOffsetY); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 getScrollOffsetY(void)
{
    s16 v = D_8007D988[(D_800A38DC >> 8) / 360];

    return (v * 1027) >> 12;
}
