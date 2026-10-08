// FUNC 8003040c 72 MAIN0
// MATCHING 8003040c 72
// Ported from psx_tomba (areainit.c, func_80033860); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_80033860(void)
{
    if (GAME.selectedArea == AREA00_VILLAGEOFALLBEGINNINGS) {
            func_8011BB54();
    } else if (GAME.selectedArea == AREA03_PHOENIXMOUNTAIN) {
            func_80119894();
    }
    return;
}
