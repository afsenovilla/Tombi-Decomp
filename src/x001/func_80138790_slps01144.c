// VERSION SLPS_01144 x01 80138790 208
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
#define U16(p, o) (*(unsigned short *)((p) + (o)))
extern unsigned char D_8009EA53, D_800A7BE1, D_8009E4EA, D_800A7C38;
extern short D_800A7B8E;
extern Fix16 *PLAYER_H;

int func_80138790_slps01144(unsigned char *o)
{
    int r = 0;
    if (D_8009EA53 != 0 && D_8009EA53 < 4 && D_800A7BE1 != 0 && (unsigned short)(PLAYER_H->p.whole - 2733) < 64 &&
        D_800A7B8E >= -271) {
        if (U16(o, 0x2c) != 0) {
            r = 1;
        } else if (SPAD.s1fc & SPAD.s3c8) {
            r = 1;
            D_8009E4EA = 1;
            D_800A7C38 = 1;
            U16(o, 0x2c) = 1;
        }
    }
    return r;
}
#endif
