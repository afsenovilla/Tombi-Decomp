// FUNC 8012e554 368 X004
// MATCHING 8012e554 368
#include "gcd.h"
#include "player.h"
#include "TOBJ.H"

void largeKeyHoleScene(TObj *o);
void smallKeyHoleScene(TObj *o);

void stepKeyHoles(TObj *o)
{
    switch (o->step) {
    case 0:
        if ((*(unsigned char*)((char*)&PLAYER.obj.da0 + 2)) != 1)
            break;
        if ((unsigned short)((*(unsigned short **)&PLAYER.obj.h)[1] - 0x370) < 0x28 && PLAYER.obj.y.p.whole == -0x1d6) {
            if (GCD.bcddb == 0) {
                o->step = 1;
                o->state = 0;
            } else if (GCD.bd0d9 != 0) {
                o->step = 1;
                o->state = 2;
            }
        }
        if ((unsigned short)((*(unsigned short **)&PLAYER.obj.h)[1] - 0x3c8) < 0x28 && PLAYER.obj.y.p.whole == -0x2e4) {
            if (GCD.bcdd7 == 0) {
                o->step = 2;
                o->state = 0;
            } else if (GCD.bd0da != 0) {
                o->step = 2;
                o->state = 2;
            }
        }
        break;
    case 1:
        largeKeyHoleScene(o);
        break;
    case 2:
        smallKeyHoleScene(o);
        break;
    }
}
