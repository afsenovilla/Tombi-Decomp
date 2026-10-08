// FUNC 8011fc84 156 X003
// MATCHING 8011fc84 156
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
extern short FUN_80042fbc(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int, int);

void func_8011FC84(TObj *o, TObj *p)
{
    int v;

    if (o->active & 2) return;
    if (D_1F8001A4 != 0) return;
    if (FUN_80042fbc(o, p) == 0) return;
    v = 2;
    o->active = v;
    o->animFrame = p->h->p.whole > o->h->p.whole;
    o->b04 = v;
    o->step = 0;
    o->state = 0;
    FUN_8004258c(o, 1, v);
}
