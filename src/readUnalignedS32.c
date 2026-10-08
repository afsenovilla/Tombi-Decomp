// FUNC 80038084 56 MAIN0
// MATCHING 80038084 56
// Ported from psx_tomba (script.c, readUnalignedS32); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"


s32 readUnalignedS32(u8* src)
{
    u8  buf[4];
    u8* d = buf;

    do {
        *d = *src;
        d++;
        src++;
    } while ((s32)d < (s32)&buf[4]);
    return *(s32*)buf;
}
