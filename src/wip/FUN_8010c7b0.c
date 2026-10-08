// FUNC 8010c7b0 292 X000
#include "TOBJ.H"
extern TObj *DAT_8009f0ec;
extern TObj *FUN_8004baa8(TObj *, TObj *);
extern void FUN_800ee428(TObj *);

void FUN_8010c7b0(TObj *o)
{
    TObj *n;
    int h, t, f;
    Fix16 *p;
    f = o->animFrame & 1;
    h = o->h->p.whole;
    t = h - 6; if (!f) t = h + 6;
    *(short *)((char *)o + 0xe8) = t;
    *(short *)((char *)o + 0xea) = o->y.p.whole - 8;
    n = FUN_8004baa8(o, DAT_8009f0ec);
    if (n == 0) {
        *(signed char *)&o->b0f = -8;
        p = o->h;
        f = o->animFrame & 1;
        h = p->p.whole;
        t = h + 8; if (!f) t = h - 8;
        p->p.whole = t;
        o->velX = 0;
        o->velY = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->b9e = 0;
        *(char *)&o->waa = 0;
        *(char *)&o->wac = 1;
        FUN_800ee428(o);
        o->step = 2;
        o->state = 3;
    } else {
        o->h->p.whole = n->h->p.whole + o->wb8;
        DAT_8009f0ec = n;
        o->y.p.whole = n->y.p.whole + o->wba + 8;
        if (o->b9e == 11) {
            *(signed char *)&o->b0f = -8;
            o->step = 11;
            o->state = 0;
        }
    }
}
