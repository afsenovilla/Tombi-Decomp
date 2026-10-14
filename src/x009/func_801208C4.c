// FUNC 801208c4 360 X009
// MATCHING 801208c4 360
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
extern short func_80121630(TObj *, short, short, unsigned char);
extern int resolveHitResultA(TObj *, TObj *);

void func_801208C4(TObj *o, TObj *e)
{
    short t;
    short u;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    u = o->h->p.whole - e->h->p.whole + (o->box0 + e->box0);
    if ((unsigned short)u >= e->box1 + o->box1) return;
    if ((unsigned short)(o->y.p.whole - e->y.p.whole + (e->box2 + o->box2)) >= e->box3 + o->box3) return;
    u = o->h->p.whole - (e->h->p.whole - e->box0);
    if ((unsigned short)u >= e->box1) return;
    t = o->y.p.whole - (e->y.p.whole + (e->box3 - e->box2));
    if (func_80121630(o, u, t, e->subtype) == 0) return;
    if (resolveHitResultA(o, e) == 0 || o->type == 9) {
        if (SPAD.s282 & 2) {
            o->b69 = 1;
            GVARS.i34 = 0;
        }
    }
    SPAD.s19e = 0;
}
