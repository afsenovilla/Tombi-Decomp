// FUNC 8011c7fc 628 X003
// MATCHING 8011c7fc 628
#include "gvars.h"
/* Real size 628 B: includes the csv piece func_8011CA00 (112 B), which is the tail of this function. */
#include "TOBJ.H"

extern TObj PLAYER;
extern unsigned char D_8009D2C2;
extern short D_8007A1F0[], D_8007A5F0[];
extern void *D_80139870[];
extern int angleFromDelta(int, int);
extern int FUN_8001fec0(TObj *);
extern void FUN_8001fe6c(TObj *);

void func_8011C7FC(TObj *o)
{
    TObj *p = &PLAYER;
    int a, dy, d;

    switch (o->state) {
    case 0:
        if (D_8009D2C2 != 2)
            break;
        o->velH = 0x800;
        ((unsigned char*)&GVARS.b3f)[0] = 1;
        ((unsigned char*)&GVARS.b42)[0] = 1;
        p->active = 5;
        p->b04 = 5;
        p->visible = 0;
        p->step = 0x40;
        o->state++;
        break;
    case 1:
        a = (unsigned char)angleFromDelta(o->h->p.whole - p->h->p.whole, o->y.p.whole - p->y.p.whole);
        o->wb0 = a;
        d = (o->velH * D_8007A5F0[a]) >> 12;
        dy = (o->velH * D_8007A1F0[a]) >> 12;
        p->h->raw += d << 8;
        p->y.raw += dy << 8;
        d = p->h->p.whole - o->h->p.whole + 0x10;
        if ((unsigned short)d >= 0x21)
            break;
        o->velH = 0x400;
        d = p->y.p.whole - o->y.p.whole + 0x10;
        if ((unsigned short)d >= 0x21)
            break;
        ((unsigned char*)&GVARS.b3f)[0] = 0;
        ((unsigned char*)&GVARS.b42)[0] = 0;
        p->active = 2;
        p->h->p.whole = o->h->p.whole;
        p->y.p.whole = o->y.p.whole;
        p->d->p.whole = o->d->p.whole;
        o->state++;
        break;
    case 2:
        if (FUN_8001fec0(o)) {
            D_8009D2C2 = 0;
            o->step = 3;
            o->state = 0;
            o->wac = 3;
            o->y.p.whole -= 0x30;
            o->anim = D_80139870[0];
            FUN_8001fe6c(o);
        }
        break;
    }
}
