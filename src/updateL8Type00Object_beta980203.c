// VERSION BETA_980203 x00 800EF750 180
#if defined(VERSION_BETA_980203)
// Beta version: PAL updateL8Type00Object.c with one cull call (no margin variant for subtype 0).
#include "TOBJ.H"
extern unsigned short D_801182B4[];
extern void cullObjectToDrawList(TObj *, short);
extern void freeObjectLayer8(TObj *);

void updateL8Type00Object(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->b04++;
        o->box0 = D_801182B4[o->b0c];
        if (o->animFrame != 0) {
            o->d88 = -0x400;
            o->b0a = 0x11;
        }
        break;
    case 1:
        cullObjectToDrawList(o, o->box0);
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer8(o);
        break;
    }
}
#endif
