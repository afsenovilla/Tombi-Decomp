// FUNC 80104ea0 140 X005
// MATCHING 80104ea0 140
#include "gv6.h"
#include "TOBJ.H"
extern TObj *PLAYER_EXT;
void playerCheckJumpPressedA(TObj *o)
{
    unsigned short m = *(unsigned short *)0x1f8001fc;
    if ((m & *(unsigned short *)0x1f8003c8) || (m & *(unsigned short *)0x1f8003c6)) {
        if (*(volatile unsigned short *)&GV6.s70 & 0x40) {
            PLAYER_EXT->animTimer = 15;
        } else {
            PLAYER_EXT->animTimer = 14;
        }
        o->b9c = 0;
        o->velY = 0;
        o->substep = 2;
    }
}
