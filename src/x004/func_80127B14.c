// FUNC 80127b14 576 X004
// MATCHING 80127b14 576
#include "TOBJ.H"
#include "raw7.h"

typedef struct { short x, y; } P;

extern short D_8007A5F0[], D_8007A3F0[];
extern short D_1F80016A, D_1F80016E;
extern short D_1F80016Ex[], D_1F80016Ey[]; /* debt: two more names for D_1F80016E keep both reloads after the stores */
extern void *D_80134D88;
extern int FUN_8002078c(P, P);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);

static __inline__ int near(TObj *o)
{
    if ((unsigned short)(o->y.p.whole - D_1F80016E + 0x20) > 0x40) return 0;
    return (unsigned short)(o->h->p.whole - D_1F80016A + 0x10) <= 0x20;
}

void func_80127B14(TObj *o)
{
    P a, b;

    switch (o->state) {
    case 0:
        o->state++;
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = D_1F80016A;
        b.y = D_1F80016E;
        o->d88 = FUN_8002078c(a, b);
        if ((unsigned)(o->d88 - 4) < 0xbc)
            o->animFrame = 1;
        else
            o->animFrame = 0;
        o->timer = 0x20;
        o->velH = 0x80;
    case 1:
        AnimAdvance(o);
        o->h->raw += (D_8007A5F0[U8(o, 0x88)] * o->velH) >> 4;
        o->y.raw += (D_8007A3F0[U8(o, 0x88)] * o->velH) >> 4;
        if (near(o)) {
            o->state++;
            o->anim = D_80134D88;
            AnimLoadDuration(o);
        } else if (--o->timer == -1) {
            o->state = 0;
        }
        break;
    case 2:
        o->h->p.whole = D_1F80016A;
        o->y.p.whole = D_1F80016Ex[0];
        if (D_1F80016Ey[0] < -0x82) {
            o->step = 2;
            o->state = 0;
        }
        break;
    }
}
