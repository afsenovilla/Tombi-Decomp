// FUNC 8011f474 148 X003
// MATCHING 8011f474 148
#include "spad.h"
#include "TOBJ.H"
extern short FUN_8004b6a0(TObj *, TObj *);
extern short D_1F80019E;

void func_8011F474(TObj *o, TObj *p)
{
    TObj *q;
    if (FUN_8004b6a0(o, p)) {
        o->b9e = 1;
        o->wb8 = SPAD.s3bc;
        o->wba = -((short)(unsigned short)p->box2 >> 1);
        o->velY = 0;
        q = (TObj *)p->movetab;
        q->b69 = 1;
        D_1F80019E = 0;
        (*(TObj **)&SPAD.p3c0) = p;
        q->animFrame = o->animFrame & 1;
    }
}
