// FUNC 80119f40 360 X002
// MATCHING 80119f40 360
// VERSION SLPM_80165 x00 800EC1EC 360
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x002 80119F40 (func_80119F40.c): short index without the &1 mask; scratchpad read through a
// literal address.
#include "TOBJ.H"

typedef struct { short v[2][8][2]; } T;
extern T D_80115FF4;
#define D_1F8001F8 (*(unsigned short *)0x1F8001F8)
extern TObj *ObjAlloc(void);

void func_80119F40(int k, int x, int y, int z)
{
    T t;
    TObj *o;
    short *p;

    t = D_80115FF4;
    o = ObjAlloc();
    if (o != 0) {
        o->active = 1;
        o->type = 0x31;
        o->subtype = 1;
        p = t.v[(short)k][D_1F8001F8 & 7];
        o->a.p.whole = x;
        o->y.p.whole = y;
        o->b.p.whole = z;
        o->h->p.whole += p[0];
        o->y.p.whole += p[1];
    }
}
#else
#include "TOBJ.H"

typedef struct { short v[2][8][2]; } T;
extern T D_80115FF4;
extern unsigned short D_1F8001F8;
extern TObj *ObjAlloc(void);

void spawnSparkleNear(int k, int x, int y, int z)
{
    T t;
    TObj *o;
    short *p;

    t = D_80115FF4;
    o = ObjAlloc();
    if (o != 0) {
        o->active = 1;
        o->type = 0x31;
        o->subtype = 1;
        p = t.v[k & 1][D_1F8001F8 & 7];
        o->a.p.whole = x;
        o->y.p.whole = y;
        o->b.p.whole = z;
        o->h->p.whole += p[0];
        o->y.p.whole += p[1];
    }
}
#endif
