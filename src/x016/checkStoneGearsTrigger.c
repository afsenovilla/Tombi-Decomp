// FUNC 801177b0 192 X016
// MATCHING 801177b0 192
#include "spad.h"
#include "gcd.h"
#include "gv6.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;

void checkStoneGearsTrigger(TObj *o)
{
    if (GCD.bce05 == 0xff) return;
    if ((unsigned short)((*(S2 **)&PLAYER.obj.h)->s2 - 0x82) >= 0x28) return;
    if (PLAYER.obj.y.p.whole != -0x3d) return;
    if (GVARS.b40 != 0 && GVARS.b41 == 0x3e) {
        o->step = 2;
        o->state = 0;
    } else if ((*(volatile unsigned short *)&GV6.s70 & 0x10) && (SPAD.s1fc & 0x2000)) {
        GVARS.b3e = 1;
        o->step = 1;
        o->state = 0;
    }
}
