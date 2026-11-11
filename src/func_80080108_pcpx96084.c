// VERSION PCPX_96084 tomba 80080108 252
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Attached sprite: placed 16 px beside its parent (+0x90) on the side of its facing (parent's - 2), animation
// D_80014DF4, then drawn while on screen.
#include "TOBJ.H"
#define P(o) ((TObj *)(o)->d90)
extern void *D_80014DF4;
extern void AnimLoadDuration(TObj *);
extern void func_80018550(TObj *);

void func_80080108(TObj *o)
{
    switch (o->state) {
    case 0:
        *(volatile short *)&o->wa8 = 0;
        o->anim = D_80014DF4;
        o->animFrame = P(o)->animFrame - 2;
        if (o->animFrame & 1)
            o->h->p.whole = P(o)->h->p.whole + 16;
        else
            o->h->p.whole = P(o)->h->p.whole - 16;
        o->y.p.whole = P(o)->y.p.whole;
        o->d->p.whole = P(o)->d->p.whole;
        o->d84 = 0;
        o->d88 = 0;
        o->d8c = 0;
        o->state++;
        AnimLoadDuration(o);
        break;
    case 1:
        func_80018550(o);
        break;
    case 2:
        break;
    case 3:
        break;
    }
}
#endif
