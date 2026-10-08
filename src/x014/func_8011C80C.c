// FUNC 8011c80c 180 X014
// MATCHING 8011c80c 180
#include "TOBJ.H"
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short FUN_80042fbc(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_8011C80C(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac != 2 && !(o->active & 2) && FUN_80042fbc(o, e) && D_1F8001A4 == 0) {
        o->active = 2;
        o->animFrame = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        FUN_8004258c(o, 2);
        D_1F80019E = 0;
    }
}
