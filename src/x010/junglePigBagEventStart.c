// FUNC 8012e328 352 X010
// MATCHING 8012e328 352
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern void setEventStarted(int, int, int);

void junglePigBagEventStart(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bce5b == 0) {
            o->step = 1;
            o->state = 0;
        }
        break;
    case 1:
        switch (o->state) {
        case 0:
            ((unsigned char*)&PLAYER.obj.b04)[0] = 5;
            ((unsigned char*)&PLAYER.obj.step)[0] = 100;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            ((unsigned char*)&GVARS.b3f)[0] = 1;
            ((unsigned char*)&GVARS.b42)[0] = 1;
            ((unsigned char*)&GVARS.b3e)[0] = 1;
            setEventStarted(0xb7, 0, 0);
            o->w08 = 200;
            o->state++;
            break;
        case 1:
            if (--o->w08 > 0)
                break;
            o->state = 0xf;
            break;
        case 2:
            break;
        case 0xf:
            ((unsigned char*)&PLAYER.obj.b04)[0] = 1;
            ((unsigned char*)&PLAYER.obj.step)[0] = 0;
            ((unsigned char*)&PLAYER.obj.state)[0] = 0;
            ((unsigned char*)&GVARS.b3f)[0] = 0;
            ((unsigned char*)&GVARS.b42)[0] = 0;
            ((unsigned char*)&GVARS.b3e)[0] = 0;
            o->step = 0;
            o->state = 0;
            break;
        }
        break;
    }
}
