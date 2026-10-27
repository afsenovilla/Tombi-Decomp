// FUNC 800fd3c0 356 I0NFO2
// MATCHING 800fd3c0 356
#include "TOBJ.H"
extern signed char D_8009C93F, D_8009C942, D_800A6039, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern short PLAYER_X[];
extern unsigned short PLAYER_Y[];
extern short D_800A6066;

void func_800FD3C0(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C93F = 1;
        D_8009C942 = 1;
        PLAYER_X[0] = 0x12E;
        PLAYER_Y[0] = 0x197;
        D_800A6066 = 2;
        D_800A6039 = 0;
        PLAYER_STATE = 4;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->w08 = 0x91;
        o->state++;
        break;
    case 1:
        if (--o->w08 >= 0) {
            if ((o->w08 & 1) && PLAYER_X[0] > 0x120) PLAYER_X[0]--;
            PLAYER_Y[0]--;
            if (o->w08 >= 0xB) D_800A6039 = 0;
        } else {
            D_8009C93F = 0;
            D_8009C942 = 0;
            D_800A6039 = 1;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
