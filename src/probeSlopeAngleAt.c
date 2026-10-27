// FUNC 80041d20 412 MAIN0
// MATCHING 80041d20 412
// VERSION SLPM_80165 slpm80165 8003E8B4 432
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 80041D20 (func_80041D20.c): returns 0 for a negative column first.
#include "TOBJ.H"
extern short *getCollisionPlaneAt(int, int);
extern int angleFromDelta(int, int);
extern unsigned short *COLLISION_SEG_PTR;
extern short SLOPE_PROBE_ANGLE;

int func_80041D20(TObj *o, short a, int b)
{
    char pad[8];
    short *p;
    unsigned short *q;
    unsigned short f;
    int n, cnt, x, w, d, e;
    unsigned int k;
    short s;
    short ws;
    unsigned short t3;
    if (a < 0) return 0;
    p = getCollisionPlaneAt(a, o->d->p.whole);
    COLLISION_SEG_PTR = (unsigned short *)(p + 1);
    n = cnt = *p;
    goto test;
top:
    q = COLLISION_SEG_PTR;
    COLLISION_SEG_PTR = q + 1;
    f = *q;
    cnt--;
    if (f & 0x4000) {
        if (!(f & 0x10)) goto hit;
    }
    COLLISION_SEG_PTR = q + 4;
    goto next;
hit:
    x = *COLLISION_SEG_PTR++;
    w = *COLLISION_SEG_PTR++;
    COLLISION_SEG_PTR++;
    ws = w;
    t3 = *(unsigned short *)((char *)q + 6);
    if (ws == 0) goto next;
    k = (t3 >> 4) & 0xf;
    d = b - x - w;
    if (((d - 1) & 0xffff) > -ws) {
        if (d << 16 > 0) return 0;
        goto next;
    }
    s = angleFromDelta(k, ws);
    SLOPE_PROBE_ANGLE = s;
    if (s < 0x40) {
        SLOPE_PROBE_ANGLE = s + 0xc0;
    } else if (s > 0xc0) {
        SLOPE_PROBE_ANGLE = s - 0xc0;
    }
    e = x + 8;
    if (((b - e - w - 1) & 0xffff) > -(short)w) return 2;
    return 1;
next:
    n = cnt << 16;
test:
    if (n != 0) goto top;
    return 0;
}
#else
#include "TOBJ.H"
extern short *getCollisionPlaneAt(int, int);
extern int angleFromDelta(int, int);
extern unsigned short *COLLISION_SEG_PTR;
extern short SLOPE_PROBE_ANGLE;

int probeSlopeAngleAt(TObj *o, short a, int b)
{
    char pad[8];
    short *p;
    unsigned short *q;
    unsigned short f;
    int n, cnt, x, w, d, e;
    unsigned int k;
    short s;
    short ws;
    unsigned short t3;
    p = getCollisionPlaneAt(a, o->d->p.whole);
    COLLISION_SEG_PTR = (unsigned short *)(p + 1);
    n = cnt = *p;
    goto test;
top:
    q = COLLISION_SEG_PTR;
    COLLISION_SEG_PTR = q + 1;
    f = *q;
    cnt--;
    if (f & 0x4000) {
        if (!(f & 0x10)) goto hit;
    }
    COLLISION_SEG_PTR = q + 4;
    goto next;
hit:
    x = *COLLISION_SEG_PTR++;
    w = *COLLISION_SEG_PTR++;
    COLLISION_SEG_PTR++;
    ws = w;
    t3 = *(unsigned short *)((char *)q + 6);
    if (ws == 0) goto next;
    k = (t3 >> 4) & 0xf;
    d = b - x - w;
    if (((d - 1) & 0xffff) > -ws) {
        if (d << 16 > 0) return 0;
        goto next;
    }
    s = angleFromDelta(k, ws);
    SLOPE_PROBE_ANGLE = s;
    if (s < 0x40) {
        SLOPE_PROBE_ANGLE = s + 0xc0;
    } else if (s > 0xc0) {
        SLOPE_PROBE_ANGLE = s - 0xc0;
    }
    e = x + 8;
    if (((b - e - w - 1) & 0xffff) > -(short)w) return 2;
    return 1;
next:
    n = cnt << 16;
test:
    if (n != 0) goto top;
    return 0;
}
#endif
