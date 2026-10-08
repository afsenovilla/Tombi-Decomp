// FUNC 80132964 216 X001
/* score 19: only the result register differs (inline result in v1 + final move v0,v1; game returns straight in v0). Direct returns/short r/ternaries all become store-flag (sltu; sll) via jump.c (score 31); tried int/short/char return types. */
#include "TOBJ.H"

extern short func_8004065C(TObj *, short, short, short);
extern short FUN_80040278(TObj *, short, short);

static __inline__ short test(TObj *o)
{
    int d = o->d88;
    unsigned char c = (unsigned int)(d - 0x40) < 0x80;

    if (o->b69 != 0) {
        if (c) {
            o->d88 = d - 0x10;
        } else {
            o->d88 = d + 0x10;
        }
        o->d88 = *(unsigned char *)&o->d88;
        return 1;
    }
    if (d < 0x81) {
        if (func_8004065C(o, o->h->p.whole, o->y.p.whole, c)) return 2;
        return 0;
    }
    if (func_8004065C(o, o->h->p.whole, o->y.p.whole, c)) return 2;
    if (FUN_80040278(o, o->h->p.whole, o->y.p.whole)) return 1;
    return 0;
}

short func_80132964(TObj *o)
{
    return test(o);
}
