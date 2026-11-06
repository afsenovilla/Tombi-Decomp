// VERSION SLPS_01144 x14 8011A0AC 300
// FLAGS -O2 -G0 -fno-strength-reduce
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern unsigned short D_8009E50A;
extern short probeCollisionAtDepthB(TObj *, int, int);
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
typedef struct { char pad[0xac]; short wac; } X;

void func_8011A0AC_slps01144(TObj *o, TObj *p)
{
    if (o->active & 2) return;
    if (p->subtype == 1 && D_8009E50A != 7) {
        if (!probeCollisionAtDepthB(o, o->h->p.whole, (short)(o->y.p.whole + o->box2))) return;
        if (SPAD.s284 != ((((X *)p)->wac + 1) & 3)) return;
        o->active = 2;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        o->animFrame &= 1;
        decreaseObjectTimer(o, 1);
        SPAD.s19e = 0;
    } else {
        if (!boxesOverlap(o, p)) return;
        o->active = 2;
        o->animFrame = p->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 3);
        SPAD.s19e = 0;
    }
}
#endif
