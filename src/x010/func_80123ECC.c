// FUNC 80123ecc 492 X010
// MATCHING 80123ecc 492
#include "TOBJ.H"

extern short FUN_8004065c(TObj *, short, short, int);
extern short FUN_800408d8(TObj *, short, short);
extern short FUN_80040278(TObj *, short, short);

int func_80123ECC(TObj *o, unsigned char dir)
{
    int r = 0;

    switch (dir >> 4) {
    case 0:
    case 1:
    case 2:
    case 3:
        if (FUN_8004065c(o, o->h->p.whole + 0x10, o->y.p.whole, 0))
            r = 1;
        if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10))
            r |= 8;
        else if (o->y.p.whole < -0x398) {
            o->y.p.whole = -0x398;
            r |= 8;
        }
        break;
    case 4:
        if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10))
            r = 8;
        else if (o->y.p.whole < -0x398) {
            o->y.p.whole = -0x398;
            r |= 8;
        }
        break;
    case 5:
    case 6:
    case 7:
        if (FUN_8004065c(o, o->h->p.whole - 0x10, o->y.p.whole, 1))
            r = 2;
        if (FUN_800408d8(o, o->h->p.whole, o->y.p.whole - 0x10))
            r |= 8;
        else if (o->y.p.whole < -0x398) {
            o->y.p.whole = -0x398;
            r |= 8;
        }
        break;
    case 8:
    case 9:
    case 10:
    case 11:
        if (FUN_8004065c(o, o->h->p.whole - 0x10, o->y.p.whole, 1))
            r = 2;
        if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10))
            r |= 4;
        break;
    case 12:
    case 13:
    case 14:
    case 15:
        if (FUN_8004065c(o, o->h->p.whole + 0x10, o->y.p.whole, 0))
            r = 1;
        if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10))
            r |= 4;
        break;
    }
    r |= o->b69;
    return r | o->b9d;
}
