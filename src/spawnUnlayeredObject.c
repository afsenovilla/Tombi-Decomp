// FUNC 8004c1b0 60 MAIN0
// MATCHING 8004c1b0 60
// VERSION PCPX_96084 tomba 80082B68 60
// Ported from psx_tomba (objpool.c, spawnUnlayeredObject); MIT licence of the original project.
// ASPSX.PCPX_96084 2.34
#define SKIP_ASM
#include "common.h"
#include "game.h"


void spawnUnlayeredObject(u8 arg0)
{
    u8* p = allocObjectUnlayered();

    if (p != NULL) {
        p[0] = 1;
        p[2] = arg0;
    }
}
