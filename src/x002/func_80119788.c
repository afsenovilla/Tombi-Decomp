// FUNC 80119788 384 X002
// MATCHING 80119788 384
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned char D_8009C93A, D_8009CDCB, D_8009D2B0;
extern unsigned int D_8009C98C;
extern short D_1F8001C6;
extern unsigned char D_8009C93FA[], D_8009C942A[];
extern void setEventComplete(int, int);

void func_80119788(TObj *o)
{
    char pad[16];
    switch (o->state) {
    case 0:
        if (D_8009C93A == 0)
            break;
        if (D_8009CDCB == 0 || D_8009CDCB == 0xff)
            break;
        if (GAME.playerAP < D_8009C98C)
            break;
        D_8009C93FA[0] = 1;
        D_8009C942A[0] = 1;
        D_8009D2B0 = 2;
        PLAYER.obj.b04 = 5;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->state++;
        break;
    case 1:
        o->w08 = 200;
        setEventComplete(0x27, 0);
        o->state++;
        break;
    case 2:
        if (--o->w08 > 0)
            break;
        D_8009C93FA[0] = 0;
        D_8009C942A[0] = 0;
        ((short*)&PLAYER.obj.wb2)[0] = 0;
        D_1F8001C6 = 0;
        o->b0f = ((unsigned short *)o)[0x12];
        PLAYER.obj.b04 = 1;
        PLAYER.obj.step = 0;
        PLAYER.obj.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
