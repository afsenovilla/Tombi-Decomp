// VERSION PCPX_96084 tomba 8007957C 284
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x000 8012D7FC (komaPigSubtype07Step.c): halfword box table, field +0xA8 (PAL +0xAC).
#include "TOBJ.H"
#include "raw7.h"
extern unsigned short D_800AE9A4[];
extern unsigned short *D_8001347C;
extern char D_800A1898[];
extern void func_800707EC(TObj *);

void func_8007957C_pcpx96084(TObj *o)
{
    unsigned short *p;
    TObj *q;
    switch (o->state) {
    case 0:
        PTR(o, 0x28) = D_800A1898;
        o->b69 = 0;
        S16(o, 0xa8) = 0x2d;
        o->state = o->state + 1;
        o->anim = D_8001347C;
        p = D_800AE9A4 + D_8001347C[1] * 4;
        o->box0 = *p++;
        o->box1 = *p++;
        o->box2 = *p;
        o->box3 = p[1];
        o->animTimer = ((unsigned short *)o->anim)[3] & 0x3fff;
        break;
    case 1:
        func_800707EC(o);
        q = (TObj *)o->d94;
        o->h->p.whole = q->h->p.whole;
        o->y.p.whole = q->y.p.whole - 0x17;
        o->d8c = (-(q->d8c >> 4)) & 0xff;
        break;
    }
}
#endif
