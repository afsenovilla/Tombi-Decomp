// FUNC 80115ea8 348 X019
// MATCHING 80115ea8 348
#include "spad.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void (*D_80079AD0[])(TObj *);
extern void func_80027810(TObj *);
extern int tryStartItemInteraction(TObj *, int, int);
extern void FUN_8002a4d0(TObj *);

void updateAreaScroll(TObj *o)
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
        } else if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x5a && (*(short*)((char*)&SPAD.i168 + 2)) < 0x11 && (*(short*)((char*)&SPAD.i16c + 2)) >= -0x4d) {
            tryStartItemInteraction(o, 1, 1);
        }
        break;
    case 2:
        FUN_8002a4d0(o);
        break;
    }
}
