// FUNC 8012e44c 264 X004
// MATCHING 8012e44c 264
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"


void func_8012E44C(TObj *o)
{
    if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) != 1) return;
    if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - 0x370) < 0x28 && PLAYER.obj.y.p.whole == -0x1d6) {
        if (GCD.bcddb == 0) {
            o->step = 1;
            o->state = 0;
        } else if (GCD.bd0d9 != 0) {
            o->step = 1;
            o->state = 2;
        }
    }
    if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - 0x3c8) < 0x28 && PLAYER.obj.y.p.whole == -0x2e4) {
        if (GCD.bcdd7 == 0) {
            o->step = 2;
            o->state = 0;
        } else if (GCD.bd0da != 0) {
            o->step = 2;
            o->state = 2;
        }
    }
}
