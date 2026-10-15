// FUNC 8011c80c 180 X014
// MATCHING 8011c80c 180
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
