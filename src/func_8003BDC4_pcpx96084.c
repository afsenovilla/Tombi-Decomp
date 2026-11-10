// VERSION PCPX_96084 tomba 8003BDC4 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Chained part on a straight arm: like func_8003BCE8, one swing table (D_800A12A4) and the arm offset +0xA1 << 16.
#include "TOBJ.H"
extern short D_800A12A4[];

void func_8003BDC4_pcpx96084(TObj *o)
{
    int dy, dx;
    TObj *p;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = *((signed char *)o + 0xA1) * D_800A12A4[o->animFrame] << 4;
    dx = *((signed char *)o + 0xA1) << 16;
    p = (TObj *)o->d94;
    p->y.raw = o->y.raw + dy + (o->velY << 16);
    o->d34 = p->y.p.whole;
    p->a.raw = o->a.raw + dx;
    o->d30 = p->a.p.whole;
    o->box1 = (dx < 0 ? -dx >> 16 : dx >> 16);
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + ((dy < 0 ? -dy >> 16 : dy >> 16));
}
#endif
