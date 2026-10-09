// FUNC 80125b88 892 X001
// MATCHING 80125b88 892
#include "TOBJ.H"

extern short D_1F80019E;
extern short func_8004461C(TObj *, TObj *);
extern int FUN_80042c70(TObj *, TObj *);
extern int FUN_80042b98(TObj *, TObj *);
extern void FUN_8002b920(TObj *);
extern void FUN_800e9f74(int, int, int, int);

void func_80125B88(TObj *o, TObj *e)
{
    TObj *q;
    int r;

    if (func_8004461C(o, e) < 0) return;
    if (e->b0c == 0) {
        r = FUN_80042c70(o, e);
        if (r) {
            if (r == 2) FUN_800e9f74(0x1f4, e->a.p.whole, e->y.p.whole, e->b.p.whole);
            if (e->active != 3) {
                if (r == 2) {
                    e->active = 2;
                } else {
                    e->active = 3;
                    if (e->b04 != 2) FUN_8002b920(e);
                }
                e->animFrame = (o->animFrame & 1) + 2;
                e->b04 = 2;
                e->step = 0;
                e->state = 0;
                for (q = (TObj *)e->d94; q; q = (TObj *)q->d94) {
                    if (r == 2) q->active = 2;
                    else q->active = 3;
                    q->animFrame = (o->animFrame & 1) + 2;
                    q->b04 = 2;
                    q->step = 0;
                    q->state = 0;
                }
            }
        }
    } else {
        switch (FUN_80042b98(o, e)) {
        case 0:
            q = *(TObj **)&e->wa8;
            q->b68 = 0;
            q->b9e = 1;
            q->b9f = 0;
            break;
        case 2:
            if (e->active & 2) break;
            FUN_800e9f74(0x1f4, e->a.p.whole, e->y.p.whole, e->b.p.whole);
            q = *(TObj **)&e->wa8;
            e->active = 2;
            q->w98 = 0;
            e->animFrame = (o->animFrame & 1) + 2;
            e->b04 = 2;
            e->step = 0;
            e->state = 0;
            for (q = (TObj *)e->d90; q; q = (TObj *)q->d90) {
                q->active = 2;
                q->animFrame = (o->animFrame & 1) + 2;
                q->b04 = 2;
                q->step = 0;
                q->state = 0;
            }
            for (q = (TObj *)e->d94; q; q = (TObj *)q->d94) {
                q->active = 2;
                q->animFrame = (o->animFrame & 1) + 2;
                q->b04 = 2;
                q->step = 0;
                q->state = 0;
            }
            break;
        case 1:
            if (e->active & 2) break;
            q = *(TObj **)&e->wa8;
            e->active = 3;
            q->b68 = 1;
            q->b9e = 0;
            e->animFrame = (o->animFrame & 1) + 2;
            e->b04 = 2;
            e->step = 0;
            e->state = 0;
            for (q = (TObj *)e->d90; q; q = (TObj *)q->d90) {
                q->active = e->active;
                q->animFrame = (o->animFrame & 1) + 2;
                if (q->b0c == 0 && q->b04 != 2) FUN_8002b920(q);
                q->b04 = 2;
                q->step = 0;
                q->state = 0;
            }
            for (q = (TObj *)e->d94; q; q = (TObj *)q->d94) {
                q->active = e->active;
                q->animFrame = (o->animFrame & 1) + 2;
                q->b04 = 2;
                q->step = 0;
                q->state = 0;
            }
            break;
        }
    }
    D_1F80019E = 0;
}
