// FUNC 80115ea8 348 X019
// MATCHING 80115ea8 348
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern short D_1F800172, D_1F80016A, D_1F80016E;
extern void (*D_80079AD0[])(TObj *);
extern void func_80027810(TObj *);
extern int tryStartItemInteraction(TObj *, int, int);
extern void FUN_8002a4d0(TObj *);

void func_80115EA8(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GVARS.b3a) o->step++;
        ((Fix16 *)o->d38)->p.whole = 0;
        func_80027810(o);
        break;
    case 1:
        D_80079AD0[o->subtype](o);
        if (PLAYER.obj.a.p.whole < 0x1e && PLAYER.obj.y.p.whole >= -0x3b) {
            tryStartItemInteraction(o, 1, 0);
        } else if (D_1F800172 == 0x5a && D_1F80016A < 0x11 && D_1F80016E >= -0x4d) {
            tryStartItemInteraction(o, 1, 1);
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
