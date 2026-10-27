// FUNC 80120ba0 200 X009
// MATCHING 80120ba0 200
#include "spad.h"
#include "TOBJ.H"

extern unsigned char PLAYER, PLAYER_STATE, PLAYER_SUBSTATE, PLAYER_STEP;
extern short boxesOverlapSigned(TObj *, TObj *);
extern int resolveHitResultC(TObj *, TObj *);

void jellyfishWeaponHit(TObj *o, TObj *e)
{
    if (boxesOverlapSigned(o, e) == -1) return;
    if (resolveHitResultC(o, e)) {
        if (!(e->active & 2)) {
            unsigned short f;
            e->active = 6;
            f = o->animFrame;
            e->b04 = 2;
            e->step = 0;
            e->state = 0;
            e->animFrame = f & 1;
        }
        if (e->b6a == 2) {
            PLAYER = 1;
            PLAYER_STATE = 1;
            PLAYER_SUBSTATE = 0;
            PLAYER_STEP = 0;
        }
        e->b6a = 0;
    }
    SPAD.s19e = 0;
}
