// VERSION SLPM_80165 x00 801171FC 196
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8003BE8C_pcpx96084 (src/func_8003BE8C_pcpx96084.c): chained part without a child, the swing offset goes to +0x30/+0x34;
// the arm length is the byte +0xA5 (PrePre 8: +0xA1).
#include "TOBJ.H"
extern short D_8006D7B8[];
extern short D_8006DBB8[];

void func_801171FC_slpm80165(TObj *o)
{
    int dy, dx;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = *((signed char *)o + 0xA5) * D_8006D7B8[o->animFrame] << 4;
    dx = *((signed char *)o + 0xA5) * D_8006DBB8[o->animFrame] << 4;
    o->d34 = (o->y.raw + dy) >> 16;
    o->d30 = (o->a.raw + dx) >> 16;
    o->box1 = (dx < 0 ? -dx >> 16 : dx >> 16);
    o->box0 = o->box1 = o->box1;
    o->box3 = o->velX + ((dy < 0 ? -dy >> 16 : dy >> 16));
}
#endif
