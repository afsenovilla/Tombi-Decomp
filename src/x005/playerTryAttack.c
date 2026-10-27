// FUNC 8010e328 700 X005
// MATCHING 8010e328 700
#include "gv2.h"
#include "TOBJ.H"
#include "gamestate.h"
typedef struct {
    TObj o;
    unsigned char bc0, bc1, bc2, bc3;
    char pc4[0xcc - 0xc4];
    unsigned char bcc, bcd, bce;
    char pcf[0xe3 - 0xcf];
    signed char be3;
} S;
typedef struct { unsigned char b0, b1, b2, b3, b4, b5, b6, b7; } P;
extern unsigned short BUTTON_MASK_C;
extern unsigned short JOYPAD_STATE;
extern short D_8007A038[];
extern P *PLAYER_EXT;
extern TObj *GRABBED_OBJECT;
extern void spawnPlayerPowerEffect(int);
extern short playerTryTalkToNpc(void);

void playerTryAttack(S *s, short mode)
{
    P *p;
    P *q;
    short v, t;

    if ((JOYPAD_STATE & BUTTON_MASK_C) == 0) return;
    GV2.bb0 = 0;
    s->bcd = 0;
    s->bce = 0;
    switch (GAME.displayExpBar) {
    case 1:
        if (s->bcc == 1) spawnPlayerPowerEffect(0);
        playerTryTalkToNpc();
        return;
    case 2:
        if (s->bcc == 1) spawnPlayerPowerEffect(1);
        playerTryTalkToNpc();
        return;
    }
    s->bc3 = 0;
    if (s->be3 >= D_8007A038[GV2.bb2]) return;
    switch (mode) {
    case 0:
        switch (GV2.bb1) {
        case 1:
            s->o.step = 0x2a;
            s->o.state = 0;
            return;
        case 2:
            s->o.step = 0x2b;
            s->o.state = 0;
            return;
        }
        if (playerTryTalkToNpc() != 0) return;
        if (PLAYER_EXT->b0 != 0) return;
        if (s->bc0 != 0) return;
        s->o.step = 3;
        s->o.state = 0;
        return;
    case 1:
        if (GV2.bb1 == 1 || GV2.bb1 == 2) return;
        p = PLAYER_EXT;
        if (p->b0 != 0) return;
        if (s->bc0 != 0) return;
        if (*(unsigned char *)&s->o.wac == 2) return;
        p->b7 = s->o.animFrame;
        v = s->o.wb2;
        if (v < 0) {
            t = v;
            if (!(s->o.animFrame & 1))
                t = -v;
            s->o.wb2 = t;
        } else {
            if (s->o.animFrame & 1)
                v = -v;
            s->o.wb2 = v;
        }
        s->o.step = 4;
        s->o.state = 0;
        return;
    case 2:
        if (GV2.bb1 == 1 || GV2.bb1 == 2) return;
        if (PLAYER_EXT->b0 != 0) return;
        if (s->bc0 != 0) return;
        PLAYER_EXT->b4 = 0;
        GRABBED_OBJECT->b69 = 4;
        s->o.velH = 0;
        s->o.velV = 0;
        s->o.velX = 0;
        s->o.velY = 0;
        s->o.step = 0x18;
        s->o.state = 0;
        break;
    }
}
