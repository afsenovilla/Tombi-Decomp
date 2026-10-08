// FUNC 80028548 104 MAIN0
// MATCHING 80028548 104
// Ported from psx_tomba (inventory.c, dampCameraAngle); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

s32 dampCameraAngle(void)
{
    s16* p = &D_1F8000E6;
    s16  v = *p;

    if (v != 0) {
        if (v > 0) {
            v = v - 2;
            *p = v;
            if (v < 0) {
                *p = 0;
            }
        } else {
            v = v + 2;
            *p = v;
            if (v > 0) {
                *p = 0;
            }
        }
        return 1;
    }
    return 0;
}
