// FUNC 8003f598 564 MAIN0
// MATCHING 8003f598 564
#include "TOBJ.H"
extern unsigned short COLLISION_HIT_FLAGS;
extern short probeCollisionAtDepthB(TObj *, int, int);
typedef struct { char pad[0xa1]; unsigned char ba1; } XA1;
#define BA1(o) (((XA1 *)(o))->ba1)

static __inline__ unsigned char attr(TObj *o)
{
    int n;
    n = (COLLISION_HIT_FLAGS >> 5) & 0xf;
    if (!(COLLISION_HIT_FLAGS & 0x3000)) {
        switch (n) {
        case 13: BA1(o) = 3; break;
        case 14: BA1(o) = 2; break;
        case 15: BA1(o) = 1; break;
        }
        return BA1(o);
    }
    return 0;
}

int probeTileShapeThreePoints(TObj *o_, short dy)
{
    TObj *o = o_;
    BA1(o) = 0;
    if (probeCollisionAtDepthB(o, (short)(o->h->p.whole + 8), (short)(dy + (o->y.p.whole + o->box2))) && attr(o))
        return 1;
    if (probeCollisionAtDepthB(o, (short)(o->h->p.whole - 8), (short)(dy + (o->y.p.whole + o->box2))) && attr(o))
        return 1;
    if (probeCollisionAtDepthB(o, o->h->p.whole, o->y.p.whole) && attr(o))
        return 1;
    return 0;
}
