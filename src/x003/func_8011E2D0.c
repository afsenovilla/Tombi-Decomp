// FUNC 8011e2d0 136 X003
// MATCHING 8011e2d0 136
#include "TOBJ.H"
extern int ObjCheckHeadCollision(TObj *);
extern void playerSetFallLean(TObj *);

static __inline__ void fall(TObj *o)
{
    o->b9c = 2;
    *(char *)&o->wac = 1;
    playerSetFallLean(o);
    o->step = 2;
    o->state = 3;
}

void func_8011E2D0(TObj *o)
{
    if (ObjCheckHeadCollision(o)) {
        fall(o);
    }
    if (o->velY > 0) {
        fall(o);
    }
}
