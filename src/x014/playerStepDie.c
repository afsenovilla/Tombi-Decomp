// FUNC 800fe75c 304 X014
// MATCHING 800fe75c 304
#include "TOBJ.H"
extern unsigned char GAME_BITTEN_JUMP_KIND, DAT_8009cf06, GVARS_PLAYER_HURT_STATE, DAT_8009c942;
extern unsigned short GAME, GAME_SELECTED_SECTION;
extern unsigned char *GRABBED_OBJECT;
extern void FUN_8001f110(int);
extern void playJingle(int);

void playerStepDie(TObj *o)
{
    unsigned char *p;
    switch (o->state) {
    case 0:
        if (o->b9e != 0 && (o->b9e == 4 || o->b9e == 7)) *GRABBED_OBJECT = 1;
        p = &GVARS_PLAYER_HURT_STATE;
        if (*p != 1) {
            DAT_8009c942 = 1;
            *p = 1;
            FUN_8001f110(0);
            playJingle(3);
        }
        o->timer = 0xb4;
        *(signed char *)&o->b0f = -8;
        o->visible = 1;
        o->state++;
        break;
    case 1:
        if (--o->timer == 0) {
            GAME_BITTEN_JUMP_KIND = 0;
            DAT_8009cf06 = 0;
            if (GAME != 10 || GAME_SELECTED_SECTION != 0)
                GVARS_PLAYER_HURT_STATE = 2;
            o->state++;
        }
        break;
    case 99:
        break;
    }
}
