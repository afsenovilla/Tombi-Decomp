// FUNC 801337b4 388 X003
// MATCHING 801337b4 388
#include "player.h"
#include "TOBJ.H"
extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);

void func_801337B4(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 100;
        PLAYER.obj.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        setEventStarted(0xaa, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 1:
        if (--o->w08 <= 0) o->state = 0xf;
        break;
    case 2:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 100;
        PLAYER.obj.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        setEventComplete(0xaa, 0);
        o->w08 = 200;
        o->state = 1;
        break;
    case 0xf:
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
