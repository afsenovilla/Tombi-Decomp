// FUNC 800fc844 416 I0NFO2
// MATCHING 800fc844 416
#include "TOBJ.H"
#include "raw7.h"
typedef struct { short s[6]; } Pos6;
extern int func_8002DCC8(int a, int b, Pos6 *p);
extern void topdownA17ActorSetDirAnim(TObj *o, int i, short n);
extern void func_8005A8A8(int e, int a, int b);
extern unsigned char D_8009C93E;
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDAB[];
extern unsigned char D_8009CF9E;
extern TObj *D_8009F2E0;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern unsigned char D_800A60E4;

void npcSet0FSceneStep2(TObj *o)
{
    TObj *n = D_8009F2E0;
    Pos6 p;
    TObj *b;
    switch (o->state) {
    case 0:
        p = *(Pos6 *)&n->a;
        S32(n, 0x90) = func_8002DCC8(2, 6, &p);
        topdownA17ActorSetDirAnim(n, 8, 0);
        o->state = 1;
        PLAYER_SUBSTATE = 2;
        PLAYER_STEP = 0;
        D_8009C93F = 1;
        D_8009C942 = 1;
        break;
    case 1:
        b = (TObj *)S32(n, 0x90);
        if (b->b04 == 2) {
            b->b04 = 3;
            D_8009CDAB[0]++;
            o->state = 2;
            func_8005A8A8(0x63, 0, 0);
            D_8009CF9E++;
        }
        break;
    case 2:
        D_8009C93F = 0;
        D_8009C93E = 0;
        D_8009C942 = 0;
        D_800A60E4 = 0;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
