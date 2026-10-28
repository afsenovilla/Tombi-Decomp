// FUNC 80117cdc 148 X001
// MATCHING 80117cdc 148
#include "TOBJ.H"

void func_80117CDC(TObj *o)
{
    switch (o->step) {
    case 0:
        o->velY = 0;
        o->step++;
        break;
    case 1:
        break;
    case 2:
        o->y.raw += o->velY << 8;
        o->velY -= 0x30;
        if (o->velY < -0x780)
            o->velY = -0x780;
        break;
    }
}
