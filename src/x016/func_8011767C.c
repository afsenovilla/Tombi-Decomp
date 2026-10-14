// FUNC 8011767c 308 X016
// MATCHING 8011767c 308
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void setEventComplete(int, int);
extern void removeItemFromInventory(int, int);

void func_8011767C(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->state++;
        break;
    case 1:
        GVARS.b3e = 1;
        setEventComplete(0x61, 0);
        removeItemFromInventory(0x3e, GCD.bd0e2);
        o->w08 = 200;
        o->state++;
        break;
    case 2:
        if (--o->w08 <= 0) {
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
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
