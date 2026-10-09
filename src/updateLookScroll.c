// FUNC 8002a034 200 MAIN0
// MATCHING 8002a034 200
// Ported from psx_tomba (inventory.c, updateLookScroll); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

void updateLookScroll(u8* self)
{
    s16 v = D_1F8000E6;
    s32 w;

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
    }
    w = *(s32*)(self + 0x24);
    if (w != 0) {
        if (w > 0) {
            *(s32*)(self + 0x24) = w - 0x80;
        } else {
            *(s32*)(self + 0x24) = w + 0x80;
        }
    } else {
        self[0x71] = 0;
        self[0x72] = 0;
        self[0x73] = 0;
    }
    func_80027ED8(self);
    func_80028250(self);
}
