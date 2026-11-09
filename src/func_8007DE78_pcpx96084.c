// VERSION PCPX_96084 tomba 8007DE78 196
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 version of PAL x001 80131904 (src/x001/updateAnimProp.c): the word copied into +0x3C is scratchpad
// 0x1F80015C (PAL 0x1F8002D4).
#include "spad.h"
#include "TOBJ.H"

extern void *D_80014150;
extern void AnimLoadDuration(TObj *);
extern int func_80018550(TObj *);
extern void freeObjectLayer7(TObj *);

void func_8007DE78_pcpx96084(TObj *o)
{
    switch (o->b04) {
    case 0:
        o->box0 = 8;
        o->b0d = 0;
        o->box1 = 0x10;
        o->box3 = 0xc;
        o->box2 = 0;
        o->w1e = 8;
        o->anim = D_80014150;
        o->d3c = ((int *)&SPAD)[0x15C / 4];
        o->b04++;
        AnimLoadDuration(o);
        break;
    case 1:
        func_80018550(o);
        break;
    case 2:
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}
#endif
