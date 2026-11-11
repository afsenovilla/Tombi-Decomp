// VERSION PCPX_96084 tomba 8007D320 200
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// Animated scenery sprite: animation by subtype from D_800140E0, drawn while on screen.
#include "TOBJ.H"
extern void *D_800140E0[];
extern int D_1F80015C;
extern void AnimLoadDuration(TObj *);
extern void func_80018550(TObj *);
extern void freeObjectLayer7(TObj *);

void func_8007D320_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->w1e = 8;
        o->b0d = 0;
        *(void * volatile *)&o->anim = D_800140E0[o->subtype];
        o->d3c = D_1F80015C;
        AnimLoadDuration(o);
        break;
    case 1:
        func_80018550(o);
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
