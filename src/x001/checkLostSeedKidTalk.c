// FUNC 80139b2c 292 X001
// MATCHING 80139b2c 292
#include "gcd.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"
#include "gamestate.h"

extern unsigned char D_800A60F8;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC, D_1F8003C4;

int checkLostSeedKidTalk(TObj *o)
{
    TObj *p = *(TObj **)&o->category;
    int r = 0;

    if (GCD.bcdb2 >= 2) return 0;
    if (!(GAME.area00_eventControl & 0x100) && PLAYER.obj.b04 == 1 && PLAYER.obj.b69 &&
        (unsigned short)((*(unsigned short*)&PLAYER.obj.y.p.whole) + 0x50a) < 0x10 &&
        (unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - p->h->p.whole + 0x30) < 0x60) {
        if (*(unsigned short *)&o->anim) {
            r = 1;
        } else if (D_8009D2B0 == PLAYER.obj.b04 && (D_1F8001FC & D_1F8003C4)) {
            r = 1;
            *(unsigned short *)&o->anim = 1;
            GVARS.b42 = 1;
            D_800A60F8 = 1;
        }
    }
    return r;
}
