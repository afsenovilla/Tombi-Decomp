// FUNC 8011d2f8 276 X003
// MATCHING 8011d2f8 276
#include "gvars.h"
#include "TOBJ.H"

extern unsigned char PLAYER;
extern Fix16 *PLAYER_D, *PLAYER_H;
extern unsigned short PLAYER_Y;
extern short D_800A60A4, D_800A60A6, D_800A60A8, D_800A60AA;

int evilPigBagPortalCheckPlayer(TObj *o)
{
    if (GVARS.playerLanded == 0) goto ret0;
    if (GVARS.b3f | GVARS.useItemPending) return 0;
    if (PLAYER == 2) return 0;
    if ((unsigned short)(PLAYER_D->p.whole - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(PLAYER_H->p.whole - o->h->p.whole + (o->box0 + D_800A60A4)) > o->box1 + D_800A60A6) {
    ret0:
        return 0;
    }
    return (unsigned short)(PLAYER_Y - o->y.p.whole + (D_800A60A8 + o->box2)) <= D_800A60AA + o->box3;
}
