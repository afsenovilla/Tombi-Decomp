// FUNC 80119374 364 X010
// MATCHING 80119374 364
#include "TOBJ.H"

void func_80119374(TObj *o)
{
    char pad;

    o->animFrame = 2;
    if (o->b69) {
        o->b69 = 0;
        if ((unsigned short)(o->velX + 8) < 0x10) {
            o->animFrame = 2;
        } else {
            o->animFrame = 0;
            if (o->velX <= 0) {
                o->animFrame = 1;
            }
        }
    }
    switch (o->animFrame) {
    case 0:
        o->velH += o->velX >> 1;
        if (o->velH > 0xc00) o->velH = 0xc00;
        break;
    case 1:
        o->velH += o->velX >> 1;
        if (o->velH < -0xc00) o->velH = -0xc00;
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
    o->d38 += o->velH;
    o->d8c = (o->d38 >> 8) & 0xfff;
}
