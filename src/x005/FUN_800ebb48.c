// FUNC 800ebb48 284 X005
// MATCHING 800ebb48 284
#include "TOBJ.H"
extern void FUN_800187e4(void);

void FUN_800ebb48(TObj *o)
{
    TObj *a = (TObj *)o->d90;
    TObj *b = (TObj *)o->d94;
    int s;
    switch (o->b04) {
    case 0:
        if ((a->active != 0 && a->b04 == 1) || a->b04 == 2) {
            b->a.p.whole = a->a.p.whole;
            b->y.p.whole = a->y.p.whole;
            b->b.p.whole = a->b.p.whole;
            s = a->state;
            if (s < 7) if (s > 1) {
                if (b->active != 0) return;
                if (b->b04 == 1) return;
                goto L;
            }
        }
        if (b->active != 0 && b->b04 == 1) {
            b->b68 = 1;
            b->animFrame = 2;
        }
    L:
        o->b04 = o->b04 + 1;
        break;
    case 1:
        FUN_800187e4();
        break;
    }
}
