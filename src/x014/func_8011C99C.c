// FUNC 8011c99c 196 X014
// MATCHING 8011c99c 196
/* Not a csv start: the csv piece func_8011CA00 is the tail of this function (0x8011C99C..0x8011CA5C). */
#include "TOBJ.H"

extern unsigned char D_8009C93F;
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short func_80042FBC(TObj *, TObj *);
extern void FUN_8004258c(void *, int);

static __inline__ void grab(TObj *o, TObj *p)
{
    o->active = 2;
    o->animFrame = p->h->p.whole > o->h->p.whole;
    o->b04 = 2;
    o->step = 0;
    o->state = 0;
    FUN_8004258c(o, 2);
}

void func_8011C99C(TObj *o, TObj *p)
{
    if (D_8009C93F) return;
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (func_80042FBC(o, p) == 0) return;
    if (D_1F8001A4) return;
    grab(o, p);
    D_1F80019E = 0;
}
