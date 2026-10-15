// FUNC 8011a0a8 108 X002
// MATCHING 8011a0a8 108
#include "gcd.h"
#include "gv2.h"
#include "TOBJ.H"

void selectSevenFriendsSceneStep(TObj *o)
{
    unsigned char c = GCD.bce41;

    if (c == 0xff) {
        o->step = 3;
    } else if (GCD.bcff7 == 6) {
        o->step = 2;
    } else if (c == 0 && GV2.bc3 == 0x7f) {
        o->step = 1;
    } else {
        o->step = 0;
    }
    o->state = 0;
}
