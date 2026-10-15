// FUNC 8004c170 64 MAIN0
// MATCHING 8004c170 64
// Ported from psx_tomba (objpool.c, spawnUnlayeredObjectAndInit); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void spawnUnlayeredObjectAndInit(u8 arg0)
{
    u8* p = allocObjectUnlayered();

    if (p != NULL) {
        p[0] = 1;
        p[2] = arg0;
        updateSectionSpawnManager(p);
    }
}
