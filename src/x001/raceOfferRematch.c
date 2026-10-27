// FUNC 8013af10 484 X001
// MATCHING 8013af10 484
#include "gcd.h"
#include "gvars.h"
#include "TOBJ.H"
#include "gamestate.h"
#include "gametask.h"
extern TObj PLAYER;
extern unsigned short D_800A6066;
extern unsigned short D_800A6066x[]; /* second name for D_800A6066: forces the reloads after the store */
extern TObj *D_800A60C8[];
extern Fix16 *PLAYER_H;
extern unsigned char PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern char D_80010748[];
typedef struct { char p[0x4c]; short w4c, w4e; } SC;
extern void AnimLoadDuration(TObj *);
extern TObj *showMessageBox(int, int, int, int);

void raceOfferRematch(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;

    switch (o->step) {
    case 0:
        GVARS.b3f = 1;
        GVARS.b3e = 1;
        GVARS.b42 = 1;
        PLAYER.anim = D_80010748;
        AnimLoadDuration(&PLAYER);
        D_800A6066 = 0;
        e->b04 = 1;
        e->step = 0;
        e->state = 0;
        PLAYER_STATE = 5;
        PLAYER_SUBSTATE = 0;
        PLAYER_STEP = 0;
        D_800A60C8[0] = showMessageBox(2, 0x1e, 0x80, 0x6c);
        D_800A6066x[0] = PLAYER_H->p.whole > e->h->p.whole;
        e->animFrame = PLAYER_H->p.whole < e->h->p.whole;
        o->step++;
    case 1:
        p = D_800A60C8[0];
        if (p->b04 == 2) {
            p->b04 = 3;
            GAME.fadeScreenControl = 3;
            o->step++;
        }
        break;
    case 2:
        if (GAME.fadeScreenControl == 1) {
            GCD.bce59 = 1;
            GAME.nextArea = GAME.selectedArea;
            GCD.bcede = 0;
            GCD.bcedf = 0;
            GAME.nextSpawnPoint = 0;
            GAME.nextSection = GAME.selectedSection;
            TASK_C->mode = 7;
            TASK_C->step = 0;
        }
        break;
    }
}
