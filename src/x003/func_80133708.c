// FUNC 80133708 172 X003
// MATCHING 80133708 172
#include "gcd.h"
#include "gv2.h"
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;

void func_80133708(TObj *o)
{
    if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) != 1) return;
    if ((unsigned short)((*(S2 **)&PLAYER.obj.h)->s2 - 0xc8e) >= 0x28) return;
    if (PLAYER.obj.y.p.whole != -0x7bb) return;
    switch (GCD.bce4e) {
    case 0:
        o->step = 1;
        o->state = 0;
        break;
    case 1:
        if (GV2.bc3 & 2) {
            o->step = 1;
            o->state = 2;
        }
        break;
    }
}
