// FUNC 8011ce64 200 X014
// MATCHING 8011ce64 200
#include "TOBJ.H"
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short FUN_80042fbc(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);

void func_8011CE64(TObj *o, TObj *e)
{
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (e->subtype == 0) return;
    if (FUN_80042fbc(o, e) == 0) return;
    e->b69 = 1;
    e->active = 2;
    e->b04 = 2;
    e->step = 0;
    e->state = 0;
    if (D_1F8001A4 != 0) return;
    o->active = 2;
    o->animFrame = 1;
    o->b04 = 2;
    o->step = 0;
    o->state = 0;
    FUN_8004258c(o, 1);
    D_1F80019E = 0;
}
