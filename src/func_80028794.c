// FUNC 80025c60 100 MAIN0
// MATCHING 80025c60 100
// Ported from psx_tomba (ui.c, func_80028794); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s16 func_80028794(u8* p, s16 mode)
{
    u8  v = *p;
    s32 a;
    s32 b;
    s32 r = 0;

    switch (mode) {
    case 0:
        a = 0x80;
        b = 0x20;
        break;
    case 1:
        a = 0x10;
        b = 0x40;
        break;
    }
    if (v == 0) {
        r = a;
    }
    if (v == 0xFF) {
        r = b;
    }
    return r;
}
