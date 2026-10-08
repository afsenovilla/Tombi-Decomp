// FUNC 8011b624 296 X001
// MATCHING 8011b624 296
#include "TOBJ.H"

extern short D_8007A5F0[];

#define RESET(o) \
    (o)->y.raw = (o)->d34; \
    (o)->h->raw = (o)->d30; \
    (o)->velH = 0; \
    (o)->velV = 0; \
    (o)->velX = 0; \
    (o)->velY = 0; \
    (o)->d84 = 0; \
    (o)->d88 = 0; \
    (o)->w74 = 0;

void func_8011B624(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        RESET(o);
        o->velV = 0x200;
        break;
    case 1:
        o->d88 += 4;
        if (o->d88 & 0x80) {
            o->state++;
            break;
        }
        o->y.raw -= (D_8007A5F0[o->d88 & 0xff] * o->velV) >> 4;
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        RESET(o);
        break;
    }
}
