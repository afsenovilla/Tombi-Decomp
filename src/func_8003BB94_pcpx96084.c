// VERSION PCPX_96084 tomba 8003BB94 340
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x000 8011DD30 (src/updateChainedPartsObj.c): the radius byte is at +0xA1, not +0xA5.
#include "TOBJ.H"
extern short D_800A128C[];
extern short D_800A168C[];
extern void func_8003B83C_pcpx96084(TObj *);
extern void func_8003B4D4_pcpx96084(TObj *);
extern void func_8003B164_pcpx96084(TObj *);

void func_8003BB94_pcpx96084(TObj *o)
{
    int dy, dx;
    TObj *p;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = ((signed char *)o)[0xA1] * D_800A128C[o->animFrame] << 4;
    dx = ((signed char *)o)[0xA1] * D_800A168C[o->animFrame] << 4;
    p = (TObj *)o->d94;
    p->y.raw = o->y.raw + dy + (o->velY << 16);
    o->d34 = p->y.p.whole;
    p->a.raw = o->a.raw + dx;
    o->d30 = p->a.p.whole;
    o->box1 = (dx < 0 ? -dx >> 16 : dx >> 16);
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + ((dy < 0 ? -dy >> 16 : dy >> 16));
    switch (o->subtype) {
    case 0:
        func_8003B83C_pcpx96084(o);
        break;
    case 1:
        func_8003B4D4_pcpx96084(o);
        break;
    case 2:
        func_8003B164_pcpx96084(o);
        break;
    }
}
#endif
