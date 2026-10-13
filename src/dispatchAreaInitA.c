// FUNC 8003040c 72 MAIN0
// MATCHING 8003040c 72
// Ported from psx_tomba (areainit.c, dispatchAreaInitA); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void dispatchAreaInitA(void)
{
    if (GAME.selectedArea == AREA00_VILLAGEOFALLBEGINNINGS) {
            func_8011C804();
    } else if (GAME.selectedArea == AREA03_PHOENIXMOUNTAIN) {
            func_8011a544();
    }
    return;
}
