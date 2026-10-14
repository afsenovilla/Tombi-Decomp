// FUNC 8011cf2c 196 X014
// MATCHING 8011cf2c 196
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void func_8011CF2C(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac != 2 && !(o->active & 2) && boxesOverlap(o, e) && SPAD.b1a4 == 0) {
        int oh, eh;
        o->active = 2;
        eh = e->h->p.whole;
        oh = o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = oh < eh;
        decreaseObjectTimer(o, 2);
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        SPAD.s19e = 0;
    }
}
