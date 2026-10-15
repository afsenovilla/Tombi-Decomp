// FUNC 8011c4ec 304 X002
// MATCHING 8011c4ec 304
#include "gcd.h"
#include "gv2.h"
#include "player.h"
#include "TOBJ.H"

extern void pigGateDiscoveryScene(TObj *);
extern void pigGateSevenFriendsScene(TObj *);

void stepPigGateScene(TObj *o)
{
    switch (o->step) {
    case 0:
        if (GCD.bce41 == 0xff) {
            o->step = 3;
            o->state = 0;
        } else if (GCD.bcff7 == 6) {
            o->step = 2;
            o->state = 0;
        } else if (GCD.bce41 == 0 && GV2.bc3 == 0x7f) {
            o->step = 1;
            o->state = 0;
        } else {
            o->step = 0;
            o->state = 0;
        }
        break;
    case 1:
        pigGateDiscoveryScene(o);
        break;
    case 2:
        pigGateSevenFriendsScene(o);
        break;
    case 3:
        if ((unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - 0x92) < 0x20) {
            (*(unsigned char*)&PLAYER.obj.da0) = 1;
            (*(unsigned char*)&PLAYER.obj.wa8) = 8;
        } else {
            (*(unsigned char*)&PLAYER.obj.da0) = 0;
            (*(unsigned char*)&PLAYER.obj.wa8) = 0;
        }
        break;
    }
}
