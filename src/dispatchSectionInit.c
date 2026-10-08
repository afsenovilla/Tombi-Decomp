// FUNC 8002a9ac 108 MAIN0
// MATCHING 8002a9ac 108
// Ported from psx_tomba (inventory.c, dispatchSectionInit); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

void dispatchSectionInit(void)
{
    switch (GAME.selectedSection) {
        case 0:
            func_800E7574();
            return;
        case 3:
            func_800E79F8();
            return;
        case 1:
        case 2:
        case 4:
        case 5:
            func_801156A8();
            // fallthrough
        default:
            return;
    }
}
