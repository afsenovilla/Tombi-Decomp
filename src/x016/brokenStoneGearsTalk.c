// FUNC 801174b8 452 X016
// MATCHING 801174b8 452
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
extern TObj PLAYER;
extern TObj *PLAYER_MESSAGE_BOX;
extern unsigned char DAT_8009c93e, DAT_8009c93f, DAT_8009c942;
extern unsigned char DAT_8009ce06;
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventStarted(int, int, int);

void brokenStoneGearsTalk(TObj *o)
{
    B12 v;

    switch (o->state) {
    case 0:
        PLAYER.b04 = 5;
        PLAYER.step = 0;
        PLAYER.state = 0;
        DAT_8009c93f = 1;
        DAT_8009c942 = 1;
        o->state++;
        break;
    case 1:
        DAT_8009c93e = 1;
        o->state++;
        break;
    case 2:
        v = *(B12 *)&PLAYER.a;
        PLAYER_MESSAGE_BOX = showMessageBoxDirect(4, 0, &v);
        o->state++;
        break;
    case 3:
        if (PLAYER_MESSAGE_BOX->b04 != 2)
            break;
        PLAYER_MESSAGE_BOX->b04 = 3;
        if (DAT_8009ce06 == 0) {
            setEventStarted(0x62, 0, 0);
            o->w08 = 200;
            o->state++;
        } else {
            o->state = 0xf;
        }
        break;
    case 4:
        if (--o->w08 > 0)
            break;
        o->state = 0xf;
        break;
    case 15:
        DAT_8009c93f = 0;
        DAT_8009c942 = 0;
        DAT_8009c93e = 0;
        PLAYER.wb2 = 0;
        PLAYER.b04 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
