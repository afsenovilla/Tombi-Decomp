// FUNC 8002abf8 112 MAIN0
// MATCHING 8002abf8 112
// Ported from psx_tomba (inventory.c, dispatchSectionConfirm); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

void dispatchSectionConfirm(void)
{
    switch (GAME.selectedSection) {                     // irregular
        case 1:
            func_80115EA8();
            return;
        case 2:
        case 0:
            func_800E9174();
            return;
    }
}
