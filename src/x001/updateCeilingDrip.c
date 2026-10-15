// FUNC 80138d4c 304 X001
// MATCHING 80138d4c 304
#include "spad.h"
#include "TOBJ.H"

extern void *D_8013E72C[];
extern void stepCeilingDripDrop(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int ObjCullRegister(TObj *);
extern void freeObjectLayer2(TObj *);

void updateCeilingDrip(TObj *o)
{
    switch (o->subtype) {
    case 0:
        stepCeilingDripDrop(o);
        break;
    case 1:
        switch (o->b04) {
        case 0:
            o->active = 2;
            o->w1e = 7;
            *(signed char *)&o->b0f = -9;
            o->d84 = 0;
            o->d88 = 0;
            o->d8c = 0;
            o->b0d = 0x80;
            o->d3c = ((int*)&SPAD.i2d4)[0];
            o->anim = D_8013E72C[0];
            AnimLoadDuration(o);
            o->b04++;
            break;
        case 1:
            ObjCullRegister(o);
            if (AnimAdvance(o) != 0) {
                o->b04++;
            }
            break;
        case 2:
            o->b04++;
            break;
        case 3:
            freeObjectLayer2(o);
            break;
        }
        break;
    }
}
