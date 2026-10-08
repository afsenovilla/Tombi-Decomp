// FUNC 8003facc 196 MAIN0
#include "TOBJ.H"
extern short D_800A4580;
short FUN_800408d8(TObj *o, short a, short b);

int ObjCheckHeadCollision(TObj *o)
{
    short r;
    TObj *s;
    short y = o->y.p.whole;
    short top = D_800A4580;
    s = o;
    if (y < top - 0x90) {
        o->y.p.whole = top - 0x90;
        return 1;
    }
    if ((r = FUN_800408d8(s, s->h->p.whole + 4, y - 0x15)) != 0)
        return r;
    if ((r = FUN_800408d8(s, s->h->p.whole - 4, s->y.p.whole - 0x15)) != 0)
        return r;
    return 0;
}
