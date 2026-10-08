// FUNC 8004b454 296 MAIN0
// MATCHING 8004b454 296
#include "TOBJ.H"
typedef struct { TObj t; char c0[0x28]; short we8; short wea; } PO;
typedef struct E { unsigned short a, b; } E;
extern E DAT_8007b5e4[];
extern short D_1F80019E;
extern TObj *D_1F8003C0;

void func_8004B454(PO *o, TObj *e)
{
    short d, ad, t;
    unsigned short ox, oy;
    char pad[8];
    if ((unsigned short)(o->t.d->p.whole - e->d->p.whole + 45) > 90)
        return;
    d = o->we8 - o->t.h->p.whole;
    ox = DAT_8007b5e4[e->b0c].a;
    oy = DAT_8007b5e4[e->b0c].b;
    ad = d;
    if (d < 0) ad = -d;
    d = o->we8 - (e->h->p.whole + ox);
    if (o->t.animFrame & 1) t = e->box0 + ad + d;
    else t = e->box0 + d;
    if ((unsigned short)t > e->box1 + ad)
        return;
    ad = e->box2 + (o->wea - (e->y.p.whole + oy));
    if ((unsigned short)ad > e->box3)
        return;
    o->t.b9e = 3;
    o->t.velY = 0;
    o->t.wb8 = ox;
    o->t.wba = 0;
    e->b69 = 2;
    D_1F80019E = 0;
    D_1F8003C0 = e;
}
