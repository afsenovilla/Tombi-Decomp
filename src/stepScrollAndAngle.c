// FUNC 80028a80 192 MAIN0
// MATCHING 80028a80 192
// Ported from psx_tomba (inventory.c, stepScrollAndAngle); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 stepScrollAndAngle(u8* self)
{
    s32 w = *(s32*)(self + 0x20);
    s16 v;
    s32 a;
    s32 b;

    if (w != 0) {
        if (w > 0) {
            *(s32*)(self + 0x20) = w - 0x100;
        } else {
            *(s32*)(self + 0x20) = w + 0x100;
        }
        a = 1;
    } else {
        a = 0;
    }
    v = D_1F8000E6;
    if (v != 0) {
        if (v > 0) {
            v = v - 2;
            D_1F8000E6 = v;
            if (v < 0) {
                D_1F8000E6 = 0;
            }
        } else {
            v = v + 2;
            D_1F8000E6 = v;
            if (v > 0) {
                D_1F8000E6 = 0;
            }
        }
        b = 1;
    } else {
        b = 0;
    }
    if ((a | b) != 0) {
        return 0;
    }
    self[0x6E] = 0;
    self[0x6F] = 0;
    return 1;
}
