// FUNC 8011c99c 196 X014
// MATCHING 8011c99c 196
#include "spad.h"
#include "gvars.h"
/* Not a csv start: the csv piece func_8011CA00 is the tail of this function (0x8011C99C..0x8011CA5C). */
#include "TOBJ.H"

extern short func_80042FBC(TObj *, TObj *);
extern void decreaseObjectTimer(void *, int);

static __inline__ void grab(TObj *o, TObj *p)
{
    o->active = 2;
    o->animFrame = p->h->p.whole > o->h->p.whole;
    o->b04 = 2;
    o->step = 0;
    o->state = 0;
    decreaseObjectTimer(o, 2);
}

void func_8011C99C(TObj *o, TObj *p)
{
    if (GVARS.b3f) return;
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (func_80042FBC(o, p) == 0) return;
    if (SPAD.b1a4) return;
    grab(o, p);
    SPAD.s19e = 0;
}
