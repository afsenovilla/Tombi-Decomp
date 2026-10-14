// FUNC 8011767c 308 X016
// MATCHING 8011767c 308
#include "player.h"
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C942, D_8009C93E;
extern unsigned char D_8009D0E2;
extern void setEventComplete(int, int);
extern void removeItemFromInventory(int, int);

void func_8011767C(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 1:
        D_8009C93E = 1;
        setEventComplete(0x61, 0);
        removeItemFromInventory(0x3e, D_8009D0E2);
        o->w08 = 200;
        o->state++;
        break;
    case 2:
        if (--o->w08 <= 0) {
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_8009C93E = 0;
            PLAYER.obj.wb2 = 0;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0;
            PLAYER.obj.state = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
