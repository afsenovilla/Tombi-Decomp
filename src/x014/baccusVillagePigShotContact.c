// FUNC 8011c80c 180 X014
// MATCHING 8011c80c 180
// VERSION SLPS_01144 x14 80119D3C 144
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
void func_80119D3C(TObj *o, TObj *e)
{
    if (!(o->active & 2) && boxesOverlap(o, e)) {
        o->active = 2;
        o->animFrame = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 2);
        SPAD.s19e = 0;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void baccusVillagePigShotContact(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac != 2 && !(o->active & 2) && boxesOverlap(o, e) && SPAD.b1a4 == 0) {
        o->active = 2;
        o->animFrame = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 2);
        SPAD.s19e = 0;
    }
}
#endif
