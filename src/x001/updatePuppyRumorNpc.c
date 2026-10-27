// FUNC 8013bab0 372 X001
// MATCHING 8013bab0 372
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern signed char GAME_PLAYER_MODE;
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8003C4;
extern unsigned char D_800A60F8;
extern void spawnPuppyRumorNpcs(TObj *);
extern void puppyRumorTalk(TObj *);
extern void PoolFree_1F800210(TObj *);

static __inline__ short check(TObj *o)
{
    int r = 0;

    if (GCD.bceab != 0 && GCD.bceab < 4 && PLAYER.obj.b69 != 0
        && (unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - 0xaad) < 0x40 && PLAYER.obj.y.p.whole >= -0x10f) {
        if (o->animTimer != 0) {
            r = 1;
        } else if (GAME_PLAYER_MODE == 1 && (D_1F8001FC & D_1F8003C4)) {
            r = 1;
            GVARS.b42 = 1;
            D_800A60F8 = 1;
            o->animTimer = 1;
        }
    }
    return r;
}

void updatePuppyRumorNpc(TObj *o)
{
    switch (o->b04) {
    case 0:
        spawnPuppyRumorNpcs(o);
        break;
    case 1:
        if (check(o)) puppyRumorTalk(o);
        break;
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
