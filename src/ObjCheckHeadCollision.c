// FUNC 8003facc 196 MAIN0
// MATCHING 8003facc 196
#include "TOBJ.H"
extern short D_800A4580;
short FUN_800408d8(TObj *o, short a, short b);

static __inline__ short chk(TObj *o, int dx)
{
    return FUN_800408d8(o, o->h->p.whole + dx, o->y.p.whole - 0x15);
}

int ObjCheckHeadCollision(TObj *o)
{
    short r;
    TObj *s = o;
    if (o->y.p.whole < D_800A4580 - 0x90) {
        o->y.p.whole = D_800A4580 - 0x90;
        return 1;
    }
    if ((r = chk(s, 4)) != 0)
        return r;
    if ((r = chk(s, -4)) != 0)
        return r;
    return 0;
}
