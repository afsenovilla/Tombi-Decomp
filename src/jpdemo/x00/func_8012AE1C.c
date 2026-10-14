// JPDEMO x00 8012AE1C 260
// Demo version of PAL x000 80136BC0 (func_80136BC0.c): fixed pad bit 0x20, no D_8009D2B0 test.
#include "spad.h"
#include "player.h"
typedef struct Q { char p0[0x16]; short y; } Q;
typedef struct R { char p0[2]; unsigned short w2; } R;
typedef struct O { char p0[0x1c]; unsigned short w1c; char p1[2]; Q *q; } O;
extern signed char D_8009D2B0;
extern char D_800A60F8;

int func_80136BC0(O *o)
{
    int r = 0;
    if (o->q->y < -500) {
        if (PLAYER.obj.y.p.whole != -0x2e3)
            goto end;
        if ((short)(*(R **)&PLAYER.obj.h)->w2 < 0xbf9)
            goto end;
        if (o->w1c) {
            r = 1; goto end2;
        }
    } else {
        if (PLAYER.obj.b04 == 5)
            goto end;
        if (PLAYER.obj.y.p.whole < -0x12b)
            goto end;
        if ((unsigned short)((*(R **)&PLAYER.obj.h)->w2 - 0xa1a) >= 0x28)
            goto end;
        if (o->w1c) {
            r = 1;
            goto end;
        }
    }
    if (SPAD.s1fc & 0x20) {
        r = 1;
        D_800A60F8 = 0;
        o->w1c = 1;
    }
end:
    return r;
end2:
    return r;
}
