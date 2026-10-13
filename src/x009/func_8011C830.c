// FUNC 8011c830 276 X009
// MATCHING 8011c830 276
#include "TOBJ.H"

extern TObj PLAYER;
extern unsigned char D_8009C93A, D_8009C93F, D_8009C940;

int func_8011C830(TObj *o)
{
    if (D_8009C93A == 0) goto ret0;
    if (D_8009C93F | D_8009C940) return 0;
    if (PLAYER.active == 2) return 0;
    if ((unsigned short)(PLAYER.d->p.whole - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(PLAYER.h->p.whole - o->h->p.whole + (o->box0 + PLAYER.box0)) > o->box1 + PLAYER.box1) {
    ret0:
        return 0;
    }
    return (unsigned short)(PLAYER.y.p.whole - o->y.p.whole + (PLAYER.box2 + o->box2)) <= PLAYER.box3 + o->box3;
}
