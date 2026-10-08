// FUNC 80124b4c 692 X004
// MATCHING 80124b4c 692
#include "TOBJ.H"

static __inline__ void swing(TObj *o)
{
    switch (o->state) {
    case 0:
        o->d8c = 0;
        o->timer = 1;
        o->b6b = o->b68 & 1;
        if (o->b6b) {
            o->velH = 0xa0;
            o->velX = -0x20;
            o->state = 1;
        } else {
            o->velH = -0xa0;
            o->velX = 0x20;
            o->state = 2;
        }
        o->b68 = 0;
        break;
    case 1:
        if (o->b68) o->state = 0;
        if (o->timer >= 5) {
            o->state = 3;
            break;
        }
        o->d8c = (o->d8c + (o->velH >> 4)) & 0xff;
        o->velH += o->velX;
        if (o->velH < 0) {
            if (o->b6b) o->timer++;
            o->d8c = 0;
            o->state = 2;
            o->velH = -0xa0 / o->timer;
            o->velX = 0x20 / o->timer;
        }
        if (o->timer >= 5) o->state = 3;
        break;
    case 2:
        if (o->b68) o->state = 0;
        if (o->timer >= 5) {
            o->state = 3;
            break;
        }
        o->d8c = (o->d8c + (o->velH >> 4)) & 0xff;
        o->velH += o->velX;
        if (o->velH > 0) {
            if (!o->b6b) o->timer++;
            o->d8c = 0;
            o->state = 1;
            o->velH = 0xa0 / o->timer;
            o->velX = -0x20 / o->timer;
        }
        if (o->timer >= 5) o->state = 3;
        break;
    case 3:
        o->step = 0;
        o->state = 0;
        break;
    }
}

void func_80124B4C(TObj *o)
{
    swing(o);
}
