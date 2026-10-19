// FUNC 80126d18 268 X003
// MATCHING 80126d18 268
#include "spad.h"
#include "TOBJ.H"

extern unsigned char D_80135CB0[];
extern void *D_80139578;
extern void AnimLoadDuration(TObj *);
extern short probeCollisionAtDepthA(TObj *, short, short);

void spikyLizardSubtype0Step(TObj *o)
{
    unsigned char *p;

    switch (o->state) {
    case 0:
        o->state++;
        o->wac = 0x1e;
        o->anim = D_80139578;
        p = D_80135CB0 + (o->wac << 2);
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        AnimLoadDuration(o);
        break;
    case 1:
        o->y.p.whole -= 4;
        if (probeCollisionAtDepthA(o, o->h->p.whole, o->y.p.whole - 0x10) != 0) {
            o->d8c = (-SPAD.s27e * 4 + 0x80) & 0xff;
            o->state++;
        }
        break;
    case 2:
        break;
    }
}
