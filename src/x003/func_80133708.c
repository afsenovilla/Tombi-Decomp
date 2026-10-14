// FUNC 80133708 172 X003
// MATCHING 80133708 172
#include "player.h"
#include "TOBJ.H"
typedef struct { short s0, s2; } S2;
extern unsigned char D_8009CE4E;
extern unsigned char D_8009D2C3;

void func_80133708(TObj *o)
{
    if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) != 1) return;
    if ((unsigned short)((*(S2 **)&PLAYER.obj.h)->s2 - 0xc8e) >= 0x28) return;
    if (PLAYER.obj.y.p.whole != -0x7bb) return;
    switch (D_8009CE4E) {
    case 0:
        o->step = 1;
        o->state = 0;
        break;
    case 1:
        if (D_8009D2C3 & 2) {
            o->step = 1;
            o->state = 2;
        }
        break;
    }
}
