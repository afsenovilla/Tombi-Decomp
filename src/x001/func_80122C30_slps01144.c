// near-twin of PAL komaPigContact (tools/jp_autofix.py)
// VERSION SLPS_01144 x01 80122C30 516
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"

typedef struct { TObj o; char pad[0x14]; TObj *d4; } TObjX;
extern short objectBoxOverlapSideFacing(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
extern void FUN_8001e4f0(int);
extern void awardAPByArea(int, int, int, int);

void func_80122C30(TObj *o, TObj *e)
{
    short r;

    r = objectBoxOverlapSideFacing(o, e);
    if (r == -1) return;
    switch (*(unsigned char *)&o->wac) {
    case 1:
        if (r < 3) {
            if (e->subtype == 8) {
                if ((*(int *)&e->b04 & 0xffffff) == 0x80301 && e->substep < 4) {
                    e->step = 3;
                    e->state = 0;
                    goto common;
                }
                if (e->b0c == 0) (*(short *)e->d94)--;
                e->subtype = 0;
                e->step = 1;
                e->state = 0;
                e->b69 = 0;
                goto common;
            }
            e->step = 1;
            e->state = 0;
            e->b69 = 0;
        common:
            e->active = 4;
            e->b04 = 2;
            e->animFrame = o->animFrame & 1;
            ((TObjX *)o)->d4 = e;
            *(unsigned char *)&o->wac = 2;
            break;
        }
    case 0:
    case 3:
        if (e->active & 2) break;
        e->b69 = 8;
        if (o->active & 2) break;
        o->active = 2;
        r = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame = r;
        decreaseObjectTimer(o, 1);
        break;
    case 2:
        FUN_8001e4f0(7);
        awardAPByArea(0x1f4, e->a.p.whole, e->y.p.whole, e->b.p.whole);
        r = o->h->p.whole > e->h->p.whole;
        e->active = 2;
        e->b04 = 2;
        e->step = 2;
        e->state = 0;
        e->b69 = 0;
        e->w7a = r;
        break;
    }
}
#endif
