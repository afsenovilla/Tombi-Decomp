// FUNC 8011fa6c 104 X003
// MATCHING 8011fa6c 104
#include "gcd.h"
#include "TOBJ.H"

extern short FUN_8004461c(TObj *a, TObj *b);

void komaPigIntroWeaponHit(TObj *o, TObj *p)
{
    if (FUN_8004461c(o, p) != -1) {
        if (GCD.bcdc3 != 0) {
            p->b04 = 3;
        } else {
            p->state = 2;
        }
        p->active = 2;
    }
}
