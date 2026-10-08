// FUNC 800285b0 60 MAIN0
// MATCHING 800285b0 60
// Ported from psx_tomba (inventory.c, stepScrollReturnX); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 stepScrollReturnX(u8* self)
{
    s32 v = *(s32*)(self + 0x20);

    if (v != 0) {
        if (v > 0) {
            v -= 0x100;
        } else {
            v += 0x100;
        }
        *(s32*)(self + 0x20) = v;
        return 1;
    }
    return 0;
}
