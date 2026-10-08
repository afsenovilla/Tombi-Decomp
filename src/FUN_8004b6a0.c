// FUNC 8004b6a0 292 MAIN0
// MATCHING 8004b6a0 292
#include "TOBJ.H"
typedef struct { TObj t; char c0[0x28]; short we8; short wea; } PO;
extern short D_1f8003bc;

int FUN_8004b6a0(PO *o, TObj *e)
{
    short d, ad, t;
    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 45) > 90)
        return 0;
    d = o->we8 - o->t.h->p.whole;
    ad = d;
    if (d < 0) ad = -d;
    d = o->we8 - e->h->p.whole;
    if (o->t.animFrame & 1) t = e->box0 + ad + d;
    else t = e->box0 + d;
    if ((unsigned short)t > e->box1 + ad)
        return 0;
    ad = e->box2 + (o->wea - e->y.p.whole);
    if ((unsigned short)ad > e->box3)
        return 0;
    if (!(o->t.animFrame & 1))
        D_1f8003bc = -e->box0;
    else
        D_1f8003bc = e->box1 - e->box0;
    return 1;
}
