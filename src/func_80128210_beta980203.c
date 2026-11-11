// VERSION BETA_980203 x00 80128210 164
#if defined(VERSION_BETA_980203)
// Beta version: USA C of src/x009/areaMechanismsPlayerCollision.c for subtype 1, a 0x5b-wide box, no GCD.bcfee test, sets wa8 = 4 only.
#include "gcd.h"
#include "TOBJ.H"


void areaMechanismsPlayerCollision(TObj *o, TObj *e)
{
    if (e->subtype != 1) return;
    if ((unsigned short)(o->d->p.whole - e->d->p.whole + 0x3c) >= 0x79) return;
    if ((unsigned short)(o->h->p.whole - e->h->p.whole) >= 0x5b) return;
    if ((unsigned short)(o->box2 + (o->y.p.whole - e->y.p.whole + 0x20)) > o->box3 + 0x40) return;
    *(unsigned char *)&o->wa8 = 4;
}
#endif
