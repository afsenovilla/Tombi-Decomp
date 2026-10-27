// FUNC 80117364 456 X010
// MATCHING 80117364 456
#include "spad.h"
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"

extern void playSFX(int);
extern void ObjListPush_1F800230(TObj *);
extern void uvAnimBgModelFlip(TObj *);
extern void freeObjectLayer8(TObj *);

void updateUvAnimBgModel(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        switch (o->subtype) {
        case 0:
        case 5:
            break;
        case 1:
            playSFX(0xb1);
            break;
        case 2:
            if (GV2.bc3 & 0x40) o->b04 = 3;
            break;
        case 3:
        case 4:
            if (!(GV2.bc3 & 0x40)) o->b04 = 3;
            break;
        }
        break;
    case 1:
        o->visible = 1;
        ObjListPush_1F800230(o);
        if (GVARS.hurtState) {
            o->b04 = 2;
            o->step = 0;
            break;
        }
        switch (o->subtype) {
        case 0:
            if ((*(short*)((char*)&SPAD.i174 + 2)) < 0xac9) break;
        case 1:
        case 2:
        case 4:
        case 5:
            uvAnimBgModelFlip(o);
            break;
        case 3:
            break;
        }
        break;
    case 2:
        o->visible = 1;
        ObjListPush_1F800230(o);
        if (o->step) break;
        if (o->w22 & 1) {
            o->timer = 1;
            uvAnimBgModelFlip(o);
        }
        o->step++;
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
