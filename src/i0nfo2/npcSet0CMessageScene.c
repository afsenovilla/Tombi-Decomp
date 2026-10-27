// FUNC 800fa84c 1820 I0NFO2
// MATCHING 800fa84c 1820
#include "TOBJ.H"
#include "raw7.h"
typedef struct { char c[12]; } B12;
extern TObj PLAYER;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern B12 PLAYER_POS;
extern short PLAYER_Y;
extern short PLAYER_ANIM_FRAME;
extern Fix16 *PLAYER_H;
extern TObj *D_800A60C8;
extern TObj *SCRIPT_OBJECTS;
extern TObj *D_8009F2E0;
extern TObj *D_8009F2E4;
extern unsigned char D_8009C93A;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDD6;
extern unsigned char D_8009CEF4;
extern unsigned char D_8009CF00;
extern unsigned char D_8009CF01;
extern unsigned char D_8009CF02[];
extern unsigned char D_8009CF04;
extern unsigned char D_8009CFC9;
extern TObj *showMessageBoxDirect(int, int, B12 *);
extern void setEventComplete(int, int);
extern void removeItemFromInventory(int, int);
extern void AnimAdvance(TObj *o);

void npcSet0CMessageScene(TObj *o)
{
    B12 v;
    TObj *a = SCRIPT_OBJECTS;
    TObj *b = D_8009F2E0;
    TObj *c = D_8009F2E4;

    switch (o->state) {
    case 0:
        if (D_8009C93A != 0) {
            if (D_8009CDD6 == 0xFF) {
                o->state = 3;
            } else {
                PLAYER_SUBSTATE = 4;
                PLAYER_ANIM_FRAME = 6;
                PLAYER_STEP = 0;
                D_8009C93F = 1;
                D_8009C942 = 1;
                o->w08 = 1;
                o->state++;
            }
        }
        break;
    case 1:
        if (--o->w08 <= 0) {
            a->animFrame = 1;
            v = PLAYER_POS;
            D_800A60C8 = showMessageBoxDirect(2, 0, &v);
            o->state++;
        }
        break;
    case 2:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            setEventComplete(0x32, 0);
            o->w08 = 2;
            a->b68 = 0;
            a->b6a = 0;
            a->step = 0;
            a->state = 0;
            b->b68 = 0;
            b->b6a = 0;
            b->step = 0;
            b->state = 0;
            c->b68 = 0;
            c->b6a = 0;
            c->step = 0;
            c->state = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            o->state++;
        }
        break;
    case 3:
        if (a->b68 != 0) {
            v = PLAYER_POS;
            PLAYER_SUBSTATE = 4;
            D_8009C93F = 1;
            D_8009C942 = 1;
            PLAYER_STEP = 0;
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                D_800A60C8 = showMessageBoxDirect(2, 2, &v);
                o->state = 10;
            } else {
                D_800A60C8 = showMessageBoxDirect(2, 1, &v);
                o->state = 6;
            }
        } else if (b->b68 != 0) {
            v = PLAYER_POS;
            PLAYER_SUBSTATE = 4;
            D_8009C93F = 1;
            D_8009C942 = 1;
            PLAYER_STEP = 0;
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCD, &v);
            } else if (D_8009CF01 != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCA, &v);
                removeItemFromInventory(0x40, 1);
                if (D_8009CF00 == 0) {
                    D_8009CF00++;
                }
            } else {
                D_800A60C8 = showMessageBoxDirect(0, 0xC9, &v);
            }
            o->state = 4;
        } else if (c->b68 != 0) {
            v = PLAYER_POS;
            PLAYER_SUBSTATE = 4;
            D_8009C93F = 1;
            D_8009C942 = 1;
            PLAYER_STEP = 0;
            if (D_8009CF00 != 0 && D_8009CF04 != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCD, &v);
            } else if (D_8009CF02[0] != 0) {
                D_800A60C8 = showMessageBoxDirect(0, 0xCC, &v);
                if (D_8009CF04 == 0) {
                    D_8009CF04++;
                }
            } else {
                D_800A60C8 = showMessageBoxDirect(0, 0xCB, &v);
                D_8009CF02[0]++;
            }
            o->state = 5;
        }
        break;
    case 4:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            b->b68 = 0;
            o->w08 = 2;
            o->state = 9;
            b->b68 = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            o->w08 = 2;
            o->state = 3;
        }
        break;
    case 5:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            o->w08 = 2;
            c->b68 = 0;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            o->w08 = 2;
            o->state = 3;
        }
        break;
    case 6:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            a->b68 = 0;
            o->state = 9;
        }
        break;
    case 9:
        a->b68 = 0;
        b->b68 = 0;
        c->b68 = 0;
        PLAYER_STATE = 1;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        PLAYER_SUBSTATE = 0;
        o->state = 3;
        break;
    case 10:
        if (D_800A60C8->b04 == 2) {
            D_800A60C8->b04 = 3;
            S16(PTR(a, 0x40), 2) = S16(PLAYER_H, 2);
            a->y.p.whole = PLAYER_Y - 4;
            o->w08 = 0x3C;
            o->state++;
        }
        break;
    case 11:
        if (--o->w08 <= 0) {
            D_8009CEF4 = 0;
            D_8009CFC9 = 1;
            o->state++;
        }
        break;
    case 12:
        AnimAdvance(&PLAYER);
        PLAYER.y.p.whole += 2;
        S16(PTR(a, 0x40), 2) = S16(PLAYER_H, 2);
        a->y.p.whole = PLAYER_Y - 4;
        break;
    }
}
