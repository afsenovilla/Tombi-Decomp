// VERSION PCPX_96084 tomba 80032C58 264
// FLAGS -O2 -G0
// ASPSX.PCPX_96084 2.34
#if defined(VERSION_PCPX_96084)
// PrePre 8 form of updateA00KomaPigSpawner (src/updateA00KomaPigSpawner.c); the volatile store keeps the anim store before the scratchpad load.
extern int D_1F80015C;
#include "TOBJ.H"
extern int func_80018550(TObj *);
extern void func_80032A9C(TObj *);
extern void AnimLoadDuration(TObj *);
extern void func_8001A87C_pcpx96084(TObj *);
extern void *D_80014000[];
extern int DAT_1f8002d4[];
void func_80032C58_pcpx96084(TObj *o)
{
    unsigned char s = o->b04;
    switch (s) {
    case 0:
        o->b04 = s + 1;
        o->w1e = 0xc;
        o->b0d = 0;
        *(short *)&o->b0a = 0;
        *(signed char *)&o->b0f = -5;
        o->animFrame = 0;
        *(void * volatile *)&o->anim = D_80014000[o->subtype];
        o->d3c = D_1F80015C;
        AnimLoadDuration(o);
        break;
    case 1:
        if (func_80018550(o) != 0 && o->subtype == 0)
            func_80032A9C(o);
        break;
    case 2:
        o->b04 = s + 1;
        break;
    case 3:
        func_8001A87C_pcpx96084(o);
        break;
    }
}
#endif
