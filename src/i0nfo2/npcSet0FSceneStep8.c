// FUNC 800fd0e8 428 I0NFO2
// MATCHING 800fd0e8 428
#include "TOBJ.H"
extern unsigned char D_8009C93F;
extern unsigned char D_8009C942;
extern unsigned char D_8009CFA1;
extern unsigned char PLAYER_VISIBLE;
extern unsigned char PLAYER_STATE;
extern unsigned char PLAYER_SUBSTATE;
extern unsigned char PLAYER_STEP;
extern short PLAYER_X;
extern short PLAYER_Y[];
extern short PLAYER_ANIM_FRAME;

void npcSet0FSceneStep8(TObj *o)
{
    switch (o->state) {
    case 0:
        D_8009C93F = 1;
        D_8009C942 = 1;
        PLAYER_X = 0x74;
        PLAYER_Y[0] = 0x1AA;
        PLAYER_ANIM_FRAME = 2;
        PLAYER_VISIBLE = 0;
        PLAYER_STATE = 4;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->state = D_8009CFA1 ? 1 : 2;
        o->w08 = 0x54;
        break;
    case 1:
        if (--o->w08 < 0) goto done;
        PLAYER_Y[0]--;
        if (o->w08 >= 0xB) {
            PLAYER_VISIBLE = 0;
        }
        break;
    case 2:
        if (--o->w08 < 0) goto fail;
        PLAYER_Y[0]--;
        if (o->w08 == 0x2A) {
            D_8009CFA1 = 1;
        }
    chk:
        if (o->w08 >= 0xB) {
            PLAYER_VISIBLE = 0;
        }
        break;
    fail:
        D_8009CFA1 = 2;
    done:
        D_8009C93F = 0;
        D_8009C942 = 0;
        PLAYER_VISIBLE = 1;
        PLAYER_STATE = 1;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
