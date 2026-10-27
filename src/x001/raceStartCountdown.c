// FUNC 8013a8cc 492 X001
// MATCHING 8013a8cc 492
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
extern TObj PLAYER;
extern TObj *PLAYER_MESSAGE_BOX;
extern unsigned char D_800A60F8, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern TObj *showMessageBox(int, int, int, int);
extern void playerSetAnimIfChangedAt(TObj *, int, int);
extern void FUN_8003fd78(TObj *, int, int);

void raceStartCountdown(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;

    switch (o->step) {
    case 0:
        GVARS.b42 = 1;
        GVARS.b3f = 1;
        PLAYER.animFrame = 0;
        PLAYER.b04 = 5;
        PLAYER.step = 100;
        PLAYER.state = 0;
        playerSetAnimIfChangedAt(&PLAYER, 2, 0);
        o->step++;
    case 1:
        PLAYER.h->raw += 0x10000;
        PLAYER.y.raw += 0x50000;
        FUN_8003fd78(&PLAYER, 0, 0);
        if (PLAYER.h->p.whole >= 0x279) {
            e->animFrame = 0;
            PLAYER_MESSAGE_BOX = showMessageBox(2, 0x19, 0x80, 0x6c);
            playerSetAnimIfChangedAt(&PLAYER, 0, 0);
            o->step++;
        }
        break;
    case 2:
        p = PLAYER_MESSAGE_BOX;
        if (p->b04 == 2) {
            p->b04 = 3;
            GVARS.b3a = 1;
            GVARS.b3f = 0;
            GVARS.b3e = 0;
            GVARS.b42 = 0;
            GCD.bcede = 1;
            D_800A60F8 = 0;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
            e->subtype = 1;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
