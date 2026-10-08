// FUNC 8011b0b8 440 X003
// MATCHING 8011b0b8 440
#include "TOBJ.H"

void func_8011B0B8(TObj *o)
{
    unsigned char s = o->step;

    switch (s) {
    case 0:
        o->step = s + 1;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
    case 1:
        if (o->b68 == 0)
            break;
        o->velH = 0x800;
        o->w74 = 0x800;
        o->velX = -0x200;
        o->timer = 4;
        o->step++;
        break;
    case 2:
        if (o->b68 != 0) {
            if (o->timer < 2)
                o->step = 0;
            break;
        }
        if (o->animFrame & 1)
            o->d8c += o->velH >> 4;
        else
            o->d8c -= o->velH >> 4;
        o->d8c &= 0xfff;
        o->velH += o->velX;
        if (o->velH == o->w74) {
            if (--o->timer == 0) {
                o->step++;
            } else {
                o->velX *= -1;
                o->velH -= 0x100;
                o->velX += 0x40;
                o->w74 = o->velH;
            }
        } else if (o->velH == -o->w74) {
            o->velX *= -1;
        }
        break;
    case 3:
        o->step = 0;
        o->d8c = 0;
        break;
    }
}
