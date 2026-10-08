// FUNC 80030454 188 MAIN0
// MATCHING 80030454 188
// Ported from psx_tomba (areainit.c, func_800338A8); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_800338A8(void)
{
    switch (GAME.selectedArea) {
        case AREA00_VILLAGEOFALLBEGINNINGS:
            func_8011E30C();
            return;
        case AREA01_DWARFFOREST:
        case AREA07_DWARFFORESTPURIFIED:
            func_8011BD84();
            return;
        case AREA02_DWARFVILLAGE:
            func_800E8DA4();
            return;
        case AREA03_PHOENIXMOUNTAIN:
            func_8011A874();
            return;
        case AREA04_HAUNTEDMANSION:
        case AREA12_HAUNTEDMANSIONPURIFIED:
            func_8011B7C4();
            return;
        case AREA09_MUSHROOMVILLAGE:
            func_8011AB44();
            return;
        case AREA10_DEEPJUNGLE:
            func_80118558();
            return;
        case AREA18_VILLAGEOFCIVILIZATIONYCROSSING:
            func_801169D0();
        default:
            return;
    }
}
