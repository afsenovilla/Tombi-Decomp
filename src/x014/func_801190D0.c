// FUNC 801190d0 316 X014
// MATCHING 801190d0 316
#include "TOBJ.H"

extern TObj *D_8009C954;

void func_801190D0(TObj *o)
{
    switch (o->state) {
    case 0:
        o->state++;
        o->timer = 600;
        o->velH = 0x1800;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->velX = -0x40;
        o->animFrame = 0;
        D_8009C954 = o;
        break;
    case 1:
        o->d8c = (o->d8c - (o->velH >> 8)) & 0xfff;
        if (--o->timer == -1) {
            o->state++;
            o->animFrame = 1 - o->animFrame;
        }
        break;
    case 2:
        o->d8c = (o->d8c - (o->velH >> 8)) & 0xfff;
        o->velH += o->velX;
        if ((unsigned short)(o->velH + 0x17ff) >= 0x2fff) {
            o->timer = 600;
            o->state--;
            o->velX *= -1;
        }
        break;
    }
}
