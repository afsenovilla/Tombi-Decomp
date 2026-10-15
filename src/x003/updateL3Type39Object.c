// FUNC 80119838 244 X003
// MATCHING 80119838 244
#include "spad.h"
#include "gv2.h"
#include "TOBJ.H"

extern void *D_80138ED8;
extern void AnimLoadDuration(TObj *);
extern int FUN_800202b4(TObj *);
extern int AnimAdvance(TObj *);
extern void FUN_800187e4(TObj *);

void updateL3Type39Object(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GV2.bc3 & 4) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->w1e = 0xe;
        o->b0d = 0x81;
        o->w08 = 0x7c0e;
        o->d3c = ((int*)&SPAD.i2d4)[0];

        o->anim = D_80138ED8;
        AnimLoadDuration(o);
        break;
    case 1:
        FUN_800202b4(o);
        AnimAdvance(o);
        break;
    case 2:
        break;
    case 3:
        FUN_800187e4(o);
        break;
    }
}
