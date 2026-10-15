// FUNC 80118f3c 464 X003
// MATCHING 80118f3c 464
#include "spad.h"
#include "gv2.h"
#include "TOBJ.H"

extern void *D_80138EB0[];
extern void windStreakRespawn(TObj *);
extern int ObjCullRegister(TObj *);
extern void ObjFree(TObj *);

void updateWindStreak(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (GV2.bc3 & 2) {
            o->b04 = 3;
            break;
        }
        o->b04++;
        o->w1e = 8;
        o->b0d = 0x80;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->anim = D_80138EB0[o->b0c];
        o->d64 = 0x1400;
        break;
    case 1:
        switch (o->step) {
        case 0:
            o->step++;
            windStreakRespawn(o);
            break;
        case 1:
            switch (o->subtype) {
            case 0:
            case 2:
                o->a.raw += 0x180000;
                break;
            case 1:
            case 4:
                o->a.raw += 0x160000;
                o->y.raw += -0x80000;
                break;
            case 3:
                o->a.raw += 0x160000;
                o->y.raw += 0x60000;
                break;
            case 5:
            case 6:
                o->a.raw += 0xf0000;
                break;
            case 7:
                o->a.raw += 0xa0000;
                break;
            }
            if (!ObjCullRegister(o)) o->step = 0;
            break;
        }
        break;
    case 2:
        o->b04++;
        break;
    case 3:
        ObjFree(o);
        break;
    }
}
