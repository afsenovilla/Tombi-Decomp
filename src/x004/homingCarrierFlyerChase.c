// FUNC 80127b14 576 X004
// MATCHING 80127b14 576
#include "spad.h"
#include "TOBJ.H"
#include "raw7.h"

typedef struct { short x, y; } P;

extern short COS_TABLE[], NEG_SIN_TABLE[];
extern short D_1F80016Ex[], D_1F80016Ey[]; /* debt: two more names for (*(short*)((char*)&SPAD.i16c + 2)) keep both reloads after the stores */
extern void *D_80134D88;
extern int angleBetweenPoints(P, P);
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);

static __inline__ int near(TObj *o)
{
    if ((unsigned short)(o->y.p.whole - (*(short*)((char*)&SPAD.i16c + 2)) + 0x20) > 0x40) return 0;
    return (unsigned short)(o->h->p.whole - (*(short*)((char*)&SPAD.i168 + 2)) + 0x10) <= 0x20;
}

void homingCarrierFlyerChase(TObj *o)
{
    P a, b;

    switch (o->state) {
    case 0:
        o->state++;
        a.x = o->h->p.whole;
        a.y = o->y.p.whole;
        b.x = (*(short*)((char*)&SPAD.i168 + 2));
        b.y = (*(short*)((char*)&SPAD.i16c + 2));
        o->d88 = angleBetweenPoints(a, b);
        if ((unsigned)(o->d88 - 4) < 0xbc)
            o->animFrame = 1;
        else
            o->animFrame = 0;
        o->timer = 0x20;
        o->velH = 0x80;
    case 1:
        AnimAdvance(o);
        o->h->raw += (COS_TABLE[(*(unsigned char*)&o->d88)] * o->velH) >> 4;
        o->y.raw += (NEG_SIN_TABLE[(*(unsigned char*)&o->d88)] * o->velH) >> 4;
        if (near(o)) {
            o->state++;
            o->anim = D_80134D88;
            AnimLoadDuration(o);
        } else if (--o->timer == -1) {
            o->state = 0;
        }
        break;
    case 2:
        o->h->p.whole = (*(short*)((char*)&SPAD.i168 + 2));
        o->y.p.whole = D_1F80016Ex[0];
        if (D_1F80016Ey[0] < -0x82) {
            o->step = 2;
            o->state = 0;
        }
        break;
    }
}
