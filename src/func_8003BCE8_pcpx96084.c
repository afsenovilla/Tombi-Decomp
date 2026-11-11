// VERSION PCPX_96084 tomba 8003BCE8 220
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of PAL x000 8011DD30 (updateChainedPartsObj.c), from the SLPM-80165 C; the swing radius byte is +0xA1 here (PAL +0xA5).
#include "TOBJ.H"
extern short D_800A128C[];
extern short D_800A168C[];

void func_8003BCE8_pcpx96084(TObj *o)
{
    int dy, dx;
    TObj *p;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = *((signed char *)o + 0xA1) * D_800A128C[o->animFrame] << 4;
    dx = *((signed char *)o + 0xA1) * D_800A168C[o->animFrame] << 4;
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
        break;
    case 1:
        break;
    case 2:
        break;
    }
}
#endif
