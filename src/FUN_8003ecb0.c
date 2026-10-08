// FUNC 8003ecb0 256 MAIN0
// MATCHING 8003ecb0 256
#include "raw7.h"
extern char *alloc();
extern unsigned char t1[];
extern unsigned char *t2[];
void FUN_8003ecb0(char *a, char *b, short c, short d, char *e)
{
    char *o = alloc();
    if (o != 0) {
        U8(o, 0) = 4;
        U8(o, 2) = 9;
        U8(o, 3) = U8(a, 1);
        U8(o, 0xc) = U8(a, 2);
        U8(o, 0xf) = t2[t1[U8(a, 1)]][3];
        U16(o, 0x2e) = 0;
        **(int **)(o + 0x40) = S16(b, 2) << 16;
        S32(o, 0x14) = S16(b, 6) << 16;
        **(int **)(o + 0x44) = S16(b, 10) << 16;
        S16(o, 0x80) = c;
        S16(o, 0x82) = d;
        U8(o, 0x6b) = U8(e, 0x6b);
    }
}
