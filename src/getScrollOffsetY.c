// FUNC 8002aeb8 88 MAIN0
// MATCHING 8002aeb8 88
// VERSION SLPM_80165 slpm80165 8002B36C 84
// Ported from psx_tomba (inventory.c, getScrollOffsetY); MIT licence of the original project.
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002AEB8 (getScrollOffsetY.c):  table index x / 360 (PAL (x >> 8) / 360).
// Ported from psx_tomba (inventory.c, getScrollOffsetY); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 getScrollOffsetY(void)
{
    s16 v = NEG_SIN_TABLE[D_800A38DC / 360];

    return (v * 1027) >> 12;
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 getScrollOffsetY(void)
{
    s16 v = NEG_SIN_TABLE[(D_800A38DC >> 8) / 360];

    return (v * 1027) >> 12;
}
#endif
