// FUNC 800f8d5c 468 I0NFO2
// MATCHING 800f8d5c 468
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void topdownActorSetAnim(TObj *o, unsigned short i, short n);
extern unsigned char D_8009C93A;
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDD5;
extern TObj *D_8009F304;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned short PLAYER_Y;
extern short D_800A6066;
extern void *PLAYER_H;

void func_800F8D5C(TObj *o)
{
    TObj *n = D_8009F304;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        S16(PLAYER_H, 2) = n->h->p.whole + 0x20;
        PLAYER_Y = n->y.p.whole;
        o->state++;
    case 1:
        if (D_8009C93A != 0) {
            topdownActorSetAnim(n, 8, 0);
            n->step = 2;
            n->state = 0;
            PLAYER_SUBSTATE = 4;
            D_8009C93F = 1;
            D_8009C942 = 1;
            D_800A6066 = 4;
            PLAYER_STEP = 0;
            o->state = 0x12;
        }
        break;
    case 9:
        n->b68 = 0;
        n->b6a = 0;
        n->step = 0;
        n->state = 0;
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        o->step = 0;
        o->state = 0;
        break;
    case 0x12:
        p = *(Pos6 *)&n->a;
        S32(n, 0x90) = func_8002DCC8(2, 0x60, &p);
        o->state++;
        break;
    case 0x13:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            topdownActorSetAnim(n, 0, 0);
            D_8009CDD5 = 2;
            o->state = 9;
        }
        break;
    }
}
