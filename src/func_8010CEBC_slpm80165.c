// VERSION SLPM_80165 x00 8010CEBC 180
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of treasureChestSettleOnGround (src/x011/treasureChestSettleOnGround.c): sets the active byte to 4,
// no landing angle or position copy; leaves for mode 2 when the byte at D_8009A7EB is set.
#include "TOBJ.H"
extern unsigned char D_8009A7EB;
extern short FUN_80040278(TObj *, int, int);
extern void FUN_800202b4(TObj *);

void treasureChestSettleOnGround(TObj *o)
{
    short yy;
    switch (o->step) {
    case 0:
        yy = o->y.p.whole;
        o->active = 4;
        o->y.p.whole = yy + 4;
        if (FUN_80040278(o, o->h->p.whole, (short)(yy + 0x14)))
            o->step++;
    case 1:
        if (D_8009A7EB != 0) {
            o->b04 = 2;
            o->step = 0;
        }
        FUN_800202b4(o);
    }
}
#endif
