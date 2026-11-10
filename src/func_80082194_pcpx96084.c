// VERSION PCPX_96084 tomba 80082194 232
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
#include "TOBJ.H"
// PrePre 8 form of playerGrabCat4Type06 (src/playerGrabCat4Type06.c): grab point at +0xBC/+0xBE tested against b's box.
#define U16(o, k) (*(unsigned short *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
extern short D_1F8002A6;
extern TObj *D_800E2450;

void func_80082194_pcpx96084(TObj *o, TObj *p)
{
    if (p->b0c == 0 && p->subtype != 2
        && (unsigned short)(o->d->p.whole - p->d->p.whole + 45) < 91
        && (unsigned short)(p->box0 + (U16(o, 0xbc) - p->h->p.whole)) <= p->box1
        && (unsigned short)(p->box2 + (U16(o, 0xbe) - p->y.p.whole)) <= p->box3
        && !(o->animFrame & 1)) {
        short b;
        ((unsigned char *)o)[0x9a] = 2;
        b = p->box0;
        S16(o, 0xb2) = 0x14;
        o->velY = 0;
        D_1F8002A6 = 0;
        D_800E2450 = p;
        S16(o, 0xb0) = -b;
    }
}
#endif
