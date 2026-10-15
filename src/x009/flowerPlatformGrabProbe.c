// FUNC 801212c4 368 X009
// MATCHING 801212c4 368
#include "spad.h"
#include "TOBJ.H"
#include "raw7.h"

extern short columnProbeB(TObj *, short, short, int);

void flowerPlatformGrabProbe(TObj *o, TObj *p)
{
    int dx;
    short r;
    int x;
    unsigned short ph;
    int y;
    unsigned short py;

    if ((unsigned short)(o->d->p.whole - p->d->p.whole + 0x2d) >= 0x5b) return;
    dx = 0x10;
    if (o->animFrame & 1) {
        dx = -0x10;
    }
    x = dx + (unsigned short)o->h->p.whole;
    ph = p->h->p.whole;
    if ((unsigned short)(p->box0 + (x - ph)) >= p->box1) return;
    y = U16(o, 0xea);
    py = p->y.p.whole;
    if ((unsigned short)(p->box2 + (y - py)) > p->box3) return;
    {
        short a = x - (ph - p->box0);
        short b = y - (py + (p->box3 - p->box2));
        r = columnProbeB(o, a, b, p->subtype);
    }
    if (r == 0) return;
    if (r == 2) {
        o->b9e = 5;
    } else {
        o->b9e = 6;
    }
    if (o->animFrame & 1) {
        o->h->p.whole -= 0xc;
    } else {
        o->h->p.whole += 0xc;
    }
    SPAD.s19e = 0;
    (*(TObj **)&SPAD.p3c0) = p;
}
