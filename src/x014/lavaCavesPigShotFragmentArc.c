// FUNC 80123aac 696 X014
// MATCHING 80123aac 696
// VERSION SLPS_01144 x14 80120D84 696
#include "camera.h"
#if defined(VERSION_SLPS_01144)
// Japanese version of PAL x014 80123AAC (x014/func_80123AAC.c): 128 -> 120.
#include "spad.h"
#include "TOBJ.H"
extern unsigned short D_801266D4[];
extern void FUN_800202b4(TObj *);
extern short probeSlopeWallSideAt(TObj *, int, int);
extern short probeFloorPlaneAt(TObj *, int, int);

static __inline__ short Out(TObj *o)
{
    if (o->h->p.whole < CAMERA_SCROLL.minX - 0x9e) return 1;
    if (o->h->p.whole > CAMERA_SCROLL.maxX + 0x9e) return 1;
    return o->y.p.whole > CAMERA_SCROLL.maxY + 0x78;
}

static __inline__ int Chk(TObj *o, short k)
{
    short r = probeSlopeWallSideAt(o, o->h->p.whole, o->y.p.whole);
    int n = k;
    if (r != 0 && n == r && SPAD.s284 == 0)
        n++;
    else if (probeFloorPlaneAt(o, o->h->p.whole, o->y.p.whole) == 0)
        n = 0;
    else
        n = SPAD.s284 == 0;
    return n;
}

void func_80123AAC(TObj *o)
{
    short v, w, s;

    FUN_800202b4(o);
    switch (o->step) {
    case 0:
        v = o->velH;
        if (v < 0)
            o->velH = v - o->b6b * 224 - 0x80;
        else
            o->velH = v + o->b6b * 224 + 0x80;
        o->velV = -(o->b6b * 0xc0) - 0x500;
        o->step++;
        break;
    case 1:
        w = o->velV + D_801266D4[o->b6b];
        o->a.raw += o->velH << 8;
        o->velV = w;
        o->y.raw += w << 8;
        if (o->velV > 0)
            o->step++;
        break;
    case 2:
        o->velV += D_801266D4[o->b6b];
        if (o->velV > 0x400)
            o->velV = 0x400;
        s = 2;
        o->y.raw += o->velV << 8;
        o->a.raw += o->velH << 8;
        if (((o->d38 - 0x40) & 0xff) > 0x80)
            s = 1;
        if (Chk(o, s)) {
            o->active = 2;
            o->b04++;
        }
        break;
    }
    if (Out(o)) {
        o->active = 2;
        o->b04 = 3;
    }
}
#else
#include "spad.h"
#include "TOBJ.H"
extern unsigned short D_801266D4[];
extern void FUN_800202b4(TObj *);
extern short probeSlopeWallSideAt(TObj *, int, int);
extern short probeFloorPlaneAt(TObj *, int, int);

static __inline__ short Out(TObj *o)
{
    if (o->h->p.whole < CAMERA_SCROLL.minX - 0x9e) return 1;
    if (o->h->p.whole > CAMERA_SCROLL.maxX + 0x9e) return 1;
    return o->y.p.whole > CAMERA_SCROLL.maxY + 0x80;
}

static __inline__ int Chk(TObj *o, short k)
{
    short r = probeSlopeWallSideAt(o, o->h->p.whole, o->y.p.whole);
    int n = k;
    if (r != 0 && n == r && SPAD.s284 == 0)
        n++;
    else if (probeFloorPlaneAt(o, o->h->p.whole, o->y.p.whole) == 0)
        n = 0;
    else
        n = SPAD.s284 == 0;
    return n;
}

void lavaCavesPigShotFragmentArc(TObj *o)
{
    short v, w, s;

    FUN_800202b4(o);
    switch (o->step) {
    case 0:
        v = o->velH;
        if (v < 0)
            o->velH = v - o->b6b * 224 - 0x80;
        else
            o->velH = v + o->b6b * 224 + 0x80;
        o->velV = -(o->b6b * 0xc0) - 0x500;
        o->step++;
        break;
    case 1:
        w = o->velV + D_801266D4[o->b6b];
        o->a.raw += o->velH << 8;
        o->velV = w;
        o->y.raw += w << 8;
        if (o->velV > 0)
            o->step++;
        break;
    case 2:
        o->velV += D_801266D4[o->b6b];
        if (o->velV > 0x400)
            o->velV = 0x400;
        s = 2;
        o->y.raw += o->velV << 8;
        o->a.raw += o->velH << 8;
        if (((o->d38 - 0x40) & 0xff) > 0x80)
            s = 1;
        if (Chk(o, s)) {
            o->active = 2;
            o->b04++;
        }
        break;
    }
    if (Out(o)) {
        o->active = 2;
        o->b04 = 3;
    }
}
#endif
