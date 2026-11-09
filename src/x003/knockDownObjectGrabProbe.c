// FUNC 8011f474 148 X003
// MATCHING 8011f474 148
#include "spad.h"
#include "TOBJ.H"
extern short probeGrabPointInBoxB(TObj *, TObj *);
extern short LIST_COUNTER;

void knockDownObjectGrabProbe(TObj *o, TObj *p)
{
    TObj *q;
    if (probeGrabPointInBoxB(o, p)) {
        o->b9e = 1;
        o->wb8 = SPAD.s3bc;
        o->wba = -((short)(unsigned short)p->box2 >> 1);
        o->velY = 0;
        q = (TObj *)p->movetab;
        q->b69 = 1;
        LIST_COUNTER = 0;
        (*(TObj **)&SPAD.p3c0) = p;
        q->animFrame = o->animFrame & 1;
    }
}
