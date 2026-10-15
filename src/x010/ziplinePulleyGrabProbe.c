// FUNC 8011df8c 116 X010
// MATCHING 8011df8c 116
#include "spad.h"
#include "TOBJ.H"

extern short FUN_8004b57c(TObj *, TObj *);

void ziplinePulleyGrabProbe(TObj *o, TObj *e)
{
    e->b6a = 0;
    if (FUN_8004b57c(o, e)) {
        o->b9e = 4;
        o->wba = 0;
        o->velY = 0;
        o->wb8 = (*(unsigned short*)&SPAD.s3bc);
        e->b6a = 1;
        SPAD.s19e = 0;
        (*(TObj **)&SPAD.p3c0) = e;
    }
}
