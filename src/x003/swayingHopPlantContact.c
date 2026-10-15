// FUNC 8011fc84 156 X003
// MATCHING 8011fc84 156
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int, int);

void swayingHopPlantContact(TObj *o, TObj *p)
{
    int v;

    if (o->active & 2) return;
    if (SPAD.b1a4 != 0) return;
    if (boxesOverlap(o, p) == 0) return;
    v = 2;
    o->active = v;
    o->animFrame = p->h->p.whole > o->h->p.whole;
    o->b04 = v;
    o->step = 0;
    o->state = 0;
    decreaseObjectTimer(o, 1, v);
}
