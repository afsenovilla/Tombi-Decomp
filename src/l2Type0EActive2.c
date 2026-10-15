// FUNC 80112918 184 X000
// MATCHING 80112918 184
// VERSION SLPM_80165 x00 8010CE18 164
#if defined(VERSION_SLPM_80165)
// Demo version of PAL x000 80112918 (l2Type0EActive2.c): does not store the position at +0x30/+0x34.
#include "TOBJ.H"
extern short DAT_1f80027e;
extern short FUN_80040278(TObj *, int, int);
extern void FUN_800202b4(TObj *);

void l2Type0EActive2(TObj *o)
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
    case 1:
        FUN_800202b4(o);
    }
}
#else
#include "TOBJ.H"
extern short DAT_1f80027e;
extern short FUN_80040278(TObj *, int, int);
extern void FUN_800202b4(TObj *);

void l2Type0EActive2(TObj *o)
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
#endif
