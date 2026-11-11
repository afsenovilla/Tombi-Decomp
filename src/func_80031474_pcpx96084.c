// VERSION PCPX_96084 tomba 80031474 100
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x004 initPoolType04Sprite (src/x004/initPoolType04Sprite.c): -8 in place of -10, +0xA a
// halfword, scratchpad 0x1F80015C (PAL 0x1F8002D4).
#include "spad.h"
#include "TOBJ.H"
extern void *D_80013F20[];
extern void AnimLoadDuration(TObj *o);

void func_80031474_pcpx96084(TObj *o)
{
    int v;
    v = -8;
    o->w1e = 10;
    o->b0d = 0;
    *(short *)&o->b0a = 0;
    if (o->subtype == 0) v = 1;
    *(signed char *)&o->b0f = v;
    o->d3c = ((int *)&SPAD)[0x15C / 4];
    o->anim = D_80013F20[o->subtype];
    AnimLoadDuration(o);
}
#endif
