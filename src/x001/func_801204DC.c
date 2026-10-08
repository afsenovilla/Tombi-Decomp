// FUNC 801204dc 276 X001
// MATCHING 801204dc 276
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C93A, D_8009C93F, D_8009C940;

int func_801204DC(TObj *o)
{
    if (D_8009C93A == 0) goto ret0;
    if (D_8009C93F | D_8009C940) return 0;
    if (D_800A6038.active == 2) return 0;
    if ((unsigned short)(D_800A6038.d->p.whole - o->d->p.whole + 0x2d) >= 0x5b) return 0;
    if ((unsigned short)(D_800A6038.h->p.whole - o->h->p.whole + (o->box0 + D_800A6038.box0)) > o->box1 + D_800A6038.box1) {
    ret0:
        return 0;
    }
    return (unsigned short)(D_800A6038.y.p.whole - o->y.p.whole + (D_800A6038.box2 + o->box2)) <= D_800A6038.box3 + o->box3;
}
