// FUNC 8003804c 56 MAIN0
// MATCHING 8003804c 56
// Ported from psx_tomba (script.c, readUnalignedU16); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


u16 readUnalignedU16(u8* src)
{
    u8  buf[2];
    u8* d = buf;

    do {
        *d = *src;
        d++;
        src++;
    } while ((s32)d < (s32)&buf[2]);
    return *(u16*)buf;
}
