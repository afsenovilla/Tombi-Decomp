// VERSION SLPM_80165 x00 80117070 200
// FLAGS -O2 -G0
#if defined(VERSION_SLPM_80165)
// Demo version of func_8003BDC4_pcpx96084 (src/func_8003BDC4_pcpx96084.c): chained part on a straight arm, one
// swing table and the arm offset << 16; the arm length is the byte +0xA5 (PrePre 8: +0xA1).
#include "TOBJ.H"
extern short D_8006D7D0[];

void func_80117070_slpm80165(TObj *o)
{
    int dy, dx;
    TObj *p;
    o->d8c = (0x80 - o->animFrame) << 4;
    dy = *((signed char *)o + 0xA5) * D_8006D7D0[o->animFrame] << 4;
    dx = *((signed char *)o + 0xA5) << 16;
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
