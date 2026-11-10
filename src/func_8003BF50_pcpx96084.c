// VERSION PCPX_96084 tomba 8003BF50 196
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Variant of func_8003BCE8_pcpx96084 (updateChainedPartsObj) without a child part: the swing offset goes to +0x30/+0x34.
#include "TOBJ.H"
extern short D_800A128C[];
extern short D_800A168C[];

void func_8003BF50_pcpx96084(TObj *o)
{
    int dy, dx;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = *((signed char *)o + 0xA1) * D_800A128C[o->animFrame] << 4;
    dx = *((signed char *)o + 0xA1) * D_800A168C[o->animFrame] << 4;
    o->d34 = (o->y.raw + dy) >> 16;
    o->d30 = (o->a.raw + dx) >> 16;
    o->box1 = (dx < 0 ? -dx >> 16 : dx >> 16);
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + ((dy < 0 ? -dy >> 16 : dy >> 16));
}
#endif
