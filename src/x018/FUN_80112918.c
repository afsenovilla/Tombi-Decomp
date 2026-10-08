// FUNC 80112918 184 X018
// MATCHING 80112918 184
#include "TOBJ.H"
extern short DAT_1f80027e;
extern short FUN_80040278(TObj *, int, int);
extern void FUN_800202b4(TObj *);

void FUN_80112918(TObj *o)
{
    short yy;
    switch (o->step) {
    case 0:
        yy = o->y.p.whole;
        o->y.p.whole = yy + 4;
        if (FUN_80040278(o, o->h->p.whole, (short)(yy + 0x14))) {
            o->d8c = (-DAT_1f80027e << 2) & 0xff;
            o->step++;
        }
        o->d30 = o->h->p.whole;
        o->d34 = o->y.p.whole;
    case 1:
        FUN_800202b4(o);
    }
}
