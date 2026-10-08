// FUNC 8004c170 64 MAIN0
// MATCHING 8004c170 64
// Ported from psx_tomba (objpool.c, func_8005A108); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


void func_8005A108(u8 arg0)
{
    u8* p = allocObjectUnlayered();

    if (p != NULL) {
        p[0] = 1;
        p[2] = arg0;
        func_8005A184(p);
    }
}
