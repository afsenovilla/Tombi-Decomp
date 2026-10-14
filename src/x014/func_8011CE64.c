// FUNC 8011ce64 200 X014
// MATCHING 8011ce64 200
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);

void func_8011CE64(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (e->subtype == 0) return;
    if (boxesOverlap(o, e) == 0) return;
    e->b69 = 1;
    e->active = 2;
    e->b04 = 2;
    e->step = 0;
    e->state = 0;
    if (SPAD.b1a4 != 0) return;
    o->active = 2;
    o->animFrame = 1;
    o->b04 = 2;
    o->step = 0;
    o->state = 0;
    decreaseObjectTimer(o, 1);
    SPAD.s19e = 0;
}
