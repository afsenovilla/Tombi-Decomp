// VERSION SLPS_01144 x01 80138130 252
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
#define U16(p, o) (*(unsigned short *)((p) + (o)))
#define PTR(p, o) (*(TObj **)((p) + (o)))
extern unsigned char D_8009E95A, D_800A7BE1, D_8009E4EA, D_800A7C38;
extern unsigned int D_8009E52C;
extern unsigned short D_800A7B8E;
extern Fix16 *PLAYER_H;

int func_80138130_slps01144(unsigned char *o)
{
    int r = 0;
    TObj *t = PTR(o, 0x1c);
    if (D_8009E95A >= 2) return 0;
    {
        if (!(D_8009E52C & 0x100) && D_800A7BE1 != 0 && (unsigned short)(D_800A7B8E + 1290) < 16 &&
            (unsigned short)(PLAYER_H->p.whole - t->h->p.whole + 48) < 96) {
            if (U16(o, 0x24) != 0) {
                r = 1;
            } else if (SPAD.s1fc & SPAD.s3c8) {
                r = 1;
                U16(o, 0x24) = 1;
                D_8009E4EA = 1;
                D_800A7C38 = 1;
            }
        }
    }
    return r;
}
#endif
