// FUNC 8001f96c 116 MAIN0
// MATCHING 8001f96c 116
#include "raw7.h"
extern char *ObjAlloc();
void FUN_8001f96c(int a, int b, int c, int d)
{
    char *o = ObjAlloc();
    if (o != 0) {
        U8(o, 0) = 1;
        U8(o, 2) = 0xf;
        U8(o, 3) = a;
        S16(o, 0x12) = b;
        S16(o, 0x16) = c;
        S16(o, 0x1a) = d;
    }
}
