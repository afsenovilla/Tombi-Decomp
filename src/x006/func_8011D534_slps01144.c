// near-twin of pitchforkPigContact (PAL)
// VERSION SLPS_01144 x06 8011D534 332
#if defined(VERSION_SLPS_01144)
extern unsigned char D_8009E4E2;
extern unsigned char D_8009E4F4[4];
extern short D_1F80019E;
#define spawnImpactEffect func_800208E4
#include "spad.h"
#include "gvars.h"
#include "TOBJ.H"
typedef struct { TObj t; char pc0[0xd0 - 0xc0]; short wd0; } TD;
extern void spawnImpactEffect(int, int, int, int);

static __inline__ int ovl(TObj *a, TObj *b)
{
    if ((unsigned short)(a->d->p.whole - b->d->p.whole + 0x14) >= 0x29) {
        return 0;
    }
    if ((unsigned short)(a->h->p.whole - b->h->p.whole + (a->box0 + b->box0)) > a->box1 + b->box1) {
        return 0;
    }
    return (unsigned short)(a->y.p.whole - b->y.p.whole + (a->box2 + b->box2)) <= a->box3 + b->box3;
}

void func_8011D534_slps01144(TObj *o, TObj *e)
{
    TD *p;

    if (D_8009E4E2 && ovl(o, e)) {
        int eh, oh;
        e->active = 2;
        eh = e->h->p.whole;
        oh = o->h->p.whole;
        e->b04 = 2;
        e->step = 0;
        e->state = 0;
        e->animFrame = eh < oh;
        spawnImpactEffect(0, e->a.p.whole, e->y.p.whole, e->b.p.whole);
        p = (*(TD **)&D_8009E4F4);
        p->t.step = 4;
        p->t.state = 0;
        p->wd0 -= 0x10;
        if (p->wd0 < 0) {
            p->wd0 = 0;
        }
        D_1F80019E = 0;
    }
}
#endif
