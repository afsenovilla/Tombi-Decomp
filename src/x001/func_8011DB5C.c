// FUNC 8011db5c 568 X001
// MATCHING 8011db5c 568
#include "TOBJ.H"

void func_8011DB5C(TObj *o)
{
    o->animFrame = 2;
    if (o->b69 != 0) {
        o->b69 = 0;
        if ((unsigned short)(o->velX + 8) < 0x10) {
            o->animFrame = 2;
        } else {
            o->animFrame = 0;
            if (o->velX < 0) o->animFrame = 1;
        }
    }
    switch (o->animFrame) {
    case 0:
        o->velH = (short)(o->velH - 10) - (o->velX >> 1);
        if (o->velH < -0x600) o->velH = -0x600;
        break;
    case 1:
        o->velH = (short)(o->velH + 10) - (o->velX >> 1);
        if (o->velH > 0x600) o->velH = 0x600;
        break;
    case 2:
        if (o->velH != 0) {
            if (o->velH > 0) {
                o->velH -= 0x10;
                if (o->velH < 0) o->velH = 0;
            } else {
                o->velH += 0x10;
                if (o->velH > 0) o->velH = 0;
            }
        }
        break;
    }
    switch (o->subtype) {
    case 0:
        if (o->velH > 0) {
            o->d38 += o->velH;
            if (o->d38 > 0x38e00) {
                o->d38 = 0x38e00;
                o->velH = 0;
            }
        } else {
            o->d38 += o->velH;
            if (o->d38 < -0x38e00) {
                o->d38 = -0x38e00;
                o->velH = 0;
            }
        }
        break;
    case 1:
        if (o->velH > 0) {
            o->d38 += o->velH;
            if (o->d38 > 0x80000) {
                o->d38 = 0x80000;
                o->velH = 0;
            }
        } else {
            o->d38 += o->velH;
            if (o->d38 < 0) {
                o->d38 = 0;
                o->velH = 0;
            }
        }
        break;
    }
    o->d8c = (o->d38 >> 8) & 0xfff;
}
