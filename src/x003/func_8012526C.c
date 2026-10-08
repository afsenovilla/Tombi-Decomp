// FUNC 8012526c 324 X003
// MATCHING 8012526c 324
#include "TOBJ.H"

void func_8012526C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->velH = 0x100;
        o->velX = 8;
        o->d8c = 0;
        o->velY = 0;
        o->timer = 1 - o->b6b;
        o->state++;
        break;
    case 1: {
        short v = o->velH - o->velX;
        o->velH = v;
        if (o->timer) {
            o->velY -= v;
        } else {
            o->velY += v;
        }
        o->d8c = (unsigned short)o->velY >> 8;
        if (o->velH <= 0) {
            o->velH = 0;
            o->state++;
        }
        break;
    }
    case 2: {
        short v = o->velH + o->velX;
        o->velH = v;
        if (o->timer) {
            o->velY += v;
        } else {
            o->velY -= v;
        }
        o->d8c = (unsigned short)o->velY >> 8;
        if (o->velH >= 0x100) {
            o->velH = 0x100;
            o->state--;
            o->timer = 1 - o->timer;
        }
        break;
    }
    }
}
