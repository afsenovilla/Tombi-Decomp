// JPDEMO slpm80165 8003BCCC 156
// Demo version of PAL x000 80123648 (FUN_80123648.c): subtype always 0 (no third argument).
#include "raw7.h"
extern char *allocObjectLayer5();
void FUN_80123648(char *p, char *q)
{
    char *o = allocObjectLayer5();
    if (o != 0) {
        U8(o, 0) = 4;
        U8(o, 2) = 7;
        U8(o, 3) = 0;
        U8(o, 0xc) = 0;
        *(signed char *)&U8(o, 0xf) = -10;
        S16(o, 0x2e) = 0;
        *(int *)(o + 0x10) = *(short *)(q + 2) << 16;
        *(int *)(o + 0x14) = *(short *)(q + 6) << 16;
        *(int *)(o + 0x18) = *(short *)(q + 10) << 16;
        *(int *)(o + 0x94) = (int)p;
        S16(o, 0xb4) = *(unsigned short *)(p + 0xb6);
    }
}
