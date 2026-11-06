// VERSION SLPS_01144 x03 80131064 396
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern TObj *D_800A0E68, *D_800A0E6C;
extern Fix16 *PLAYER_H;
extern unsigned char D_8009E4E7, D_8009E4EA, D_8009EE58, D_800A7B7C, D_800A7B7D, D_800A7B7E;
extern unsigned char D_800A7BE1, D_8009EA97;
extern short PLAYER_ANIM_FRAME;
extern void func_80127810_slps01144(TObj *, int, int);

void func_80131064_slps01144(TObj *o)
{
    TObj *p = D_800A0E68;
    TObj *q = D_800A0E6C;
    register int two asm("v0");

    switch (o->state) {
    case 0:
        if (PLAYER_H->p.whole >= 441) {
            D_8009E4E7 = 1;
            D_8009E4EA = 1;
            D_8009EE58 = 0;
            PLAYER_H->p.whole = 440;
            asm("");
            PLAYER_ANIM_FRAME = 0;
            D_800A7B7C = 5;
            D_800A7B7D = 0;
            D_800A7B7E = 0;
            o->state++;
        }
        break;
    case 1:
        if (D_800A7BE1) {
            D_8009EA97 = 1;
            func_80127810_slps01144(p, 0, 0);
            func_80127810_slps01144(q, 0, 0);
            o->state++;
        }
        break;
    case 2:
        asm("" : "=r"(two));
        if (D_8009EA97 == two) {
            o->w08 = 60;
            o->state++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            D_8009EA97 = 4;
            o->step = 1;
            o->state = 0;
        }
        break;
    }
}
#endif
