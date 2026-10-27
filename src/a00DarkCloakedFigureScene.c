// FUNC 80136ce0 1676 X000
// MATCHING 80136ce0 1676
#include "TOBJ.H"
extern TObj PLAYER;
extern unsigned short PLAYER_ANIM_FRAME;
extern TObj *DAT_800a60c8;
extern unsigned char DAT_800a60f8;
extern unsigned char DAT_8009cebe;
extern unsigned char DAT_8009cebeA[];
extern unsigned char DAT_8009cebeR;
extern unsigned char DAT_8009c93e, DAT_8009c93f, DAT_8009c942;
extern unsigned char GAME_PLAYER_MODE;
extern int DAT_8009c984;
extern int GAME_AREA00_EVENT_CONTROL[];
extern char DAT_80010748[];
extern void AnimLoadDuration(TObj *);
extern TObj *showMessageBox(int, int, int, int);
extern void setEventStarted(int, int, int);
extern void setEventComplete(int, int);

#define W1C(o) (*(short *)&(o)->category)

void a00DarkCloakedFigureScene(TObj *o)
{
    TObj *t = *(TObj **)&o->timer;

    switch (o->step) {
    case 0:
        o->w08 = 0x14;
        PLAYER.animFrame = 0;
        PLAYER.b04 = 1;
        DAT_8009c93f = 1;
        DAT_8009c93e = 1;
        DAT_8009c942 = 1;
        PLAYER.step = 0;
        PLAYER.state = 0;
        GAME_PLAYER_MODE = 0;
        PLAYER.anim = DAT_80010748;
        AnimLoadDuration(&PLAYER);
        o->step++;
        break;
    case 1:
        if (--o->w08 != 0)
            break;
        DAT_8009cebeA[0] = 0;
        if (DAT_8009c984 & 0x80)
            { if (DAT_8009c984 & 8) { if (DAT_8009c984 & 0x10) DAT_8009cebeA[0] = 6; else DAT_8009cebeA[0] = 4; } else DAT_8009cebeA[0] = 5; }
        DAT_800a60c8 = showMessageBox(6, DAT_8009cebeR, 0xe0, 0x6c);
        DAT_8009c984 |= 0x80;
        o->step++;
        break;
    case 2:
        if (DAT_800a60c8->b04 != 2)
            break;
        W1C(o) = 0;
        switch (DAT_8009cebe) {
        case 0:
            DAT_800a60c8->b04 = 3;
            DAT_8009cebe = 1;
            o->w08 = 1;
            o->step = 4;
            W1C(o) = 1;
            break;
        case 1:
            DAT_800a60c8->b04 = 3;
            DAT_8009cebe = 2;
            setEventStarted(5, 0, 0);
            W1C(o) = 1;
            o->w08 = 0x168;
            o->step = 4;
            break;
        case 2:
            DAT_800a60c8->b04 = 3;
            if (DAT_8009c984 & 0x18) {
                DAT_8009cebe = 3;
                DAT_800a60c8 = showMessageBox(6, 3, 0xe0, 0x6c);
                o->step = 2;
                W1C(o) = 1;
            } else {
                o->w08 = 4;
                o->step = 3;
                W1C(o) = 1;
            }
            break;
        case 3:
            DAT_800a60c8->b04 = 3;
            { if (DAT_8009c984 & 8) { if (DAT_8009c984 & 0x10) DAT_8009cebe = 7; else DAT_8009cebe = 4; } else DAT_8009cebe = 5; }
            DAT_800a60c8 = showMessageBox(6, DAT_8009cebeR, 0xe0, 0x6c);
            W1C(o) = 1;
            o->step = 2;
            break;
        case 4:
            DAT_800a60c8->b04 = 3;
            if (DAT_8009c984 & 0x10) {
                DAT_8009cebe = 3;
                DAT_800a60c8 = showMessageBox(6, 3, 0xe0, 0x6c);
                o->step = 2;
                W1C(o) = 1;
            } else {
                PLAYER.b04 = 1;
                PLAYER.step = 0;
                PLAYER.state = 0;
                DAT_800a60f8 = 0;
                o->b04 = 0;
                o->step = 0;
                o->state = 0;
                DAT_8009c93e = 0;
                DAT_8009c93f = 0;
                DAT_8009c942 = 0;
                W1C(o) = 0;
            }
            break;
        case 5:
            DAT_800a60c8->b04 = 3;
            if (DAT_8009c984 & 8) {
                DAT_8009cebe = 3;
                DAT_800a60c8 = showMessageBox(6, 3, 0xe0, 0x6c);
                o->step = 2;
                W1C(o) = 1;
            } else {
                PLAYER.b04 = 1;
                PLAYER.step = 0;
                PLAYER.state = 0;
                DAT_800a60f8 = 0;
                o->b04 = 1;
                o->step = 0;
                o->state = 0;
                DAT_8009c93f = 0;
                DAT_8009c942 = 0;
                DAT_8009c93e = 0;
                W1C(o) = 0;
            }
            break;
        case 6:
            DAT_800a60c8->b04 = 3;
            DAT_8009cebe = 7;
            DAT_800a60c8 = showMessageBox(6, 7, 0xe0, 0x6c);
            W1C(o) = 1;
            o->step = 2;
            break;
        case 7:
            DAT_800a60c8->b04 = 3;
            setEventComplete(5, 0);
            DAT_8009cebe = 8;
            o->w08 = 0x168;
            o->step = 5;
            W1C(o) = 1;
            break;
        case 8:
            DAT_800a60c8->b04 = 3;
            DAT_8009cebe = 9;
            DAT_800a60c8 = showMessageBox(6, 9, 0xe0, 0x6c);
            W1C(o) = 1;
            o->step = 2;
            break;
        case 9:
            DAT_800a60c8->b04 = 3;
            DAT_8009cebe = 0xb;
            PLAYER.b04 = 1;
            PLAYER.step = 0;
            PLAYER.state = 0;
            t->b04 = 2;
            t->step = 2;
            t->state = 0;
            o->b04 = 2;
            o->step = 0;
            o->state = 0;
            DAT_8009c93e = 0;
            DAT_8009c93f = 0;
            DAT_8009c942 = 0;
            W1C(o) = 0;
            DAT_800a60f8 = 0;
            break;
        }
        break;
    case 3:
        if (--o->w08 != 0)
            break;
        PLAYER.timer = 0x50;
        PLAYER.active = 3;
        PLAYER.b04 = 5;
        PLAYER.animFrame = 0;
        PLAYER_ANIM_FRAME = 0;
        PLAYER.step = 1;
        PLAYER.state = 0;
        PLAYER.substep = 0;
        o->b04 = 0;
        o->step = 0;
        W1C(o) = 0;
        DAT_800a60f8 = 0;
        DAT_8009c93e = 0;
        DAT_8009c93f = 0;
        DAT_8009c942 = 0;
        GAME_AREA00_EVENT_CONTROL[0] |= 0x80;
        break;
    case 4:
        if (--o->w08 != 0)
            break;
        DAT_800a60c8 = showMessageBox(6, DAT_8009cebe, 0xe0, 0x6c);
        o->step = 2;
        W1C(o) = 1;
        break;
    case 5:
        if (--o->w08 != 0)
            break;
        setEventStarted(7, 0, 0);
        o->w08 = 300;
        o->step = 4;
        W1C(o) = 1;
        break;
    }
}
