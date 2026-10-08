// FUNC 80126d04 336 X004
// MATCHING 80126d04 336
#include "TOBJ.H"
typedef struct { char pad[0xc4]; unsigned short wc4, wc6, wc8; } X4;

extern short D_1F8001C8;
extern void func_80125420(TObj *);
extern void func_80125554(TObj *);
extern void func_801256A8(TObj *);
extern void func_80125818(TObj *);
extern void func_80126464(TObj *);
extern short func_80041EBC(TObj *, short, short);

static __inline__ void approach(TObj *o, unsigned short t)
{
    short d = o->d->p.whole;
    int s;

    if (t != d) {
        s = (t - d) >> 4;
        if ((unsigned)(s + 1) < 3) {
            o->d->p.whole = t;
        } else {
            o->d->p.whole = d + s;
        }
    }
}

void func_80126D04(TObj *o)
{
    switch (o->state) {
    case 0:
        func_80125420(o);
        goto common;
    case 1:
        func_80125554(o);
        goto common;
    case 2:
        func_801256A8(o);
    common:
        if (func_80041EBC(o, o->h->p.whole, o->y.p.whole + 0x18) == 0) {
            o->state = 3;
            o->substep = 0;
        }
        break;
    case 3:
        func_80125818(o);
        break;
    case 4:
        func_80126464(o);
        break;
    }
    if (D_1F8001C8 == 0) {
        approach(o, ((X4 *)o)->wc8);
    } else {
        approach(o, ((X4 *)o)->wc4);
    }
}
