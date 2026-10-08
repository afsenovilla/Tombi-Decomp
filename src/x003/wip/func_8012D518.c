// FUNC 8012d518 340 X003
/* score 7: final 'if (slt) return 1; return 0;' collapses (game keeps bnez/li 1/move 0); first compare load order; tried int/short r, inline past(), goto forms */
#include "TOBJ.H"

extern short func_800408D8(TObj *, short, short);
extern short FUN_80040278(TObj *, short, short);

static __inline__ int land(TObj *o)
{
    if (o->b69 == 1) {
        o->b69 = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) {
        o->b69 = 0;
        return 1;
    }
    return 0;
}

int func_8012D518(TObj *o)
{
    short r;

    switch (o->velX & 7) {
    case 1:
    case 3:
        if (func_800408D8(o, o->h->p.whole, o->y.p.whole - 0x10)) return 1;
        break;
    case 5:
    case 7:
        if (land(o)) return 1;
        break;
    }
    if (o->animFrame & 1) {
        r = o->wb6 > o->a.p.whole;
    } else if (o->y.p.whole < -0x46a) {
        r = o->wb8 - 0x14 < o->a.p.whole;
    } else {
        r = o->a.p.whole > o->wb8;
    }
    if (r) return 1;
    return 0;
}
