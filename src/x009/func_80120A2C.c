// FUNC 80120a2c 372 X009
// MATCHING 80120a2c 372
#include "TOBJ.H"

extern short D_1F80019E;
extern short FUN_8004461c(TObj *a, TObj *b);
extern int FUN_80042b98(TObj *, TObj *);
extern void FUN_800e9f74(int, int, int, int);
extern TObj *FUN_800183b8(void);

void func_80120A2C(TObj *o, TObj *p)
{
    int r;
    unsigned short t;
    TObj *n;

    if (FUN_8004461c(o, p) == -1) return;
    p->w7a = o->animFrame & 1;
    r = FUN_80042b98(o, p);
    if (r != 0) {
        if (r == 1) {
            if (!(p->active & 2)) {
                p->active = 3;
                p->b04 = 2;
                p->step = 0;
                p->state = 0;
            }
        } else if ((unsigned short)p->wb4 != 3) {
            if (o->type == 1) if (o->subtype == 4) if (o->b0d | p->b0d) {
                n = FUN_800183b8();
                if (n) {
                    n->active = 1;
                    n->type = 0x1c;
                    n->subtype = 4;
                    n->b0c = 4;
                    n->a.p.whole = p->a.p.whole;
                    n->y.p.whole = p->y.p.whole;
                    n->b.p.whole = p->b.p.whole;
                }
            }
            FUN_800e9f74(0x1f4, p->a.p.whole, p->y.p.whole, p->b.p.whole);
            p->active = 2;
            t = o->animFrame;
            p->b04 = 2;
            p->step = 2;
            p->state = 0;
            p->animFrame = ~t & 1;
        }
    }
    D_1F80019E = 0;
}
