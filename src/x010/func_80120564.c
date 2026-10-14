// FUNC 80120564 400 X010
// MATCHING 80120564 400
#include "spad.h"
#include "TOBJ.H"

extern short boxesOverlapFlipAware(TObj *, TObj *);
extern int resolveHitResultD(TObj *, TObj *);
extern int resolveHitResultB(TObj *, TObj *);
extern void resolveHitResultG(TObj *, TObj *);
extern void awardAPByArea(int, int, int, int);
extern void FUN_8001f96c(int, int, int, int);

void func_80120564(TObj *o, TObj *p)
{
    int r;
    unsigned short t;

    if (boxesOverlapFlipAware(o, p) == -1) return;
    if (p->subtype == 2) {
        if (resolveHitResultD(o, p) == 0) goto end;
        goto hit;
    } else if (*(unsigned short *)&p->wb4 != 0) {
        r = resolveHitResultB(o, p);
        if (r != 0) {
            if (r >= 6) {
                awardAPByArea(0x1f4, p->a.p.whole, p->y.p.whole, p->b.p.whole);
                p->active = 2;
                p->animFrame = ~o->animFrame & 1;
                p->b04 = 2;
                p->step = 2;
                p->state = 0;
            } else if (p->b9c) {
                p->b68 = 0;
                FUN_8001f96c(1, o->a.p.whole, o->y.p.whole, o->b.p.whole);
            } else {
            hit:
                t = o->animFrame;
                p->active = 3;
                p->b04 = 2;
                p->step = 0;
                p->state = 0;
                p->w7a = t & 1;
            }
        }
    } else {
        p->w7a = o->animFrame & 1;
        resolveHitResultG(o, p);
        switch (o->type) { case 5: case 6: case 7:
            o->wa8 = 0x4ff;
        }
        if (p->b9c && p->w98) {
            p->b68 = 0;
        }
    }
end:
    SPAD.s19e = 0;
}
