// FUNC 8012e9e0 388 X010
// MATCHING 8012e9e0 388
#include "TOBJ.H"

extern TObj PLAYER;
extern unsigned short D_8009C962;
extern unsigned char D_8009D2C3;
extern short D_8009C944;
extern short D_8009C946;
void PoolFree_1F800210(TObj *o);

void func_8012E9E0(TObj *o)
{
    switch (o->b04) {
    case 0:
        if (D_8009C962 == 1) {
            o->y.raw = -0xea;
        } else if (D_8009D2C3 & 0x40) {
            o->y.raw = -0x50;
        } else {
            o->y.raw = -0x3b8;
        }
        D_8009C944 = 0;
        D_8009C946 = -0x40;
        o->b04++;
        break;
    case 1: {
        TObj *p = &PLAYER;
        switch (PLAYER.step) {
        case 0x3e:
            if (PLAYER.y.p.whole < o->y.raw) {
                PLAYER.y.p.whole = o->y.raw;
                { int y = o->y.raw;
                PLAYER.step = 0x3d;
                PLAYER.state = 0;
                PLAYER.w78 = y; }
            }
            break;
        case 2:
        case 0xd:
            if (o->y.raw < p->y.p.whole) {
                p->y.p.whole = o->y.raw;
                { int y = o->y.raw;
                p->step = 0x3d;
                p->state = 0;
                p->w78 = y; }
            }
            break;
        }
        break;
    }
    case 2:
        o->b04 = 3;
        break;
    case 3:
        PoolFree_1F800210(o);
        break;
    }
}
