// FUNC 8003bcd0 176 MAIN0
// MATCHING 8003bcd0 176
// Ported from psx_tomba (scriptop.c, func_8003F124); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_8003F124(void)
{
    switch (GAME.selectedArea) {
        case AREA05_BACCUSVILLAGE:
            func_800EF5B0();
            return;
        case AREA08_BACCUSLAKE:
            func_800F08C0();
            return;
        case AREA11_VILLAGEOFCIVILIZATION:
            func_800F1CA8();
            return;
        case AREA16_VILLAGEOFCIVILIZATIONCLOCKTOWER:
            func_800F2D84();
            return;
        case AREA17_VILLAGEOFCIVILIZATIONIRONTOWER:
            func_800F428C();
            return;
        case AREA18_VILLAGEOFCIVILIZATIONYCROSSING:
            func_800F448C();
            return;
        case AREA19_VILLAGEOFCIVILIZATIONPURIFIED:
            func_800F5B4C();
            // fallthrough
        default:
            return;
    }
}
