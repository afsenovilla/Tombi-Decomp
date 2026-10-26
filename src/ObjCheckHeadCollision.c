// FUNC 8003facc 196 MAIN0
// MATCHING 8003facc 196
// VERSION SLPS_01144 slps01144 8004069C 196
#include "camera.h"
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL main0 8003FACC (ObjCheckHeadCollision.c): -144 -> -136.
#include "TOBJ.H"
short probeCollisionAtDepthA(TObj *o, short a, short b);

static __inline__ short chk(TObj *o, int dx)
{
    return probeCollisionAtDepthA(o, o->h->p.whole + dx, o->y.p.whole - 0x15);
}

int ObjCheckHeadCollision(TObj *o)
{
    short r;
    TObj *s = o;
    if (o->y.p.whole < CAMERA_SCROLL.minY - 0x88) {
        o->y.p.whole = CAMERA_SCROLL.minY - 0x88;
        return 1;
    }
    if ((r = chk(s, 4)) != 0)
        return r;
    if ((r = chk(s, -4)) != 0)
        return r;
    return 0;
}
#else
#include "TOBJ.H"
short probeCollisionAtDepthA(TObj *o, short a, short b);

static __inline__ short chk(TObj *o, int dx)
{
    return probeCollisionAtDepthA(o, o->h->p.whole + dx, o->y.p.whole - 0x15);
}

int ObjCheckHeadCollision(TObj *o)
{
    short r;
    TObj *s = o;
    if (o->y.p.whole < CAMERA_SCROLL.minY - 0x90) {
        o->y.p.whole = CAMERA_SCROLL.minY - 0x90;
        return 1;
    }
    if ((r = chk(s, 4)) != 0)
        return r;
    if ((r = chk(s, -4)) != 0)
        return r;
    return 0;
}
#endif
