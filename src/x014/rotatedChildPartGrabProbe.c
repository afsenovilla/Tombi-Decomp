// FUNC 8011d3f8 260 X014
// MATCHING 8011d3f8 260
#include "spad.h"
#include "TOBJ.H"
typedef struct {
    TObj t;
    char pad[0xe8 - 0xc0];
    unsigned short e8;
    unsigned short ea;
} P;

void rotatedChildPartGrabProbe(P *o, TObj *e)
{
    short d;
    unsigned short x;
    unsigned short w;

    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 45) < 91) {
        d = o->e8 - o->t.h->p.whole;
        if (d < 0) d = -d;
        w = o->e8 - e->h->p.whole;
        if (o->t.animFrame & 1)
            x = e->box0 + d + w;
        else
            x = e->box0 + w;
        if (x > e->box1 + d) return;
        d = e->box2 + (o->ea - e->y.p.whole);
        if ((unsigned short)d <= e->box3) {
            o->t.b9e = 3;
            o->t.velY = 0;
            o->t.wb8 = 0;
            o->t.wba = 0;
            e->b69 = 2;
            SPAD.s19e = 0;
            (*(TObj **)&SPAD.p3c0) = e;
        }
    }
}
