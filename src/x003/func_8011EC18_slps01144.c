// near-twin of swayingHopPlantContact (PAL)
// VERSION SLPS_01144 x03 8011EC18 136
#if defined(VERSION_SLPS_01144)
#define boxesOverlap func_8004E3B0
#define decreaseObjectTimer func_8004D980
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int, int);

void func_8011EC18_slps01144(TObj *o, TObj *p)
{
    int v;

    if (o->active & 2) return;
    if (boxesOverlap(o, p) == 0) return;
    v = 2;
    o->active = v;
    o->animFrame = p->h->p.whole > o->h->p.whole;
    o->b04 = v;
    o->step = 0;
    o->state = 0;
    decreaseObjectTimer(o, 1, v);
}
#endif
