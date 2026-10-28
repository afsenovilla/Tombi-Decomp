// VERSION SLPM_80165 slpm80165 8002EF6C 72
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 8003040C (dispatchAreaInitA.c): second case is area 6 (PAL area 3).
// Ported from psx_tomba (areainit.c, dispatchAreaInitA); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void dispatchAreaItemInit(void)
{
    if (GAME.selectedArea == AREA00_VILLAGEOFALLBEGINNINGS) {
            a00BuildPlacedObjectList();
    } else if (GAME.selectedArea == AREA06_DIRTMOTOCROSS) {
            func_8011a544();
    }
    return;
}
#endif
