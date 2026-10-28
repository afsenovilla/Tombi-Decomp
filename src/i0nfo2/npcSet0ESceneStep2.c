// FUNC 800fbc34 312 I0NFO2
// MATCHING 800fbc34 312
#include "TOBJ.H"
extern unsigned char D_8009CFA8[];
extern signed char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;

void npcSet0ESceneStep2(TObj *o)
{
    switch (o->state) {
    case 0:
        PLAYER_STATE = 4;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->w08 = 0x1E;
        o->state++;
        break;
    case 1:
        if (--o->w08 <= 0) {
            D_8009CFA8[0]++;
            o->w08 = 0x1E;
            o->state++;
        }
        break;
    case 2:
        if (--o->w08 <= 0) {
            D_8009CFA8[0]++;
            o->state++;
        }
        break;
    case 3:
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        D_8009CFA8[0]++;
        o->step = 0;
        o->state = 0;
        break;
    }
}
