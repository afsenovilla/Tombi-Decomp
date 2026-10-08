// FUNC 8003f144 120 MAIN0
// MATCHING 8003f144 120
// Ported from psx_tomba (itemspawn.c, dispatchAreaItemDraw); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void dispatchAreaItemDraw(void)
{
    if (GAME.selectedArea == AREA00_VILLAGEOFALLBEGINNINGS) {
        func_80122F64();
    } else if (GAME.selectedArea == AREA04_HAUNTEDMANSION) {
        func_8011D844();
    } else if (GAME.selectedArea == AREA10_DEEPJUNGLE) {
        func_8011CD70();
    } else if (GAME.selectedArea == AREA13_PIGISLAND) {
        func_8011602C();
    }
}
