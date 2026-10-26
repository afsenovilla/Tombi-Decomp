// FUNC 8002ae48 112 MAIN0
// MATCHING 8002ae48 112
// VERSION SLPM_80165 slpm80165 8002B300 108
// Ported from psx_tomba (inventory.c, getScrollOffsetX); MIT licence of the original project.
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8002AE48 (getScrollOffsetX.c): table index x / 360 (PAL (x >> 8) / 360).
// Ported from psx_tomba (inventory.c, getScrollOffsetX); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s16 getScrollOffsetX(void)
{
    s32 x = D_800A38DC;
    s32 v = NEG_SIN_TABLE[x / 360];
    s32 r = (v * 567) >> 12;

    if (x > 0) {
        r = r - 0x14;
    } else {
        r = r + 0x14;
    }
    return r;
}
#else
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s16 getScrollOffsetX(void)
{
    s32 x = D_800A38DC;
    s32 v = NEG_SIN_TABLE[(x >> 8) / 360];
    s32 r = (v * 567) >> 12;

    if (x > 0) {
        r = r - 0x14;
    } else {
        r = r + 0x14;
    }
    return r;
}
#endif
