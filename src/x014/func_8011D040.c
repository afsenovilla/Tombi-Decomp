// FUNC 8011d040 248 X014
// MATCHING 8011d040 248
#include "TOBJ.H"

extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern short FUN_80042fbc(TObj *, TObj *);
extern void FUN_8004258c(TObj *, int);
extern void FUN_800eea7c(TObj *, int, int);

void func_8011D040(TObj *o, TObj *e)
{
    if (D_1F8001A4 != 0) return;
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (FUN_80042fbc(o, e) == 0) return;
    if (*(unsigned char *)&o->wac >= 2) {
        o->active = 2;
        o->animFrame = 1;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        FUN_8004258c(o, 1);
    } else {
        e->active = 2;
        e->b6a = 1;
        *(unsigned char *)&o->wac = 0;
        FUN_800eea7c(o, 0x10, 0);
        o->active = 6;
        o->b04 = 5;
        o->step = 0x65;
        o->state = 0;
    }
    D_1F80019E = 0;
}
