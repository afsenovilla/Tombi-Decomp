// FUNC 8012f10c 360 X004
// MATCHING 8012f10c 360
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;
extern void setEventComplete(int, int);
extern void addItemToInventory(int, int, int);

void paintingBigKeyReward(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER.b04 = 5;
        PLAYER.step = 0;
        PLAYER.state = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        o->state++;
        break;
    case 1:
        ((unsigned char*)&GVARS.b3e)[0] = 1;
        o->w08 = 0xc8;
        o->state++;
        setEventComplete(0x38, 0);
        break;
    case 2:
        if (--o->w08 > 0) break;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        ((unsigned char*)&GVARS.b3e)[0] = 0;
        o->state++;
        break;
    case 3:
        addItemToInventory(0x35, 1, 1);
        o->w08 = 0x3c;
        o->state = 0xf;
        break;
    case 15:
        if (--o->w08 > 0) break;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        break;
    }
}
