// near-twin of PAL baccusVillagePigShotContact (tools/jp_autofix.py)
// VERSION SLPS_01144 x14 80119E84 144
// FLAGS -O2 -G0
#if defined(VERSION_SLPS_01144)
#include "spad.h"
#include "TOBJ.H"
extern short boxesOverlap(TObj *, TObj *);
extern void decreaseObjectTimer(TObj *, int);
void func_80119E84(TObj *o, TObj *e)
{
    if (!(o->active & 2) && boxesOverlap(o, e)) {
        o->active = 2;
        o->animFrame = e->h->p.whole > o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        decreaseObjectTimer(o, 2);
        SPAD.s19e = 0;
    }
}
#endif
