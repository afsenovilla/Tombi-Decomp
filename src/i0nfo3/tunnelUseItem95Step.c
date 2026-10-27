// FUNC 800ee414 400 I0NFO3
// MATCHING 800ee414 400
#include "TOBJ.H"
typedef struct { short a, x, b, y, c, d; } BoxXY;
extern int func_8002DCC8(int a, int b, BoxXY *p);
extern void func_8005A9A4(int e, int a);
extern unsigned char D_8009C93F;
extern unsigned char GVARS_USE_ITEM_PENDING[];
extern unsigned char GVARS_USE_ITEM_ID;
extern unsigned char D_8009C942;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern TObj *PLAYER_MESSAGE_BOX;
extern unsigned char D_800A60E4;

static __inline__ short usingItem(unsigned char *p)
{
    short r = 0;
    if (p[0] != 0) {
        r = GVARS_USE_ITEM_ID == 0x95;
    }
    return r;
}

void tunnelUseItem95Step(TObj *o)
{
    BoxXY b;
    unsigned char *u;
    unsigned char *v;
    switch (o->state) {
    case 0:
        u = GVARS_USE_ITEM_PENDING;
        v = u;
        if (usingItem(v)) {
            b.x = 0x90;
            b.y = 0x40;
            PLAYER_SUBSTATE = 2;
            PLAYER_STEP = 0;
            u--;
            u[0] = 1;
            D_8009C942 = 1;
            u[1] = 0;
            PLAYER_MESSAGE_BOX = (TObj *)func_8002DCC8(6, 0, &b);
            o->state++;
            break;
        }
        goto done;
    case 1:
        if (PLAYER_MESSAGE_BOX->b04 == 2) {
            PLAYER_MESSAGE_BOX->b04 = 3;
            func_8005A9A4(0x7D, 0);
            o->w08 = 0xC8;
            o->state = 9;
        }
        break;
    case 9:
        if (--o->w08 <= 0) {
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
        done:
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
