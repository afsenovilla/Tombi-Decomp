// FUNC 800ee5a4 248 I0NFO3
// MATCHING 800ee5a4 248
#include "TOBJ.H"
extern void func_8005A9A4(int a, int b);
extern unsigned char GVARS_PLAYER_LANDED;
extern signed char D_8009C93F, D_8009C942, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP, D_800A60E4;

void tunnelStartEvent4FStep(TObj *o)
{
    switch (o->state) {
    case 0:
        if (GVARS_PLAYER_LANDED != 0) {
            PLAYER_SUBSTATE = 2;
            D_8009C93F = 1;
            D_8009C942 = 1;
            PLAYER_STEP = 0;
            o->w08 = 0xC8;
            func_8005A9A4(0x4F, 0);
            o->state++;
        }
        break;
    case 1:
        if (--o->w08 <= 0) {
            D_8009C942 = 0;
            D_8009C93F = 0;
            D_800A60E4 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
