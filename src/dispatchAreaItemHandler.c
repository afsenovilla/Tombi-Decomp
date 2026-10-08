// FUNC 800274e4 152 MAIN0
// MATCHING 800274e4 152
// Ported from psx_tomba (inventory.c, dispatchAreaItemHandler); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
//INCLUDE_ASM("asm/scus_942.36/nonmatchings/main/game/inventory", removeItemFromInventory);

void dispatchAreaItemHandler(s32 arg0)
{
    loadSectionHeight();
    switch (GAME.selectedArea) {                    // irregular
        case AREA00_VILLAGEOFALLBEGINNINGS:
            func_80116758(arg0);
            return;
        case AREA01_DWARFFOREST:
        case AREA07_DWARFFORESTPURIFIED:
            func_801165c0(arg0);
            return;
        case AREA03_PHOENIXMOUNTAIN:
            func_80116F74(arg0);
            return;
    }
}
