// FUNC 80121b4c 444 X014
// MATCHING 80121b4c 444
#include "TOBJ.H"

extern short DAT_8007a5f0[];
extern short D_8007A3F0[];
extern short D_800A457C, D_800A457E, D_800A4582;
extern short FUN_80041240(TObj *, short, short);
extern short func_80041EBC(TObj *, short, short);

static __inline__ short hitWall(TObj *o, short r)
{
    short v = FUN_80041240(o, o->h->p.whole, o->y.p.whole);
    if (v != 0) {
        if (r != v) return 0;
        return 1;
    }
    if (func_80041EBC(o, o->h->p.whole, o->y.p.whole) != 0) return 1;
    return 0;
}

static __inline__ int offScreen(TObj *o)
{
    if (o->h->p.whole < D_800A457C - 0xa0) return 1;
    if (D_800A457E + 0xa0 < o->h->p.whole) return 1;
    return D_800A4582 + 0x80 < o->y.p.whole;
}

void func_80121B4C(TObj *o)
{
    short r;

    switch (o->step) {
    case 0:
        o->velH = 0x500;
        o->step++;
        break;
    case 1:
        r = 2;
        {
            int i = o->d38;
            int dx = (o->velH * DAT_8007a5f0[i]) << 4 >> 16;
            int dy = (o->velH * D_8007A3F0[i]) << 4 >> 16;
            o->h->raw += dx << 8;
            o->y.raw += dy << 8;
        }
        if (((o->d38 - 0x40) & 0xff) > 0x80) r = 1;
        if (hitWall(o, r)) {
            o->active = 2;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
        } else if (offScreen(o)) {
            o->active = 2;
            o->b04 = 3;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
