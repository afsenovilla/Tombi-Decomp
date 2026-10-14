// FUNC 8012dfbc 584 X004
// MATCHING 8012dfbc 584
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
extern unsigned char D_8009CDDB, D_8009CF29;
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);

void func_8012DFBC(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 100;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        GVARS.b3e = 1;
        setEventStarted(0x37, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 1:
        if (--o->w08 <= 0) o->state = 15;
        break;
    case 2:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 7;
        PLAYER.obj.state = 0;
        GVARS.b3f = 1;
        GVARS.b42 = 1;
        o->state++;
        break;
    case 3:
        GVARS.b3e = 1;
        if (D_8009CDDB == 0xff) {
            PLAYER.obj.step = 0x20;
            PLAYER.obj.timer = 1;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.state = 0;
            o->state = 5;
        } else {
            setEventComplete(0x37, 0);
            o->w08 = 250;
            o->state++;
        }
        break;
    case 4:
        if (--o->w08 <= 0) {
            PLAYER.obj.b04 = 1;
            PLAYER.obj.step = 0x20;
            PLAYER.obj.state = 0;
            PLAYER.obj.timer = 1;
            o->state++;
        }
        break;
    case 5:
        {
            short *p = &PLAYER.obj.timer;
            D_8009CF29 = 1;
            GVARS.b3f = 0;
            GVARS.b42 = 0;
            GVARS.b3e = 0;
            (*p)--;
        }
        o->step = 0;
        o->state = 0;
        break;
    case 15:
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
