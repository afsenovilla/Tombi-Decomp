// FUNC 800337c0 116 MAIN0
// MATCHING 800337c0 116
// Ported from psx_tomba (objlogic.c, func_80036C14); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s32 func_80036C14(u8* self)
{
    s32 r = 0;
    s16 v = func_800331c4(self);

    switch (v) {
    case 0:
    case 1:
        *(s16*)(self + 0x20) = 5;
        break;
    case 2:
        *(s16*)(self + 0x20) = 5;
        r = 1;
        break;
    }
    return r;
}
