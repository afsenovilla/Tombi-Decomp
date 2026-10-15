// FUNC 8011f1a8 264 X001
// MATCHING 8011f1a8 264
#include "spad.h"
#include "TOBJ.H"

extern void *D_8013E5E8[];
extern void AnimLoadDuration(TObj *);
extern void cullObjectToDrawList(TObj *, int);
extern void ObjFreeDup(TObj *);

void updateClimbPole(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->w1e = 8;
        o->b0d = 0;
        o->anim = D_8013E5E8[o->subtype];
        o->d3c = ((int*)&SPAD.i2d4)[0];
        switch (o->subtype) {
        case 0 ... 1:
            o->box1 = 0x10;
            o->box2 = 2;
            o->box0 = 8;
            o->box3 = 0x72;
            break;
        case 2:
            o->active = 2;
            break;
        }
        AnimLoadDuration(o);
        break;
    case 1:
        cullObjectToDrawList(o, 0x60);
        break;
    case 2:
        break;
    case 3:
        ObjFreeDup(o);
        break;
    }
}
