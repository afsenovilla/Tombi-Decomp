// FUNC 8011b958 388 X001
// MATCHING 8011b958 388
#include "TOBJ.H"
extern short D_8007A5F0[];

void func_8011B958(TObj *o)
{
    switch (o->state) {
    case 0:
        o->y.raw = o->d34;
        o->h->raw = o->d30;
        o->velV = 0;
        o->velH = 0;
        o->velV = 0x200;
        o->velX = 0;
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->w74 = 0;
        o->velY = -0x80;
        o->timer = 0;
        o->state++;
        break;
    case 1:
        if (o->timer > 0) {
            o->state = 2;
            break;
        }
        o->d88 += 4;
        if ((o->d88 & 0xff) == 0) {
            o->velV += o->velY;
            o->y.raw = o->d34;
            o->timer++;
        }
        o->y.raw += (D_8007A5F0[(unsigned char)o->d88] * o->velV) >> 4;
        if (o->b6a == 1) {
            o->state++;
        }
        break;
    case 2:
        o->y.raw = o->d34;
        o->h->raw = o->d30;
        o->velH = 0;
        o->velV = 0;
        o->velX = 0;
        o->velY = 0;
        o->d84 = 0;
        o->d88 = 0;
        o->w74 = 0;
        o->timer = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
