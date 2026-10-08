// FUNC 80123648 168 X000
// MATCHING 80123648 168
#include "raw7.h"
extern char *ObjAlloc();
void FUN_80123648(char *p, char *q, int c)
{
    char *o = ObjAlloc();
    if (o != 0) {
        U8(o, 0) = 4;
        U8(o, 2) = 7;
        U8(o, 3) = c;
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
