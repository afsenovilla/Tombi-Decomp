// JP-only (x01), written from the disassembly
// VERSION SLPS_01144 x01 801245B0 828
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultE(TObj *, TObj *);
extern int resolveHitResultD(TObj *, TObj *);
extern void func_800EB894(int, int, int, int);

void func_801245B0_slps01144(TObj *o, TObj *e)
{
    TObj *q;
    int r;

    if (boxesOverlapSigned(o, e) < 0) return;
    if (e->b0c == 0) {
        r = resolveHitResultE(o, e);
        if (r) {
            if (r == 2) func_800EB894(0x1f4, e->a.p.whole, e->y.p.whole, e->b.p.whole);
            if (e->active != 3) {
                if (r == 2) e->active = 2;
                else e->active = 3;
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
        switch (resolveHitResultD(o, e)) {
        case 0:
            q = *(TObj **)&e->wa8;
            q->b68 = 0;
            q->b9e = 1;
            q->b9f = 0;
            break;
        case 2:
            if (e->active & 2) break;
            func_800EB894(0x1f4, e->a.p.whole, e->y.p.whole, e->b.p.whole);
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
    SPAD.s19e = 0;
}
#endif
