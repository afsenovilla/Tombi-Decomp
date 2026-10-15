// FUNC 80126fe4 144 X001
// MATCHING 80126fe4 144
#include "spad.h"
#include "TOBJ.H"
extern short FUN_8004b57c(TObj *, TObj *);

void springVineGrabProbe(TObj *o, TObj *e)
{
    if (e->subtype == 0 && FUN_8004b57c(o, e)) {
        o->b9e = 4;
        o->wba = 0;
        o->velY = 0;
        SPAD.s19e = 0;
        o->wb8 = (*(unsigned short*)&SPAD.s3bc);
        e->b6a = 1;
        (*(TObj **)&SPAD.p3c0) = e;
        e->animFrame = (o->animFrame & 1) << 1;
    }
}
