// JP x16 80116B60 192
// Japanese version of PAL x016 801177B0 (x016/func_801177B0.c): 8192 -> 32, 16 -> 4096.
#include "spad.h"
#include "gcd.h"
#include "gv6.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;

void func_801177B0(TObj *o)
{
    if (GCD.bce05 == 0xff) return;
    if ((unsigned short)((*(S2 **)&PLAYER.obj.h)->s2 - 0x82) >= 0x28) return;
    if (PLAYER.obj.y.p.whole != -0x3d) return;
    if (GVARS.b40 != 0 && GVARS.b41 == 0x3e) {
        o->step = 2;
        o->state = 0;
    } else if ((*(volatile unsigned short *)&GV6.s70 & 0x1000) && (SPAD.s1fc & 0x20)) {
        GVARS.b3e = 1;
        o->step = 1;
        o->state = 0;
    }
}
