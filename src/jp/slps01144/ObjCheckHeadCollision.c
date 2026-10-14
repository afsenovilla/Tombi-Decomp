// JP slps01144 8004069C 196
// Japanese version of PAL main0 8003FACC (ObjCheckHeadCollision.c): -144 -> -136.
#include "TOBJ.H"
extern short D_800A4580;
short probeCollisionAtDepthA(TObj *o, short a, short b);

static __inline__ short chk(TObj *o, int dx)
{
    return probeCollisionAtDepthA(o, o->h->p.whole + dx, o->y.p.whole - 0x15);
}

int ObjCheckHeadCollision(TObj *o)
{
    short r;
    TObj *s = o;
    if (o->y.p.whole < D_800A4580 - 0x88) {
        o->y.p.whole = D_800A4580 - 0x88;
        return 1;
    }
    if ((r = chk(s, 4)) != 0)
        return r;
    if ((r = chk(s, -4)) != 0)
        return r;
    return 0;
}
