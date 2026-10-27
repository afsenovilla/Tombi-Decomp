// FUNC 8013a1f4 228 X001
// MATCHING 8013a1f4 228
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern signed char GAME_PLAYER_MODE;
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8003C4;
extern unsigned char D_800A60F8;

int checkPuppyRumorNpcTalk(TObj *o)
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
