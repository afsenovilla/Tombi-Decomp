// FUNC 80132a3c 296 X001
// MATCHING 80132a3c 296
#include "TOBJ.H"

void func_80132A3C(TObj *o)
{
    short t;

    switch (o->substep) {
    case 0:
        o->w22 = 0x10;
        o->substep++;
    case 1:
        o->d84 = (o->d84 + 1) & 0xff;
        if (--o->w22 == -1) {
            o->w22 = 0x20;
            o->substep++;
        }
        break;
    case 2:
        o->d84 = (o->d84 - 1) & 0xff;
        if (--o->w22 == -1) {
            o->w22 = 0x20;
            o->substep--;
        }
        break;
    }
    t = o->timer;
    if (t) {
        o->timer = t - 1;
    } else if (o->d88 == o->d84) {
        o->substep = 0;
        o->state++;
    }
}
