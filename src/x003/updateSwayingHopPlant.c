// FUNC 8012552c 456 X003
// MATCHING 8012552c 456
#include "spad.h"
#include "TOBJ.H"
extern void *D_80138E94;
extern int FUN_800202b4(TObj *);
extern void swayingHopPlantSway(TObj *);
extern void swayingHopPlantHop(TObj *);
extern void freeObjectLayer2(TObj *);

static __inline__ int near(TObj *o)
{
    if ((unsigned short)(o->d->p.whole - (*(unsigned short*)((char*)&SPAD.i170 + 2)) + 0x2d) >= 0x5b) return 0;
    if ((short)((*(unsigned short*)((char*)&SPAD.i16c + 2)) - o->y.p.whole) < -0xb0) return 0;
    return (unsigned short)(o->h->p.whole - (*(unsigned short*)((char*)&SPAD.i168 + 2)) + 0x10) < 0x21;
}

void updateSwayingHopPlant(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box2 = 8;
        o->box0 = 2;
        o->w1e = 8;
        o->b0d = 0;
        o->box1 = 4;
        o->box3 = 0x10;
        o->animFrame = 1;
        o->anim = D_80138E94;
        o->d3c = ((int*)&SPAD.i2d4)[0];
        o->b6b = 0;
        o->b6a = 0;
        o->b69 = 0;
        o->b68 = 0;
        o->b04++;
        o->step = 0;
        o->state = 0;
        o->d34 = o->y.p.whole;
        break;
    case 1:
        if (FUN_800202b4(o)) {
            switch (o->step) {
            case 0:
                if (near(o)) {
                    o->step = 1;
                    o->state = 0;
                } else {
                    swayingHopPlantSway(o);
                }
                break;
            case 1:
                swayingHopPlantHop(o);
                break;
            }
        }
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer2(o);
        break;
    }
}
