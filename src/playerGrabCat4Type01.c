// FUNC 801269d0 148 X000
// MATCHING 801269d0 148
#include "TOBJ.H"
extern short probeGrabPointInBoxB(void);
extern short GRAB_POINT_OFFSET;
extern short LIST_COUNTER;
extern TObj *TOUCHED_OBJECT;

void playerGrabCat4Type01(TObj *o, TObj *t)
{
    if (*(int *)((char *)t + 0x94) == 0 && probeGrabPointInBoxB() != 0) {
        TObj *m;
        short s;
        o->b9e = 1;
        s = GRAB_POINT_OFFSET;
        o->wba = 0xc;
        o->velY = 0;
        o->wb8 = s;
        m = t->movetab;
        m->b69 = 1;
        LIST_COUNTER = 0;
        TOUCHED_OBJECT = t;
        m->animFrame = o->animFrame & 1;
    }
}
