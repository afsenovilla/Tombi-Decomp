// FUNC 8002171c 316 MAIN0
#include "raw7.h"
extern unsigned short c960, c962, c982;
extern unsigned char flag;
extern short **tabs[];
void FUN_8002171c(char *o)
{
    short *p = (short *)((int)tabs[c960][c962] + c982 * 8);
    short v;
    unsigned short u;
    S32(o, 0xec) = *p++ << 16;
    S32(o, 0xf0) = *p << 16;
    S32(o, 0xf4) = p[1] << 16;
    if (c960 == 2) {
        u = 4;
    } else {
        if (c960 < 3) {
            if (c960 != 0) return;
            if (flag != 0 || (v = 0x40, c962 != 0)) {
                if (c962 != 3) return;
                S16(o, 0xee) = 0xd2;
                return;
            }
            goto set;
        }
        if (c960 == 4) {
            u = 0xf;
        } else {
            u = 8;
            if (c960 != 10) return;
        }
    }
    v = 0x90;
    if (c962 != u) return;
set:
    S16(o, 0xee) = v;
}
