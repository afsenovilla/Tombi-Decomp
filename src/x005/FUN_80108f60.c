// FUNC 80108f60 128 X005
// MATCHING 80108f60 128
#include "raw7.h"
extern unsigned char DAT_801152e8[];
extern char DAT_80010e34[];
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fe94(char *, int);
void FUN_80108f60(char *o)
{
    int t;
    FUN_8001e5f4(0x1c, 0x7f);
    U8(o, 0xa3) = 1;
    PTR(o, 0x24) = DAT_80010e34;
    FUN_8001fe94(o, 2);
    S16(*(char **)(o + 0x44), 2) = U16(o, 0xf6);
    t = DAT_801152e8[S16(o, 0xb0)];
    U8(o, 0xac) = 0;
    U8(o, 6) = 3;
    S32(o, 0x8c) = t;
}
