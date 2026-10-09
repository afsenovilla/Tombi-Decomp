// FUNC 801198f8 372 X010
// MATCHING 801198f8 372
#include "TOBJ.H"

extern void func_80119648(TObj *);

void func_801198F8(TObj *o)
{
    o->animFrame = 2;
    if (o->b69 != 0) {
        o->b69 = 0;
        if ((unsigned short)(o->velX + 8) < 0x10) {
            o->animFrame = 2;
        } else {
            o->animFrame = 0;
            if (o->velX < 0)
                o->animFrame = 1;
        }
    }
    switch (o->animFrame) {
    case 0:
        o->velH = (short)(o->velH - 10) - (o->velX >> 1);
        if (o->velH < -0x600)
            o->velH = -0x600;
        break;
    case 1:
        o->velH = (short)(o->velH + 10) - (o->velX >> 1);
        if (o->velH > 0x600)
            o->velH = 0x600;
        break;
    case 2:
        if (o->velH == 0)
            return;
        if (o->velH > 0) {
            o->velH -= 0x10;
            if (o->velH < 0)
                o->velH = 0;
        } else {
            o->velH += 0x10;
            if (o->velH > 0)
                o->velH = 0;
        }
        break;
    }
    if (o->velH != 0)
        func_80119648(o);
}
