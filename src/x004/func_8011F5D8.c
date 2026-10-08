// FUNC 8011f5d8 240 X004
// MATCHING 8011f5d8 240
#include "TOBJ.H"
#include "raw7.h"

extern short D_1F80019E;
extern TObj *D_1F8003C0;

void func_8011F5D8(TObj *o, TObj *p)
{
    short d;
    int t;

    if ((unsigned short)(p->box2 + (U16(o, 0xea) - (unsigned short)p->y.p.whole)) > p->box2) return;
    t = U16(o, 0xe8) - (unsigned short)p->h->p.whole;
    d = t;
    if ((short)t > 0) {
        if (!(o->animFrame & 1)) return;
    } else {
        if (o->animFrame & 1) return;
        d = -t;
    }
    if ((unsigned short)(p->box0 - d) > 0x10) return;
    o->b9e = 9;
    if (o->animFrame & 1) {
        o->wb8 = p->box0;
    } else {
        o->wb8 = -p->box0;
    }
    o->wba = 10;
    o->velY = 0;
    D_1F80019E = 0;
    D_1F8003C0 = p;
}
