// FUNC 8011cf2c 196 X014
// MATCHING 8011cf2c 196
#include "TOBJ.H"
extern short FUN_80042fbc(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);
extern unsigned char D_1F8001A4;
extern short D_1F80019E;

void func_8011CF2C(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac != 2 && !(o->active & 2) && FUN_80042fbc(o, e) && D_1F8001A4 == 0) {
        int oh, eh;
        o->active = 2;
        eh = e->h->p.whole;
        oh = o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = oh < eh;
        FUN_8004258c(o, 2);
        e->active = 2;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        D_1F80019E = 0;
    }
}
