// FUNC 8004c1b0 60 MAIN0
// MATCHING 8004c1b0 60
// Ported from psx_tomba (objpool.c, func_8005A148); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_8005A148(u8 arg0)
{
    u8* p = allocObjectUnlayered();

    if (p != NULL) {
        p[0] = 1;
        p[2] = arg0;
    }
}
