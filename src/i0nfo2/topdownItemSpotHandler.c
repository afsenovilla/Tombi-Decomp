// FUNC 800ee748 360 I0NFO2
// MATCHING 800ee748 360
#include "TOBJ.H"
#include "spad.h"
#include "raw7.h"
extern void func_8001FE6C(TObj *o);
extern void topdownItemSpotUpdate(TObj *o);
extern void func_80018790(TObj *o);
extern unsigned char D_8009D064[];
extern short TOPDOWN_ITEM_SPOT_KINDS[];
extern void *TOPDOWN_ITEM_SPOT_ANIMS[][8];

void topdownItemSpotHandler(TObj *o)
{
    switch (o->b04) {
    case 0: {
        int t;
        o->w1e = 0xF;
        t = SPAD.i2f8;
        S16(o, 0x6C) = 0x10;
        S16(o, 0x6E) = 0x20;
        S16(o, 0x70) = 0xD;
        o->b0d = 0;
        S16(o, 0x72) = 0x1D;
        o->b0f = 0;
        S32(o, 0x3C) = t;
        o->anim = TOPDOWN_ITEM_SPOT_ANIMS[TOPDOWN_ITEM_SPOT_KINDS[o->b0c]][0];
        func_8001FE6C(o);
        o->b04 = 1;
        o->step = D_8009D064[o->b0c];
        o->state = 0;
        break;
    }
    case 1:
        topdownItemSpotUpdate(o);
        break;
    case 2:
        if (o->state == 0) {
            o->anim = TOPDOWN_ITEM_SPOT_ANIMS[TOPDOWN_ITEM_SPOT_KINDS[o->b0c]][0];
            func_8001FE6C(o);
            o->state++;
        }
        break;
    case 3:
        func_80018790(o);
        break;
    }
}
