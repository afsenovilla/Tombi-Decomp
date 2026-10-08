// FUNC 800f66e4 216 X000
#include "raw7.h"
extern char *DAT_8009c330;
extern void FUN_800efc04(void);
extern void FUN_8001fe94(char *, int);
void FUN_800f66e4(char *o)
{
    char *p = DAT_8009c330;
    S16(p, 0x2c) = 4;
    if (U16(p, 0x2e) != 4) {
        S16(p, 0x2c) = 4;
        FUN_800efc04();
        FUN_8001fe94(o, 1);
        S16(DAT_8009c330, 0x2e) = U16(DAT_8009c330, 0x2c);
    }
    U8(o, 0xac) = 1;
    U8(o, 0x9c) = 2;
    S16(o, 0x20) = 10;
    U8(DAT_8009c330, 8) = 1;
    S16(o, 0x7e) = 0;
    S16(o, 0x82) = 0;
    S32(o, 0x84) = 0;
    S32(o, 0x88) = (U16(o, 0x2e) & 1) ? 0xf0 : 0x10;
    S32(o, 0x8c) = (U16(o, 0x2e) & 1) ? 0x18 : 0xe8;
    U8(o, 5) = 2;
    U8(o, 6) = 3;
}
