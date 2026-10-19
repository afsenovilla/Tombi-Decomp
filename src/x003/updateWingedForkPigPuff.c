// FUNC 8012fbc4 232 X003
// MATCHING 8012fbc4 232
#include "spad.h"
#include "TOBJ.H"
extern void *D_8013A704[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_800202b4(TObj *);
extern void FUN_800187e4(TObj *);

void updateWingedForkPigPuff(TObj *o)
{
    unsigned char b = o->b04;

    switch (b) {
    case 0:
        o->b04 = b + 1;
        o->w1e = 1;
        o->d3c = ((int*)&SPAD.i2dc)[0];
        *(signed char *)&o->b0f = -0xb;
        o->b0d = 0x80;
        o->wac = 0x1b;
        o->anim = D_8013A704[0];
        AnimLoadDuration(o);
        break;
    case 1:
        if (AnimAdvance(o))
            o->b04++;
        FUN_800202b4(o);
        break;
    case 2:
    case 3:
        FUN_800187e4(o);
        break;
    }
}
