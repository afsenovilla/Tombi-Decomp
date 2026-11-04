// FUNC 8011e2d0 136 X003
// MATCHING 8011e2d0 136
// VERSION SLPS_01144 x03 8011D1B8 192
#if defined(VERSION_SLPS_01144)
#include "TOBJ.H"
extern int ObjCheckHeadCollision(TObj *);
static __inline__ void fall(TObj *o)
{
    int a = 16;
    unsigned short s = o->animFrame;
    o->b9c = 2;
    *(char *)&o->wac = 1;
    o->d84 = 0;
    if (s & 1)
        a = 0xf0;
    o->timer = 10;
    o->d88 = a;
    o->d8c = 0;
    o->step = 2;
    o->state = 3;
}
void func_8011D1B8(TObj *o)
{
    if (ObjCheckHeadCollision(o)) {
        fall(o);
    }
    if (o->velY > 0) {
        fall(o);
    }
}
#else
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

void playerFallIfHeadHit(TObj *o)
{
    if (ObjCheckHeadCollision(o)) {
        fall(o);
    }
    if (o->velY > 0) {
        fall(o);
    }
}
#endif
