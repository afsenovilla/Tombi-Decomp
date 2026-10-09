// FUNC 8011f508 300 X003
// MATCHING 8011f508 300
#include "TOBJ.H"
extern unsigned char D_1F8001A4;
extern short D_1F80019E;
extern void FUN_8004258c(TObj *, int);

void func_8011F508(TObj *o, TObj *e)
{
    if (o->active & 2) return;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x2d) >= 0x5b) return;
    if ((unsigned short)((o->h->p.whole - e->h->p.whole) + (e->box0 + o->box0)) > e->box1 + o->box1) return;
    if ((unsigned short)((o->y.p.whole - e->y.p.whole) + (e->box2 + o->box2)) > o->box3 + e->box3) return;
    if (D_1F8001A4) return;
    {
        int ah, bh;
        o->active = 2;
        bh = e->h->p.whole;
        ah = o->h->p.whole;
        o->b04 = 2;
        o->step = 0;
        o->state = 0;
        *((unsigned char *)o + 0xd1) = 1;
        o->animFrame = ah < bh;
    }
    FUN_8004258c(o, 2);
    D_1F80019E = 0;
}
