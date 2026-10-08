// FUNC 8011d2f8 276 X003
// MATCHING 8011d2f8 276
#include "TOBJ.H"

extern unsigned char D_8009C93A, D_8009C93F, D_8009C940;
extern unsigned char D_800A6038;
extern Fix16 *D_800A607C, *D_800A6078;
extern unsigned short D_800A604E;
extern short D_800A60A4, D_800A60A6, D_800A60A8, D_800A60AA;

int func_8011D2F8(TObj *o)
{
    if (D_8009C93A == 0) goto ret0;
    if (D_8009C93F | D_8009C940) return 0;
    if (D_800A6038 == 2) return 0;
    if ((unsigned short)(D_800A607C->p.whole - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + (o->box0 + D_800A60A4)) > o->box1 + D_800A60A6) {
    ret0:
        return 0;
    }
    return (unsigned short)(D_800A604E - o->y.p.whole + (D_800A60A8 + o->box2)) <= D_800A60AA + o->box3;
}
