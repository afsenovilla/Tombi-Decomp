// FUNC 80124348 140 X001
// MATCHING 80124348 140
#include "TOBJ.H"
extern short objectBoxOverlapSideFacing(TObj *, TObj *);
extern void printInfoMessage(int, int);

void puppyBaronContact(TObj *p, TObj *o)
{
    short r = objectBoxOverlapSideFacing(p, o);
    if (r != -1 && r < 3) {
        o->active = 2;
        o->b04 = 2;
        o->b6a = 0;
        o->step = 3;
        o->state = 0;
        if (o->subtype == 0)
            printInfoMessage(0x12, 2);
        o->animFrame = p->animFrame & 1;
    }
}
