// FUNC 8003bcd0 176 MAIN0
// MATCHING 8003bcd0 176
// Ported from psx_tomba (scriptop.c, updateL2Type30Object); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void updateL2Type30Object(void)
{
    switch (GAME.selectedArea) {
        case AREA05_BACCUSVILLAGE:
            func_800F05B0();
            return;
        case AREA08_BACCUSLAKE:
            func_800F18C0();
            return;
        case AREA11_VILLAGEOFCIVILIZATION:
            func_800F2CA8();
            return;
        case AREA16_VILLAGEOFCIVILIZATIONCLOCKTOWER:
            func_800F3D84();
            return;
        case AREA17_VILLAGEOFCIVILIZATIONIRONTOWER:
            func_800F528C();
            return;
        case AREA18_VILLAGEOFCIVILIZATIONYCROSSING:
            func_800F548C();
            return;
        case AREA19_VILLAGEOFCIVILIZATIONPURIFIED:
            func_800F6B4C();
            // fallthrough
        default:
            return;
    }
}
