// FUNC 80126f64 128 X001
// MATCHING 80126f64 128
#include "spad.h"
#include "TOBJ.H"

extern short D_1F8003BC;
extern short FUN_8004b57c(TObj *, TObj *);

void flowerTowerBlockerGrabProbe(TObj *o, TObj *e)
{
    if (e->subtype && FUN_8004b57c(o, e)) {
        o->b9e = 4;
        o->wba = 0;
        o->velY = 0;
        SPAD.s19e = 0;
        o->wb8 = D_1F8003BC;
        e->b68 = 1;
        (*(TObj **)&SPAD.p3c0) = e;
    }
}
