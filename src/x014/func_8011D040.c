// FUNC 8011d040 248 X014
// MATCHING 8011d040 248
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
extern void FUN_800eea7c(TObj *, int, int);

void func_8011D040(TObj *o, TObj *e)
{
    if (SPAD.b1a4 != 0) return;
    if (*(unsigned char *)&o->wac == 2) return;
    if (o->active & 2) return;
    if (boxesOverlap(o, e) == 0) return;
    if (*(unsigned char *)&o->wac >= 2) {
        o->active = 2;
        o->animFrame = 1;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 1);
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
    SPAD.s19e = 0;
}
