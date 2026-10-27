// FUNC 800fe848 724 I0NFO2
// MATCHING 800fe848 724
#include "TOBJ.H"
typedef struct { short s[6]; } Pos6;
extern void func_80026C50(int a, int b, int c);
extern TObj *func_8002DCC8(int a, int b, Pos6 *p);
extern void func_8005A9A4(int a, int b);
extern void topdownA19ActorSetDirAnim(TObj *o, int a, int b, int c);
extern void func_800FE410(TObj *o);
extern unsigned char D_8009C93F;
extern unsigned char GVARS_USE_ITEM_PENDING;
extern unsigned char GVARS_USE_ITEM_ID;
extern unsigned char D_8009C942;
extern unsigned char D_8009CDF6;
extern unsigned char D_8009CE56;
extern unsigned char D_8009CFCF;
extern unsigned char D_8009CFD7;
extern TObj *D_8009F2E8;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern TObj PLAYER;
extern TObj *PLAYER_MESSAGE_BOX;
extern unsigned char D_800A60E4;

void stepNpcSet11(TObj *o)
{
    Pos6 p;
    TObj *n = D_8009F2E8;
    int d;
    switch (o->step) {
    case 0:
        if (D_8009CFD7 != 0) {
            if (GVARS_USE_ITEM_ID >= 0x1E && GVARS_USE_ITEM_ID < 0x20 && D_8009CDF6 != 0xFF) {
                o->step = 1;
            }
            D_8009CFD7 = 0;
            GVARS_USE_ITEM_PENDING = 0;
        } else if (D_8009CE56 != 0xFF && D_8009CFCF != 0) {
            o->step = 4;
        }
        break;
    case 1:
        p = *(Pos6 *)&PLAYER.a;
        D_8009C93F = 1;
        D_8009C942 = 1;
        PLAYER_STATE = 4;
        PLAYER_SUBSTATE = 3;
        PLAYER_STEP = 0;
        n->step = 2;
        n->state = 0;
        d = 3;
        if (PLAYER.y.p.whole < n->y.p.whole) d = 1;
        n->animFrame = d;
        topdownA19ActorSetDirAnim(n, 8, 0, d);
        PLAYER_MESSAGE_BOX = func_8002DCC8(3, 1, &p);
        o->step++;
        break;
    case 2:
        if (PLAYER_MESSAGE_BOX->b04 == 2) {
            PLAYER_MESSAGE_BOX->b04 = 3;
            func_8005A9A4(0x52, 1);
            func_80026C50(0x37, 1, 1);
            func_80026C50(0x37, 1, 1);
            o->w08 = 0xC8;
            o->step++;
        }
        break;
    case 3:
        if (--o->w08 <= 0) {
            d = 3;
            if (PLAYER.y.p.whole < n->y.p.whole) d = 1;
            n->animFrame = d;
            topdownA19ActorSetDirAnim(n, 0, 0, d);
            GVARS_USE_ITEM_PENDING = 0;
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            n->b04 = 1;
            n->step = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    case 4:
        func_800FE410(o);
        break;
    }
}
