// FUNC 8012e204 584 X004
// MATCHING 8012e204 584
#include "player.h"
/* Real size 584 B: includes the csv piece func_8012E438 (20 B), the epilogue of this function. */
#include "TOBJ.H"

extern unsigned char D_8009C93F, D_8009C942, D_8009C93E, D_8009CDD7, D_8009CF29;
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);

void func_8012E204(TObj *o)
{
    unsigned short *q;

    switch (o->state) {
    case 0:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 100;
        PLAYER.obj.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_8009C93E = 1;
        setEventStarted(0x33, 0, 0);
        o->w08 = 200;
        o->state++;
        break;
    case 1:
        if (--o->w08 > 0)
            break;
        o->state = 15;
        break;
    case 2:
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 7;
        PLAYER.obj.state = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        o->state++;
        break;
    case 3:
        D_8009C93E = 1;
        if (D_8009CDD7 == 0xff) {
            PLAYER.obj.step = 0x20;
            (*(unsigned short*)&PLAYER.obj.timer) = 1;
            PLAYER.obj.b04 = 1;
            PLAYER.obj.state = 0;
            o->state = 5;
            break;
        }
        setEventComplete(0x33, 0);
        o->w08 = 250;
        o->state++;
        break;
    case 4:
        if (--o->w08 > 0)
            break;
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0x20;
        PLAYER.obj.state = 0;
        (*(unsigned short*)&PLAYER.obj.timer) = 1;
        o->state++;
        break;
    case 5:
        q = &(*(unsigned short*)&PLAYER.obj.timer);
        D_8009CF29 = 1;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_8009C93E = 0;
        *q = *q - 1;
        o->step = 0;
        o->state = 0;
        break;
    case 15:
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
