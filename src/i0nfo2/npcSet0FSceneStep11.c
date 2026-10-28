// FUNC 800fd524 356 I0NFO2
// MATCHING 800fd524 356
#include "TOBJ.H"
extern signed char D_8009C93F, D_8009C942, PLAYER_VISIBLE, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern short PLAYER_X[];
extern unsigned short PLAYER_Y[];
extern short PLAYER_ANIM_FRAME;

void npcSet0FSceneStep11(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C93F = 1;
        D_8009C942 = 1;
        PLAYER_X[0] = 0x120;
        PLAYER_Y[0] = 0x110;
        PLAYER_ANIM_FRAME = 6;
        PLAYER_VISIBLE = 0;
        PLAYER_STATE = 4;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->w08 = 0x91;
        o->state++;
        break;
    case 1:
        if (--o->w08 >= 0) {
            if ((o->w08 & 1) && PLAYER_X[0] < 0x12E) PLAYER_X[0]++;
            PLAYER_Y[0]++;
            if (o->w08 >= 0xB) PLAYER_VISIBLE = 0;
        } else {
            D_8009C93F = 0;
            D_8009C942 = 0;
            PLAYER_VISIBLE = 1;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
