// FUNC 801337b4 388 X003
// MATCHING 801337b4 388
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);

void windDiesEventScene(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 100;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        GVARS.b3e = 1;
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
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        GVARS.b3e = 1;
        setEventComplete(0xaa, 0);
        o->w08 = 200;
        o->state = 1;
        break;
    case 0xf:
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        GVARS.b3f = 0;
        GVARS.b42 = 0;
        GVARS.b3e = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
