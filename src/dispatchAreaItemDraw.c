// FUNC 8003f144 120 MAIN0
// MATCHING 8003f144 120
// Ported from psx_tomba (itemspawn.c, dispatchAreaItemDraw); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void dispatchAreaItemDraw(void)
{
    if (GAME.selectedArea == AREA00_VILLAGEOFALLBEGINNINGS) {
        func_80123C20();
    } else if (GAME.selectedArea == AREA04_HAUNTEDMANSION) {
        func_8011E594();
    } else if (GAME.selectedArea == AREA10_DEEPJUNGLE) {
        func_8011DA50();
    } else if (GAME.selectedArea == AREA13_PIGISLAND) {
        func_80116CDC();
    }
}
