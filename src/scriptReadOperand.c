// FUNC 800380bc 100 MAIN0
// MATCHING 800380bc 100
// Ported from psx_tomba (script.c, scriptReadOperand); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s32 scriptReadOperand(u8* src, u8 kind)
{
    unkstruct_8009E458* p = D_8009E458;
    u8  buf[4];
    u8* d;

    if (kind == 0) {
        return *(s32*)((u8*)p + src[0] * 4 + 0x1090);
    }
    d = buf;
    do {
        *d = *src;
        d++;
        src++;
    } while ((s32)d < (s32)&buf[4]);
    return *(s32*)buf;
}
