// VERSION PCPX_96084 tomba 8007DD58 288
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Scenery sprite with the first animation of D_800AEF04[subtype] and a 12x24 / 18x34 box; only subtype 0 animates.
#include "TOBJ.H"
extern void **D_800AEF04[];
extern int D_1F80015C;
extern void AnimLoadDuration(TObj *);
extern int func_80018550(TObj *);
extern int func_800180D8_pcpx96084(TObj *);
extern void freeObjectLayer7(TObj *);

void func_8007DD58_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->w1e = 8;
        o->b0d = 0x80;
        *(void * volatile *)&o->anim = *D_800AEF04[o->subtype];
        o->d3c = D_1F80015C;
        *(volatile short *)&o->box0 = 0xc;
        *(volatile short *)&o->box1 = 0x18;
        *(volatile short *)&o->box2 = 0x12;
        *(volatile short *)&o->box3 = 0x22;
        AnimLoadDuration(o);
        break;
    case 1:
        if (func_80018550(o) && o->subtype == 0) func_800180D8_pcpx96084(o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        freeObjectLayer7(o);
        break;
    }
}

#endif
