// FUNC 8011a224 276 X010
// MATCHING 8011a224 276
#include "gvars.h"
#include "TOBJ.H"

extern TObj PLAYER;

int evilPigBagPortalCheckPlayer(TObj *o)
{
    if (GVARS.b3a == 0) goto ret0;
    if (GVARS.b3f | GVARS.useItemPending) return 0;
    if (PLAYER.active == 2) return 0;
    if ((unsigned short)(PLAYER.d->p.whole - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(PLAYER.h->p.whole - o->h->p.whole + (o->box0 + PLAYER.box0)) > o->box1 + PLAYER.box1) {
    ret0:
        return 0;
    }
    return (unsigned short)(PLAYER.y.p.whole - o->y.p.whole + (PLAYER.box2 + o->box2)) <= PLAYER.box3 + o->box3;
}
