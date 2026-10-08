// FUNC 8010c7b0 292 X011
// MATCHING 8010c7b0 292
#include "TOBJ.H"
typedef struct { TObj b; char pad[0xe8 - 0xc0]; short e8, ea; } OX;
extern TObj *DAT_8009f0ec;
extern TObj *FUN_8004baa8(TObj *, TObj *);
extern void FUN_800ee428(TObj *);

void FUN_8010c7b0(TObj *o)
{
    TObj *n;
    int h, t, f;
    Fix16 *p;
    {
        int t1, h1;
        t1 = o->animFrame & 1;
        h1 = o->h->p.whole;
        if (t1) t1 = h1 - 6; else t1 = h1 + 6;
        ((OX *)o)->e8 = t1;
    }
    ((OX *)o)->ea = o->y.p.whole - 8;
    n = FUN_8004baa8(o, DAT_8009f0ec);
    if (n == 0) {
        *(signed char *)&o->b0f = -8;
        p = o->h;
        {
            int t2, h2;
            t2 = o->animFrame & 1;
            h2 = p->p.whole;
            if (t2) t2 = h2 + 8; else t2 = h2 - 8;
            p->p.whole = t2;
        }
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
